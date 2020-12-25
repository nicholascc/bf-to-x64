#ifndef INSTRUCTION_ENCODING_H
#define INSTRUCTION_ENCODING_H


#include "byte_array.h"
#include "types.h"
#include "stdbool.h"

typedef enum r32_index {
  EAX = 0,
  ECX = 1,
  EDX = 2,
  EBX = 3,

  ESP = 4,
  EBP = 5,
  ESI = 6,
  EDI = 7
} r32_index;

void push_mov_r32_imm32(Byte_Array *program, r32_index op1, u32 op2);
void push_mov_r64_imm64(Byte_Array *program, r32_index op1, u64 op2);
void push_mov_r32_r32(Byte_Array *program, r32_index op1, r32_index op2);
void push_mov_r64_r64(Byte_Array *program, r32_index op1, r32_index op2);
void push_mov_r8_mem8_r_plus_r(Byte_Array *program, r32_index op1, r32_index addend1, r32_index addend2);

void push_inc_or_dec_r32(Byte_Array *program, r32_index op, bool is_increment);
void push_inc_or_dec_r64(Byte_Array *program, r32_index op, bool is_increment);
void push_inc_or_dec_mem8_r_plus_r(Byte_Array *program, r32_index op1, r32_index op2, bool is_increment);

void push_push_r64(Byte_Array *program, r32_index op);
void push_pop_r64 (Byte_Array *program, r32_index op);

void push_call_r64(Byte_Array *program, r32_index op);
void push_ret(Byte_Array *program);


#endif /* end of include guard: INSTRUCTION_ENCODING_H */
