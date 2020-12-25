#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include "types.h"

typedef struct Byte_Array {
  u64 reserved;
  u64 length;
  u8 *data;
} Byte_Array;

Byte_Array initialize_byte_array(u64 reserve);

int byte_array_size(Byte_Array code);

void byte_array_push(Byte_Array *code, u8 data);


#endif /* end of include guard: BYTE_ARRAY_H */
