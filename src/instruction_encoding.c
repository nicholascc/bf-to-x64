#include "instruction_encoding.h"
#include "byte_array.h"
#include "stdbool.h"

void push_rex(Byte_Array *program, bool w, bool r, bool x, bool b) {
  byte_array_push(program, (0b0100 << 4) | (w << 3) | (r << 2) | (x << 1) | b);
}

void push_rex_w(Byte_Array *program) {
  push_rex(program, true, false, false, false);
}

void push_mod_rm(Byte_Array *program, u8 mod, u8 rm, u8 reg) {
  byte_array_push(program, ((mod & 0x3) << 6) | ((reg & 0x7) << 3) | (rm & 0x7));
}

void push_sib(Byte_Array *program, u8 scale, u8 index, u8 base) {
  byte_array_push(program, ((scale & 0x3) << 6) | ((index & 0x7) << 3) | (base & 0x7));
}

void push_dword(Byte_Array *program, u32 dword) {
  byte_array_push(program, (dword & 0x000000ffUL)      );
  byte_array_push(program, (dword & 0x0000ff00UL) >>  8);
  byte_array_push(program, (dword & 0x00ff0000UL) >> 16);
  byte_array_push(program, (dword & 0xff000000UL) >> 24);
}

void push_qword(Byte_Array *program, u64 qword) {
  push_dword(program, (qword & 0x00000000ffffffffULL)      );
  push_dword(program, (qword & 0xffffffff00000000ULL) >> 32);
}




void push_mov_r32_imm32(Byte_Array *program, r32_index op1, u32 op2) {
  byte_array_push(program, 0xb8+op1); // opcode + rd
  push_dword(program, op2); // immediate operand
}

void push_mov_r64_imm64(Byte_Array *program, r32_index op1, u64 op2) {
  push_rex_w(program);
  byte_array_push(program, 0xb8+op1); // opcode + rd
  push_qword(program, op2); // immediate operand
}

void push_mov_r32_r32(Byte_Array *program, r32_index op1, r32_index op2) {
  byte_array_push(program, 0x8b);
  push_mod_rm(program, 0b11, op2, op1);
}

void push_mov_r64_r64(Byte_Array *program, r32_index op1, r32_index op2) {
  push_rex_w(program);
  byte_array_push(program, 0x8b);
  push_mod_rm(program, 0b11, op2, op1);
}

// mov r8, [r64 + r64]
void push_mov_r8_mem8_r_plus_r(Byte_Array *program, r32_index op1, r32_index addend1, r32_index addend2) {
  byte_array_push(program, 0x8a);
  push_mod_rm(program, 0b00, 0b100, op1);
  push_sib(program, 0b00, addend1, addend2);
}



void push_inc_or_dec_r32(Byte_Array *program, r32_index op, bool is_increment) {
  byte_array_push(program, 0xff);
  push_mod_rm(program, 0b11, op, !is_increment);
}

void push_inc_or_dec_r64(Byte_Array *program, r32_index op, bool is_increment) {
  push_rex_w(program);
  byte_array_push(program, 0xff);
  push_mod_rm(program, 0b11, op, !is_increment);
}

void push_inc_or_dec_mem8_r_plus_r(Byte_Array *program, r32_index op1, r32_index op2, bool is_increment) {
  push_rex_w(program);
  byte_array_push(program, 0xfe);
  push_mod_rm(program, 0b00, 0b100, !is_increment);
  push_sib(program, 0b00, op1, op2);
}



void push_push_r64(Byte_Array *program, r32_index op) {
  byte_array_push(program, 0x50 + op);
}

void push_pop_r64(Byte_Array *program, r32_index op) {
  byte_array_push(program, 0x58 + op);
}

void push_call_r64(Byte_Array *program, r32_index op) {
  byte_array_push(program, 0xff);
  push_mod_rm(program, 0b11, op, 2);
}

void push_ret(Byte_Array *program) {
  byte_array_push(program, 0xc3);
}
