#pragma once

#include <stdint.h>

// signed integers

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// unsigned integers

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// floats

typedef float f32;
typedef double f64;


// glm types

#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4x4;
using glm::dot;
using glm::cross;
using glm::normalize;
