#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "dynamic_array.h"

Byte_Array initialize_byte_array(u64 reserve) {
  Byte_Array r;
  r.reserved = reserve;
  r.length = 0;
  r.data = malloc(r.reserved * sizeof(u8));
  assert(r.data != NULL && "initial dynamic array malloc failed");
  return r;
}

u64 byte_array_size(Byte_Array arr) {
  return arr.length * sizeof(u8);
}

void byte_array_push(Byte_Array *arr, u8 push_value) {
  if(arr->length == arr->reserved) {
    // Expand array
    u64 new_reserved = arr->reserved * 2;
    u8 *new_data = realloc(arr->data, new_reserved * sizeof(u8));
    assert(new_data != NULL && "realloc to expand dynamic array failed");
    arr->reserved = new_reserved;
    arr->data = new_data;
  }
  arr->data[arr->length] = push_value;
  arr->length++;
}

void byte_array_pop(Byte_Array *arr) {
  assert(arr->length > 0 && "failed to pop; array has 0 elements");
  arr->length--;
}





Dword_Array initialize_dword_array(u64 reserve) {
  Dword_Array r;
  r.reserved = reserve;
  r.length = 0;
  r.data = malloc(r.reserved * sizeof(u64));
  assert(r.data != NULL && "initial dynamic array malloc failed");
  return r;
}

u64 dword_array_size(Dword_Array arr) {
  return arr.length * sizeof(u64);
}

void dword_array_push(Dword_Array *arr, u32 push_value) {
  if(arr->length == arr->reserved) {
    // Expand array
    u64 new_reserved = arr->reserved * 2;
    u32 *new_data = realloc(arr->data, new_reserved * sizeof(u64));
    assert(new_data != NULL && "realloc to expand dynamic array failed");
    arr->reserved = new_reserved;
    arr->data = new_data;
  }
  arr->data[arr->length] = push_value;
  arr->length++;
}

void dword_array_pop(Dword_Array *arr) {
  assert(arr->length > 0 && "failed to pop; array has 0 elements");
  arr->length--;
}
