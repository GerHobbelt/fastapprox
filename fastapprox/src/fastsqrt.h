
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_SQRT_H_
#define __FAST_SQRT_H_

#include <stdint.h>

static inline float q_sqrt(float number) {
  uint32_t i;
  float x2, y;
  const float threehalfs = 1.5f;

  x2 = number * 0.5f;
  y = number;
  i = *(uint32_t *)&y;        // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);  // what the fuck?
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y));  // 1st iteration
  // 	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
  // 	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return number * y;
}

static inline float x_sqrt(float x) {
  union {
    uint32_t i;
    float f;
  } any = {.f = x};

  // any.i = (any.i >> 1) + 0x1FC00000;
  any.i = (any.i >> 1) + 0x1fbc704f;
  // any.i = (any.i >> 1) + 0x1fbd4089;

  float y = any.f;

  y = (y + x / y) * 0.5;
  y = (y + x / y) * 0.5;

  // y = y + x / y;
  // y = 0.25 * y + x / y;

  return y;
}

static inline float h_sqrt(float x)
{
	__asm__ ("sqrtss %1, %0" : "=x"(x) : "x"(x));
  return x;

#if 0
  const int32_t coeffs[] = {
    -583,
    8118237,
    6407,
    0,
    -824,
    14859524,
    3101374,
    0,
  };

  union {
    uint32_t i;
    float f;
  } any = {.f = x};

  uint32_t y0 = (any.i >> 1) + 0x1FC00000;
  uint32_t m = (y0 & 0x7FFFFF) >> 11;
  const int32_t *a = coeffs + ((m >> 9) & 0x4);
  uint32_t fpart = ((a[0] * m + a[1]) >> 12) * m - a[2];

  any.i = (y0 & 0x7F800000) + fpart;

  float y = any.f;
  // y = (y + x / y) * 0.5;
  // y = (y + x / y) * 0.5;
  return y;
#endif
}

#endif  // __FAST_SQRT_H_
