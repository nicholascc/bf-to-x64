#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "dynamic_array.h"
#include "instruction_encoding.h"
#include "bf_to_x64.h"

typedef u64 (*JitFunction)();

u8 tape[30000];

u64 dispatch(Byte_Array program) {
  i64 program_size = byte_array_size(program);

  i32 protection = PAGE_READWRITE;
  void *memory = VirtualAlloc(/*addr=*/NULL, /*length=*/program_size,
                              /*allocation_type=*/MEM_COMMIT | MEM_RESERVE,
                              /*protection=*/protection);
  assert(memory != NULL && "memory allocation failed");

  memcpy(memory, program.data, program_size);

  {
    bool result = VirtualProtect(memory, program_size, PAGE_EXECUTE, &protection);
    assert(result && "failed to change permissions of memory");
  }

  JitFunction function = *(JitFunction*)&memory;
  u64 returned = function();

  {
    bool result = VirtualFree(memory, 0, MEM_RELEASE);
    u32 error = GetLastError();
    assert(result && "failed to free memory");
  }

  return returned;
}

int main(int argc, char *argv[]) {

  if(argc < 2) {
    printf("You must provide a .bf file to execute.\n");
    exit(EXIT_FAILURE);
  } else if(argc > 2) {
    printf("Too many arguments provided.\n");
    exit(EXIT_FAILURE);
  }

  char *path = argv[1];

  FILE *f = fopen(path, "r");
  if(f == NULL) {
    printf("Failed to open file '%s'.\n", path);
    exit(EXIT_FAILURE);
  }

  Byte_Array program = initialize_byte_array(1);

  push_bf(&program, f, tape);
  assert(fclose(f) == 0 && "file close failed");

  printf("Generated x64 code:\n");

  for(int i = 0; i < program.length; i++) {
    if(i%8 == 0) {
      printf("\n");
    }
    printf("%02X ", program.data[i]);
  }

  printf("\n\nExecuting '%s'...\n\n", path);

  u64 returned = dispatch(program);

  printf("\n\nEnded at head position: %llu\n", returned);

  printf("First 64 memory cells at end of execution:\n");

  for(int i = 0; i < 64; i++) {
    if(i%8 == 0) {
      printf("\n");
    }
    printf("%02X ", tape[i]);
  }
  printf("\n");
}
