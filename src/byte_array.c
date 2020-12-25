#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "byte_array.h"

Byte_Array initialize_byte_array(u64 reserve) {
  Byte_Array r;
  r.reserved = reserve;
  r.length = 0;
  r.data = malloc(r.reserved * sizeof(u8));
  assert(r.data != NULL && "initial code array malloc failed");
  return r;
}

int byte_array_size(Byte_Array code) {
  return code.length * sizeof(u8);
}

void byte_array_push(Byte_Array *code, u8 push_value) {
  if(code->length == code->reserved) {
    // Expand array
    u64 new_reserved = code->reserved * 2;
    u8 *new_data = realloc(code->data, new_reserved);
    assert(new_data != NULL && "realloc to expand byte array failed");
    code->reserved = new_reserved;
    code->data = new_data;
  }
  code->data[code->length] = push_value;
  code->length++;
}
