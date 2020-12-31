#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "dynamic_array.h"
#include "instruction_encoding.h"



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



  Dword_Array open_brace_index_stack = initialize_dword_array(1);

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
        // cmp [rax + rbx], 0
        push_cmp_mem8_r_plus_r_imm8(program, EAX, EBX, 0);
        // je (to end of this pair of '[' and ']')
        push_je_rel32(program, 0); // this will be filled in with the correct destination later

        dword_array_push(&open_brace_index_stack, program->length);
        break;
      case ']':
        // cmp [rax + rbx], 0
        push_cmp_mem8_r_plus_r_imm8(program, EAX, EBX, 0);

        // jne (to beginning of this pair of '[' and ']')
        {
          i64 index_to_jump_to = (i64)open_brace_index_stack.data[open_brace_index_stack.length-1];
          i32 dist = index_to_jump_to - (i64)(program->length + SIZE_OF_JNE_REL32);
          push_jne_rel32(program, dist);
        }

        // fill in the correct jump destination for the corresponding '['
        {
          // index of start of dword denoting relative offset of jump
          u64 id = open_brace_index_stack.data[open_brace_index_stack.length-1]-4;
          i64 index_to_jump_to = program->length - 1;
          i32 dist = (program->length-1) - (id+3);
          program->data[id]   = (dist & 0x000000ffUL);
          program->data[id+1] = (dist & 0x0000ff00UL) >>  8;
          program->data[id+2] = (dist & 0x00ff0000UL) >> 16;
          program->data[id+3] = (dist & 0xff000000UL) >> 24;
        }

        dword_array_pop(&open_brace_index_stack);
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

  free(open_brace_index_stack.data);

  // function epilogue
  // pop rbx
  push_pop_r64(program, EBX);
  // ret
  push_ret(program);
}
