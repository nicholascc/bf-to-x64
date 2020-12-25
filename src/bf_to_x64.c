#include <stdio.h>

#include "types.h"
#include "instruction_encoding.h"
#include "byte_array.h"



// rax is index                 - caller-saved
// rbx is pointer to head       - callee-saved
// rdx is pointer to print_char - caller-saved

u32 print_char(u8 c) {
  printf("%c", c);
  return 0;
}


void push_bf(Byte_Array *program, FILE *file, u8 *tape_ptr) {
  // function prologue
  // push rbx - rbx is callee saved
  push_push_r64(program, EBX);

  // mov rax, 0 (start at index 0 of tape)
  push_mov_r64_imm64(program, EAX, 0);
  // mov rbx, (pointer to tape)
  push_mov_r64_imm64(program, EBX, (u64)tape_ptr);
  // mov rdx, (pointer to print_char)
  push_mov_r64_imm64(program, EDX, (u64)(&print_char));


  char c = fgetc(file);

  while(c != EOF) {
    switch(c) {
      case '>':
        // inc rax
        push_inc_or_dec_r64(program, EAX, true);
        break;
      case '<':
        // dec rax
        push_inc_or_dec_r64(program, EAX, false);
        break;
      case '+':
        // inc (8-bit memory address referenced by:) [rax + rbx]
        push_inc_or_dec_mem8_r_plus_r(program, EAX, EBX, true);
        break;
      case '-':
        // dec (8-bit memory address referenced by:) [rax + rbx]
        push_inc_or_dec_mem8_r_plus_r(program, EAX, EBX, false);
        break;
      case '[':
        break;
      case ']':
        break;
      case '.':
        // push rax
        push_push_r64(program, EAX);
        // push rdx
        push_push_r64(program, EDX);
        // mov cl, [rax+rbx]
        push_mov_r8_mem8_r_plus_r(program, ECX, EAX, EBX);
        // call rdx
        push_call_r64(program, EDX);
        // pop rdx
        push_pop_r64(program, EDX);
        // pop rax
        push_pop_r64(program, EAX);
        break;
      case ',':
        break;
      default:;
    }
    c = fgetc(file);
  }

  // function epilogue
  // pop rbx
  push_pop_r64(program, EBX);
  // ret
  push_ret(program);

}
