/* 
 * Copyright © 2012 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Segovia <benjamin.segovia@intel.com>
 */

#include "string"
namespace gbe {
std::string sim_vector_str = 
"/*\n"
" * Copyright 2012 Intel Corporation\n"
" *\n"
" * Permission is hereby granted, free of charge, to any person obtaining a\n"
" * copy of this software and associated documentation files (the \"Software\"),\n"
" * to deal in the Software without restriction, including without limitation\n"
" * the rights to use, copy, modify, merge, publish, distribute, sublicense,\n"
" * and/or sell copies of the Software, and to permit persons to whom the\n"
" * Software is furnished to do so, subject to the following conditions:\n"
" *\n"
" * The above copyright notice and this permission notice (including the next\n"
" * paragraph) shall be included in all copies or substantial portions of the\n"
" * Software.\n"
" *\n"
" * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
" * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
" * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL\n"
" * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
" * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING\n"
" * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n"
" * DEALINGS IN THE SOFTWARE.\n"
" */\n"
"\n"
"/**\n"
" * \\file sim_vector.h\n"
" * \\author Benjamin Segovia <benjamin.segovia@intel.com>\n"
" *\n"
" * c++ class helper for the simulator\n"
" */\n"
"\n"
"#ifndef __GBE_SIM_VECTOR_H__\n"
"#define __GBE_SIM_VECTOR_H__\n"
"\n"
"#include <xmmintrin.h>\n"
"#include <emmintrin.h>\n"
"#include <pmmintrin.h>\n"
"#include <smmintrin.h>\n"
"#include <stdint.h>\n"
"#include <cmath>\n"
"\n"
"#define INLINE inline __attribute__((always_inline))\n"
"#define ID(X) (X)\n"
"#define PS2SI(X) _mm_castps_si128(X)\n"
"#define SI2PS(X) _mm_castsi128_ps(X)\n"
"\n"
"/* Some extra SSE functions */\n"
"template<size_t i0, size_t i1, size_t i2, size_t i3>\n"
"INLINE const __m128 shuffle(const __m128& b) {\n"
"  return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(b), _MM_SHUFFLE(i3, i2, i1, i0)));\n"
"}\n"
"template<size_t i> INLINE\n"
"__m128 expand(const __m128& b) { \n"
"  return shuffle<i, i, i, i>(b);\n"
"}\n"
"template<size_t index_0, size_t index_1, size_t index_2, size_t index_3>\n"
"INLINE const __m128i shuffle(const __m128i& a) {\n"
"  return _mm_shuffle_epi32(a, _MM_SHUFFLE(index_3, index_2, index_1, index_0));\n"
"}\n"
"template<size_t index>\n"
"INLINE const __m128i expand(const __m128i& b) {\n"
"  return shuffle<index, index, index, index>(b);\n"
"}\n"
"\n"
"/*! Base structure for scalar double word */\n"
"union scalar_dw { uint32_t u; int32_t s; float f; };\n"
"\n"
"/*! Base structure for scalar mask */\n"
"union scalar_m { uint32_t u; int32_t s; float f; };\n"
"\n"
"/*! Base structure for vectors 4 / 8 / 16 / 32 double words */\n"
"template <uint32_t vectorNum>\n"
"struct simd_dw {\n"
"  INLINE simd_dw(void) {}\n"
"  INLINE simd_dw(const scalar_dw &s) {\n"
"    for (uint32_t i = 0; i < vectorNum; ++i) m[i] = _mm_load1_ps(&s.f);\n"
"  }\n"
"  simd_dw &operator= (const scalar_dw &s) {\n"
"    for (uint32_t i = 0; i < vectorNum; ++i) m[i] = _mm_load1_ps(&s.f);\n"
"    return *this;\n"
"  }\n"
"  __m128 m[vectorNum];\n"
"};\n"
"\n"
"/*! Base structure for 4 / 8 / 16 / 32 booleans (m stands for \"mask\") */\n"
"template <uint32_t vectorNum>\n"
"struct simd_m {\n"
"  INLINE simd_m(void) {}\n"
"  INLINE simd_m(scalar_m s) {\n"
"    for (uint32_t i = 0; i < vectorNum; ++i) m[i] = _mm_load1_ps(&s.f);\n"
"  }\n"
"  __m128 m[vectorNum];\n"
"};\n"
"\n"
"/*! To cast through memory */\n"
"union cast_dw {\n"
"  INLINE cast_dw(uint32_t u0, uint32_t u1, uint32_t u2, uint32_t u3) {\n"
"    u[0] = u0; u[1] = u1; u[2] = u2; u[3] = u3;\n"
"  }\n"
"  INLINE cast_dw(int32_t s0, int32_t s1, int32_t s2, int32_t s3) {\n"
"    s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3;\n"
"  }\n"
"  INLINE cast_dw(float f0, float f1, float f2, float f3) {\n"
"    f[0] = f0; f[1] = f1; f[2] = f2; f[3] = f3;\n"
"  }\n"
"  INLINE cast_dw(const __m128 &v) : v(v) {}\n"
"  INLINE cast_dw(const __m128i &vi) : vi(vi) {}\n"
"  INLINE cast_dw(void) {}\n"
"  __m128 v;\n"
"  __m128i vi;\n"
"  uint32_t u[4];\n"
"  int32_t s[4];\n"
"  float f[4];\n"
"};\n"
"static const cast_dw alltrue(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);\n"
"\n"
"/* Some convenient typedefs */\n"
"typedef scalar_dw  simd1dw;\n"
"typedef simd_dw<1> simd4dw;\n"
"typedef simd_dw<2> simd8dw;\n"
"typedef simd_dw<4> simd16dw;\n"
"typedef simd_dw<8> simd32dw;\n"
"typedef scalar_m   simd1m;\n"
"typedef simd_m<1>  simd4m;\n"
"typedef simd_m<2>  simd8m;\n"
"typedef simd_m<4>  simd16m;\n"
"typedef simd_m<8>  simd32m;\n"
"\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"// Vector instructions\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"/* Simple function to get the number of element per vector */\n"
"template <uint32_t vectorNum>\n"
"INLINE uint32_t elemNum(const simd_dw<vectorNum> &x) {\n"
"  return 4 * vectorNum;\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE uint32_t elemNum(const simd_m<vectorNum> &x) {\n"
"  return 4 * vectorNum;\n"
"}\n"
"\n"
"/* Build an integer mask from the mask vectors */\n"
"template <uint32_t vectorNum>\n"
"INLINE uint32_t mask(const simd_m<vectorNum> v) {\n"
"  uint32_t m = _mm_movemask_ps(v.m[0]);\n"
"  for (uint32_t i = 1; i < vectorNum; ++i)\n"
"    m |= (_mm_movemask_ps(v.m[i]) << (4*i));\n"
"  return m;\n"
"}\n"
"\n"
"/* Vector instructions that use sse* */\n"
"#define VEC_OP(DST_TYPE, SRC_TYPE, NAME, INTRINSIC_NAME, FN, FN0, FN1)\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const SRC_TYPE &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\\\n"
"    dst.m[i] = FN(INTRINSIC_NAME(FN0(v0.m[i]), FN1(v1.m[i])));\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const scalar_dw &v1) {\\\n"
"  NAME(dst, v0, simd_dw<vectorNum>(v1));\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const scalar_dw &v0, const SRC_TYPE &v1) {\\\n"
"  NAME(dst, simd_dw<vectorNum>(v0), v1);\\\n"
"}\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, ADD_F, _mm_add_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, SUB_F, _mm_sub_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, MUL_F, _mm_mul_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, DIV_F, _mm_div_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, EQ_F, _mm_cmpeq_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, NE_F, _mm_cmpneq_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, LT_F, _mm_cmplt_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, LE_F, _mm_cmple_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, GT_F, _mm_cmpgt_ps, ID, ID, ID);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, GE_F, _mm_cmpge_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, ADD_S32, _mm_add_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, SUB_S32, _mm_sub_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, EQ_S32, _mm_cmpeq_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, LT_S32, _mm_cmplt_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_m<vectorNum>,  simd_dw<vectorNum>, GT_S32, _mm_cmpgt_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, OR_S32, _mm_or_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, XOR_S32, _mm_xor_ps, ID, ID, ID);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, AND_S32, _mm_and_ps, ID, ID, ID);\n"
"#undef VEC_OP\n"
"\n"
"/* Vector integer operations that we can get by switching argument order */\n"
"#define VEC_OP(DST_TYPE, SRC_TYPE, NAME, INTRINSIC_NAME, FN, FN0, FN1)\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const SRC_TYPE &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\\\n"
"    dst.m[i] = _mm_xor_ps(FN(INTRINSIC_NAME(FN1(v0.m[i]), FN0(v1.m[i]))), alltrue.v);\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const scalar_dw &v1) {\\\n"
"  NAME(dst, v0, simd_dw<vectorNum>(v1));\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const scalar_dw &v0, const SRC_TYPE &v1) {\\\n"
"  NAME(dst, simd_dw<vectorNum>(v0), v1);\\\n"
"}\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, GE_S32, _mm_cmplt_epi32, SI2PS, PS2SI, PS2SI);\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, LE_S32, _mm_cmpgt_epi32, SI2PS, PS2SI, PS2SI);\n"
"#undef VEC_OP\n"
"\n"
"/* Vector binary integer operations that require C */\n"
"#define VEC_OP(DST_TYPE, SRC_TYPE, NAME, OP, FIELD)\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const SRC_TYPE &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\\\n"
"    cast_dw c0(v0.m[i]), c1(v1.m[i]), d;\\\n"
"    for (uint32_t j = 0; j < 4; ++j)\\\n"
"      d.FIELD[j] = c0.FIELD[j] OP c1.FIELD[j];\\\n"
"    dst.m[i] = d.v;\\\n"
"  }\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const scalar_dw &v1) {\\\n"
"  NAME(dst, v0, simd_dw<vectorNum>(v1));\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const scalar_dw &v0, const SRC_TYPE &v1) {\\\n"
"  NAME(dst, simd_dw<vectorNum>(v0), v1);\\\n"
"}\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, MUL_S32, *, s);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, DIV_S32, /, s);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, REM_S32, %, s);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, MUL_U32, *, u);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, DIV_U32, /, u);\n"
"VEC_OP(simd_dw<vectorNum>, simd_dw<vectorNum>, REM_U32, %, u);\n"
"#undef VEC_OP\n"
"\n"
"/* Vector compare vectors that require C */\n"
"#define VEC_OP(DST_TYPE, SRC_TYPE, NAME, OP, FIELD)\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const SRC_TYPE &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\\\n"
"    cast_dw c0(v0.m[i]), c1(v1.m[i]), d;\\\n"
"    for (uint32_t j = 0; j < 4; ++j)\\\n"
"      d.u[j] = (c0.FIELD[j] OP c1.FIELD[j]) ? ~0u : 0u;\\\n"
"    dst.m[i] = d.v;\\\n"
"  }\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const SRC_TYPE &v0, const scalar_dw &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\\\n"
"    cast_dw c0(v0.m[i]), d;\\\n"
"    for (uint32_t j = 0; j < 4; ++j)\\\n"
"      d.u[j] = (c0.FIELD[j] OP v1.FIELD) ? ~0u : 0u;\\\n"
"    dst.m[i] = d.v;\\\n"
"  }\\\n"
"}\\\n"
"template <uint32_t vectorNum>\\\n"
"INLINE void NAME(DST_TYPE &dst, const scalar_dw &v0, const SRC_TYPE &v1) {\\\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\\\n"
"    cast_dw c1(v1.m[i]), d;\\\n"
"    for (uint32_t j = 0; j < 4; ++j)\\\n"
"      d.u[j] = (v0.FIELD OP c1.FIELD[j]) ? ~0u : 0u;\\\n"
"    dst.m[i] = d.v;\\\n"
"  }\\\n"
"}\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, LE_U32, <=, u);\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, LT_U32, <, u);\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, GE_U32, >=, u);\n"
"VEC_OP(simd_m<vectorNum>, simd_dw<vectorNum>, GT_U32, >, u);\n"
"#undef VEC_OP\n"
"\n"
"template <uint32_t vectorNum>\n"
"INLINE void NE_S32(simd_m<vectorNum> &dst,\n"
"                   const simd_dw<vectorNum> &v0,\n"
"                   const scalar_dw &v1)\n"
"{\n"
"  NE_S32(dst, v0, simd_dw<vectorNum>(v1));\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE void NE_S32(simd_m<vectorNum> &dst,\n"
"                   const scalar_dw &v0,\n"
"                   const simd_dw<vectorNum> &v1)\n"
"{\n"
"  NE_S32(dst, simd_dw<vectorNum>(v0), v1);\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE void NE_S32(simd_m<vectorNum> &dst,\n"
"                   const simd_dw<vectorNum> &v0,\n"
"                   const simd_dw<vectorNum> &v1)\n"
"{\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\n"
"    dst.m[i] = _mm_xor_ps(alltrue.v, SI2PS(_mm_cmpeq_epi32(PS2SI(v0.m[i]), PS2SI(v1.m[i]))));\n"
"}\n"
"\n"
"/* Load from contiguous double words */\n"
"template <uint32_t vectorNum>\n"
"INLINE void LOAD(simd_dw<vectorNum> &dst, const char *ptr) {\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\n"
"    dst.m[i] = _mm_loadu_ps((const float*) ptr + 4*i);\n"
"}\n"
"\n"
"/* Store to contiguous double words */\n"
"template <uint32_t vectorNum>\n"
"INLINE void STORE(const simd_dw<vectorNum> &src, char *ptr) {\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\n"
"    _mm_storeu_ps((float*) ptr + 4*i, src.m[i]);\n"
"}\n"
"\n"
"/* Load immediates */\n"
"template <uint32_t vectorNum>\n"
"INLINE void LOADI(simd_dw<vectorNum> &dst, float f) {\n"
"  for (uint32_t i = 0; i < vectorNum; ++i)\n"
"    dst.m[i] = _mm_load1_ps(&f);\n"
"}\n"
"\n"
"/* Scatter */\n"
"template <uint32_t vectorNum>\n"
"INLINE void SCATTER(const simd_dw<vectorNum> &value,\n"
"                    const simd_dw<vectorNum> &offset,\n"
"                    char *base_address) {\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\n"
"    const int v0 = _mm_extract_epi32(PS2SI(value.m[i]), 0);\n"
"    const int v1 = _mm_extract_epi32(PS2SI(value.m[i]), 1);\n"
"    const int v2 = _mm_extract_epi32(PS2SI(value.m[i]), 2);\n"
"    const int v3 = _mm_extract_epi32(PS2SI(value.m[i]), 3);\n"
"    const int o0 = _mm_extract_epi32(offset.m[i], 0);\n"
"    const int o1 = _mm_extract_epi32(offset.m[i], 1);\n"
"    const int o2 = _mm_extract_epi32(offset.m[i], 2);\n"
"    const int o3 = _mm_extract_epi32(offset.m[i], 3);\n"
"    *(int*)(base_address + o0) = v0;\n"
"    *(int*)(base_address + o1) = v1;\n"
"    *(int*)(base_address + o2) = v2;\n"
"    *(int*)(base_address + o3) = v3;\n"
"  }\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE void SCATTER(const scalar_dw &value,\n"
"                    const simd_dw<vectorNum> &offset,\n"
"                    char *base_address) {\n"
"  SCATTER(simd_dw<vectorNum>(value), offset, base_address);\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE void SCATTER(const simd_dw<vectorNum> &value,\n"
"                    const scalar_dw &offset,\n"
"                    char *base_address) {\n"
"  SCATTER(value, simd_dw<vectorNum>(offset), base_address);\n"
"}\n"
"#include <cstdio>\n"
"/* Gather */\n"
"template <uint32_t vectorNum>\n"
"INLINE void GATHER(simd_dw<vectorNum> &dst,\n"
"                   const simd_dw<vectorNum> &offset,\n"
"                   const char *base_address) {\n"
"  for (uint32_t i = 0; i < vectorNum; ++i) {\n"
"    const int o0 = _mm_extract_epi32(offset.m[i], 0);\n"
"    const int o1 = _mm_extract_epi32(offset.m[i], 1);\n"
"    const int o2 = _mm_extract_epi32(offset.m[i], 2);\n"
"    const int o3 = _mm_extract_epi32(offset.m[i], 3);\n"
"    const int v0 = *(const int*)(base_address + o0);\n"
"    const int v1 = *(const int*)(base_address + o1);\n"
"    const int v2 = *(const int*)(base_address + o2);\n"
"    const int v3 = *(const int*)(base_address + o3);\n"
"    dst.m[i] = SI2PS(_mm_insert_epi32(PS2SI(dst.m[i]), v0, 0));\n"
"    dst.m[i] = SI2PS(_mm_insert_epi32(PS2SI(dst.m[i]), v1, 1));\n"
"    dst.m[i] = SI2PS(_mm_insert_epi32(PS2SI(dst.m[i]), v2, 2));\n"
"    dst.m[i] = SI2PS(_mm_insert_epi32(PS2SI(dst.m[i]), v3, 3));\n"
"  }\n"
"}\n"
"template <uint32_t vectorNum>\n"
"INLINE void GATHER(simd_dw<vectorNum> &dst,\n"
"                   const scalar_dw &offset,\n"
"                   const char *base_address) {\n"
"  GATHER(dst, simd_dw<vectorNum>(offset), base_address);\n"
"}\n"
"\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"// Scalar instructions\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"INLINE uint32_t elemNum(const scalar_dw &x) { return 1; }\n"
"INLINE uint32_t elemNum(const scalar_m &x) { return 1; }\n"
"INLINE uint32_t mask(const scalar_m &v) { return v.u ? 1 : 0; }\n"
"INLINE void ADD_F(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.f = v0.f + v1.f; }\n"
"INLINE void SUB_F(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.f = v0.f - v1.f; }\n"
"INLINE void MUL_F(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.f = v0.f * v1.f; }\n"
"INLINE void DIV_F(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.f = v0.f / v1.f; }\n"
"INLINE void EQ_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f == v1.f ? ~0 : 0); }\n"
"INLINE void NE_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f != v1.f ? ~0 : 0); }\n"
"INLINE void LE_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f <= v1.f ? ~0 : 0); }\n"
"INLINE void LT_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f < v1.f ? ~0 : 0); }\n"
"INLINE void GE_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f >= v1.f ? ~0 : 0); }\n"
"INLINE void GT_F(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.f > v1.f ? ~0 : 0); }\n"
"INLINE void ADD_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s + v1.s; }\n"
"INLINE void SUB_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s - v1.s; }\n"
"INLINE void MUL_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s * v1.s; }\n"
"INLINE void DIV_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s / v1.s; }\n"
"INLINE void REM_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s % v1.s; }\n"
"INLINE void MUL_U32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.u = v0.u * v1.u; }\n"
"INLINE void DIV_U32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.u = v0.u / v1.u; }\n"
"INLINE void REM_U32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.u = v0.u % v1.u; }\n"
"INLINE void EQ_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s == v1.s ? ~0 : 0); }\n"
"INLINE void NE_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s != v1.s ? ~0 : 0); }\n"
"INLINE void LE_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s <= v1.s ? ~0 : 0); }\n"
"INLINE void LT_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s < v1.s ? ~0 : 0); }\n"
"INLINE void GE_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s >= v1.s ? ~0 : 0); }\n"
"INLINE void GT_S32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.s > v1.s ? ~0 : 0); }\n"
"INLINE void XOR_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s ^ v1.s; }\n"
"INLINE void OR_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s | v1.s; }\n"
"INLINE void AND_S32(scalar_dw &dst, scalar_dw v0, scalar_dw v1) { dst.s = v0.s & v1.s; }\n"
"INLINE void LE_U32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.u <= v1.u ? ~0 : 0); }\n"
"INLINE void LT_U32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.u < v1.u ? ~0 : 0); }\n"
"INLINE void GE_U32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.u >= v1.u ? ~0 : 0); }\n"
"INLINE void GT_U32(scalar_m &dst, scalar_dw v0, scalar_dw v1) { dst.u = (v0.u > v1.u ? ~0 : 0); }\n"
"INLINE void LOAD(scalar_dw &dst, const char *ptr) { dst.u = *(const uint32_t *) ptr; }\n"
"INLINE void STORE(scalar_dw src, char *ptr) { *(uint32_t *) ptr = src.u; }\n"
"INLINE void LOADI(scalar_dw &dst, uint32_t u) { dst.u = u; }\n"
"INLINE void SCATTER(scalar_dw value, scalar_dw offset, char *base) { *(uint32_t*)(base + offset.u) = value.u; }\n"
"INLINE void GATHER(scalar_dw &dst, scalar_dw offset, const char *base) { dst.u = *(const uint32_t*)(base + offset.u); }\n"
"\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"// Identical instructions are forwarded\n"
"//////////////////////////////////////////////////////////////////////////////\n"
"\n"
"#define ADD_U32 ADD_S32\n"
"#define SUB_U32 SUB_S32\n"
"#define XOR_U32 XOR_S32\n"
"#define OR_U32 OR_S32\n"
"#define AND_U32 AND_S32\n"
"#define EQ_U32 EQ_S32\n"
"#define NE_U32 NE_S32\n"
"\n"
"#undef PS2SI\n"
"#undef SI2PS\n"
"#undef ID\n"
"#undef INLINE\n"
"\n"
"#endif /* __GBE_SIM_VECTOR_H__ */\n"
"\n"
;
}



