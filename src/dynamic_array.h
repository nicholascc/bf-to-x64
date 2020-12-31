#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "types.h"

typedef struct Byte_Array {
  u64 reserved;
  u64 length;
  u8 *data;
} Byte_Array;

Byte_Array initialize_byte_array(u64 reserve);

u64 byte_array_size(Byte_Array arr);

void byte_array_push(Byte_Array *arr, u8 value);

void byte_array_pop(Byte_Array *arr);


typedef struct Dword_Array {
  u64 reserved;
  u64 length;
  u32 *data;
} Dword_Array;

Dword_Array initialize_dword_array(u64 reserve);

u64 dword_array_size(Dword_Array arr);

void dword_array_push(Dword_Array *arr, u32 value);

void dword_array_pop(Dword_Array *arr);


#endif /* end of include guard: DYNAMIC_ARRAY_H */
