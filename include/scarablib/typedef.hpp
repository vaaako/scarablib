#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <cstdint>

// int
typedef int8_t  int8;   // signed char
typedef int16_t int16; // short
typedef int32_t int32; // int
typedef int64_t int64; // long

// unsigned
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

// float
typedef double float64;

template <typename T>
using vec2 = glm::vec<2, T>;
template <typename T>
using vec3 = glm::vec<3, T>;
template <typename T>
using vec4 = glm::vec<4, T>;

