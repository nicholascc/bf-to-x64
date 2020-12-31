#ifndef BF_TO_X64_H
#define BF_TO_X64_H

#include <stdio.h>

#include "types.h"
#include "dynamic_array.h"

void push_bf(Byte_Array *program, FILE *file, u8 *tape_ptr);

#endif /* end of include guard: BF_TO_X64_H */
