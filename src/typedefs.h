#pragma once

#include <stdint.h>
#include <stdlib.h>

#define ASSERT(_e, ...)                                                        \
  if (!(_e)) {                                                                 \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  }

#define COL_RESET "\033[0m"
#define COL_RED "\033[0;31m"
#define COL_GREEN "\033[0;32m"
#define COL_YELLOW "\033[0;33m"
#define COL_CYAN "\033[0;36m"

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;

/* Vector */
struct vec2i_t {
  i32 x, y;
};
typedef struct vec2i_t v2i;

struct vec2f_t {
  f32 x, y;
};
typedef struct vec2f_t v2f;