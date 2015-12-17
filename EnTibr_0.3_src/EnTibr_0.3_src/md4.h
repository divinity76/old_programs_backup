/*
 * Implementation of the MD4 message-digest algorithm, optimized for passwords with length < 32 (or NTLM < 16)
 * (see http://www.ietf.org/rfc/rfc1320.txt)
 *
 * Author: Daniël Niggebrugge
 * License: Use and share as you wish at your own risk, please keep this header ;)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, [...] etc :p
 *
 */

#include <emmintrin.h>

typedef unsigned int UINT4;

#ifdef WIN32
//these seem to be defined already in linux
inline __m128i operator +(const __m128i &a, const __m128i &b){
	return _mm_add_epi32(a,b);
}
inline __m128i operator -(const __m128i &a, const __m128i &b){
	return _mm_sub_epi32(a,b);
}
inline __m128i operator ^(const __m128i &a, const __m128i &b){
	return _mm_xor_si128(a,b);
}
inline __m128i operator |(const __m128i &a, const __m128i &b){
	return _mm_or_si128(a,b);
}
inline __m128i operator &(const __m128i &a, const __m128i &b){
	return _mm_and_si128(a,b);
}
#endif

// Define rotations
#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

// Define initial values
#define Ca 				0x67452301
#define Cb 				0xefcdab89
#define Cc 				0x98badcfe
#define Cd 				0x10325476

// Define functions for use in the 3 rounds of MD4
//#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
//#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define F(x, y, z)   (((x) & (y)) | (_mm_andnot_si128((x),(z))))
#define G(x, y, z)   ((((x) & (y)) | (z)) & ((x) | (y)))
#define H(x, y, z)   ((x) ^ (y) ^ (z))

#define ROTATE_LEFT(a,n) _mm_or_si128(_mm_slli_epi32(a, n), _mm_srli_epi32(a, (32-n)))
#define ROTATE_RIGHT(a,n) _mm_or_si128(_mm_srli_epi32(a, n), _mm_slli_epi32(a, (32-n)))
// Rotate right is only used in reversing code, which isn't SSE2 code
#define ROTATE_RIGHT_SMALL(x, n)	(((x) >> (n)) | ((x) << (32-(n))))

// A regular MD4STEP for ROUND3, but then reversed
// + becomes -, ROTATE_LEFT becomes ROTATE_RIGHT
#define REVERSE_MD4STEP_SMALL(a, b, c, d, x, s) \
	a= ROTATE_RIGHT_SMALL(a, s) - 0x6ed9eba1 - x - H(b, c, d);

// The reversing steps, start with input hash (uDigest)
// Then subtract initial values (normally added after the last round)
// Do reversing steps and then update reversed values (mDigest)
#define updateReverseSmall() { \
	A = uDigest0;\
	B = uDigest1;\
	C = uDigest2;\
	D = uDigest3;\
	\
	A -= Ca;\
	B -= Cb;\
	C -= Cc;\
	D -= Cd;\
	\
	REVERSE_MD4STEP_SMALL (B, C, D, A, 0,      S34);\
	REVERSE_MD4STEP_SMALL (C, D, A, B, wIn[7], S33);\
	REVERSE_MD4STEP_SMALL (D, A, B, C, 0,      S32);\
	REVERSE_MD4STEP_SMALL (A, B, C, D, wIn[3], S31);\
	\
	REVERSE_MD4STEP_SMALL (B, C, D, A, 0,      S34);\
	REVERSE_MD4STEP_SMALL (C, D, A, B, wIn[5], S33);\
	REVERSE_MD4STEP_SMALL (D, A, B, C, 0,      S32);\
	REVERSE_MD4STEP_SMALL (A, B, C, D, wIn[1], S31);\
	\
	REVERSE_MD4STEP_SMALL (B, C, D, A, (length<<4), S34);\
	REVERSE_MD4STEP_SMALL (C, D, A, B, wIn[6], S33);\
	REVERSE_MD4STEP_SMALL (D, A, B, C, 0,      S32);\
	REVERSE_MD4STEP_SMALL (A, B, C, D, wIn[2], S31);\
	\
	REVERSE_MD4STEP_SMALL (B, C, D, A, 0,      S34);\
	REVERSE_MD4STEP_SMALL (C, D, A, B, wIn[4], S33);\
	REVERSE_MD4STEP_SMALL (D, A, B, C, 0,      S32);\
	REVERSE_MD4STEP_SMALL (A, B, C, D, 0,      S31);\
	\
	B  = ROTATE_RIGHT_SMALL((B), S24) - 0x5a827999; \
	C2 = ROTATE_RIGHT_SMALL((C), S23) - 0x5a827999; \
	mA2    = _mm_set1_epi32(A); \
	mB2    = _mm_set1_epi32(B); \
	mC2    = _mm_set1_epi32(C2); \
	mD2    = _mm_set1_epi32(D); \
	mCandD = _mm_set1_epi32(C & D); \
	mCorD  = _mm_set1_epi32(C | D); \
}

// Interlacing 4x SSE2
#define MD4_STEPS_SMALL() { \
	MD4STEP_ROUND1      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w0, w0_2, w0_3, w0_4,  S11);\
	MD4STEP_ROUND1      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w1, w1, w1, w1,  S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w2, w2, w2, w2,  S13);\
	MD4STEP_ROUND1      (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, w3, w3, w3, w3,  S14);\
	MD4STEP_ROUND1      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w4, w4, w4, w4,  S11);\
	MD4STEP_ROUND1      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w5, w5, w5, w5,  S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w6, w6, w6, w6,  S13);\
	MD4STEP_ROUND1      (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, w7, w7, w7, w7,  S14);\
	MD4STEP_ROUND1_NULL (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, S11);\
	MD4STEP_ROUND1_NULL (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, S12);\
	MD4STEP_ROUND1_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S13);\
	MD4STEP_ROUND1_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S14);\
	MD4STEP_ROUND1_NULL (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, S11);\
	MD4STEP_ROUND1_NULL (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w14, w14, w14, w14,  S13);\
	MD4STEP_ROUND1_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S14);\
	\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w0, w0_2, w0_3, w0_4,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w4, w4, w4, w4,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	MD4STEP_ROUND2_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S24);\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w1, w1, w1, w1,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w5, w5, w5, w5,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	MD4STEP_ROUND2_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S24);\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w2, w2, w2, w2,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w6, w6, w6, w6,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	/*MD4STEP_ROUND2      (b, c, d, a, w14, S24);\
	MD4STEP_ROUND2      (a, b, c, d,  w3, S21);*/\
}

#define REVERSE_MD4STEP(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, x, x2, x3, x4, s) { \
	tmp1 = _mm_srli_epi32(a, s);\
	tmp1_2 = _mm_srli_epi32(a2, s);\
	tmp1_3 = _mm_srli_epi32(a3, s);\
	tmp1_4 = _mm_srli_epi32(a4, s);\
	a = _mm_slli_epi32(a, (32-s));\
	a2 = _mm_slli_epi32(a2, (32-s));\
	a3 = _mm_slli_epi32(a3, (32-s));\
	a4 = _mm_slli_epi32(a4, (32-s));\
	a = tmp1 | a;\
	a2 = tmp1_2 | a2;\
	a3 = tmp1_3 | a3;\
	a4 = tmp1_4 | a4;\
	\
	a = a - x;\
	a2 = a2 - x2;\
	a3 = a3 - x3;\
	a4 = a4 - x4;\
	\
	a = a - AC2;\
	a2 = a2 - AC2;\
	a3 = a3 - AC2;\
	a4 = a4 - AC2;\
	\
	tmp1 = b ^ c;\
	tmp1_2 = b2 ^ c2;\
	tmp1_3 = b3 ^ c3;\
	tmp1_4 = b4 ^ c4;\
	tmp1 = tmp1 ^ d;\
	tmp1_2 = tmp1_2 ^ d2;\
	tmp1_3 = tmp1_3 ^ d3;\
	tmp1_4 = tmp1_4 ^ d4;\
	\
	a = a - tmp1;\
	a2 = a2 - tmp1_2;\
	a3 = a3 - tmp1_3;\
	a4 = a4 - tmp1_4;\
}

#define REVERSE_MD4STEP_NULL(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, s) { \
	tmp1 = _mm_srli_epi32(a, s);\
	tmp1_2 = _mm_srli_epi32(a2, s);\
	tmp1_3 = _mm_srli_epi32(a3, s);\
	tmp1_4 = _mm_srli_epi32(a4, s);\
	a = _mm_slli_epi32(a, (32-s));\
	a2 = _mm_slli_epi32(a2, (32-s));\
	a3 = _mm_slli_epi32(a3, (32-s));\
	a4 = _mm_slli_epi32(a4, (32-s));\
	a = tmp1 | a;\
	a2 = tmp1_2 | a2;\
	a3 = tmp1_3 | a3;\
	a4 = tmp1_4 | a4;\
	\
	a = a - AC2;\
	a2 = a2 - AC2;\
	a3 = a3 - AC2;\
	a4 = a4 - AC2;\
	\
	tmp1 = b ^ c;\
	tmp1_2 = b2 ^ c2;\
	tmp1_3 = b3 ^ c3;\
	tmp1_4 = b4 ^ c4;\
	tmp1 = tmp1 ^ d;\
	tmp1_2 = tmp1_2 ^ d2;\
	tmp1_3 = tmp1_3 ^ d3;\
	tmp1_4 = tmp1_4 ^ d4;\
	\
	a = a - tmp1;\
	a2 = a2 - tmp1_2;\
	a3 = a3 - tmp1_3;\
	a4 = a4 - tmp1_4;\
}

#define wIn		((UINT4 *)pMuteMe1)


// The reversing steps, start with input hash (uDigest)
// Then subtract initial values (normally added after the last round)
// Do reversing steps and then update reversed values (mDigest)
#define updateReverse() { \
	mA2   = mDigest0 - mCa;\
	mA2_2 = mDigest0 - mCa;\
	mA2_3 = mDigest0 - mCa;\
	mA2_4 = mDigest0 - mCa;\
	mB2   = mDigest1 - mCb;\
	mB2_2 = mDigest1 - mCb;\
	mB2_3 = mDigest1 - mCb;\
	mB2_4 = mDigest1 - mCb;\
	mC2   = mDigest2 - mCc;\
	mC2_2 = mDigest2 - mCc;\
	mC2_3 = mDigest2 - mCc;\
	mC2_4 = mDigest2 - mCc;\
	mD2   = mDigest3 - mCd;\
	mD2_2 = mDigest3 - mCd;\
	mD2_3 = mDigest3 - mCd;\
	mD2_4 = mDigest3 - mCd;\
	\
	REVERSE_MD4STEP_NULL(mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4,			     S34);\
	REVERSE_MD4STEP     (mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, w7, w7, w7, w7, S33);\
	REVERSE_MD4STEP_NULL(mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, 			     S32);\
	REVERSE_MD4STEP     (mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, w3, w3, w3, w3, S31);\
	\
	REVERSE_MD4STEP_NULL(mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4,			     S34);\
	REVERSE_MD4STEP     (mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, w5, w5, w5, w5, S33);\
	REVERSE_MD4STEP_NULL(mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, 			     S32);\
	REVERSE_MD4STEP     (mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, w1, w1_2, w1_3, w1_4, S31);\
	\
	REVERSE_MD4STEP     (mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, w14, w14, w14, w14, S34);\
	REVERSE_MD4STEP     (mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, w6, w6, w6, w6, S33);\
	REVERSE_MD4STEP_NULL(mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, 			     S32);\
	REVERSE_MD4STEP     (mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, w2, w2, w2, w2, S31);\
	\
	REVERSE_MD4STEP_NULL(mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4,			     S34);\
	REVERSE_MD4STEP     (mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, w4, w4, w4, w4, S33);\
	REVERSE_MD4STEP_NULL(mD2, mD2_2, mD2_3, mD2_4, mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, 			     S32);\
	REVERSE_MD4STEP_NULL(mA2, mA2_2, mA2_3, mA2_4, mB2, mB2_2, mB2_3, mB2_4, mC2, mC2_2, mC2_3, mC2_4, mD2, mD2_2, mD2_3, mD2_4,                 S31);\
	\
	mCandD = mC2 & mD2; \
	mCandD_2 = mC2_2 & mD2_2; \
	mCandD_3 = mC2_3 & mD2_3; \
	mCandD_4 = mC2_4 & mD2_4; \
	\
	mCorD = mC2 | mD2; \
	mCorD_2 = mC2_2 | mD2_2; \
	mCorD_3 = mC2_3 | mD2_3; \
	mCorD_4 = mC2_4 | mD2_4; \
	\
	tmp1 = _mm_srli_epi32(mB2, S24);\
	tmp1_2 = _mm_srli_epi32(mB2_2, S24);\
	tmp1_3 = _mm_srli_epi32(mB2_3, S24);\
	tmp1_4 = _mm_srli_epi32(mB2_4, S24);\
	mB2 = _mm_slli_epi32(mB2, (32-S24));\
	mB2_2 = _mm_slli_epi32(mB2_2, (32-S24));\
	mB2_3 = _mm_slli_epi32(mB2_3, (32-S24));\
	mB2_4 = _mm_slli_epi32(mB2_4, (32-S24));\
	mB2 = tmp1 | mB2;\
	mB2_2 = tmp1_2 | mB2_2;\
	mB2_3 = tmp1_3 | mB2_3;\
	mB2_4 = tmp1_4 | mB2_4;\
	\
	mB2 = mB2 - AC;\
	mB2_2 = mB2_2 - AC;\
	mB2_3 = mB2_3 - AC;\
	mB2_4 = mB2_4 - AC;\
	\
	tmp1 = _mm_srli_epi32(mC2, S23);\
	tmp1_2 = _mm_srli_epi32(mC2_2, S23);\
	tmp1_3 = _mm_srli_epi32(mC2_3, S23);\
	tmp1_4 = _mm_srli_epi32(mC2_4, S23);\
	mC2 = _mm_slli_epi32(mC2, (32-S23));\
	mC2_2 = _mm_slli_epi32(mC2_2, (32-S23));\
	mC2_3 = _mm_slli_epi32(mC2_3, (32-S23));\
	mC2_4 = _mm_slli_epi32(mC2_4, (32-S23));\
	mC2 = tmp1 | mC2;\
	mC2_2 = tmp1_2 | mC2_2;\
	mC2_3 = tmp1_3 | mC2_3;\
	mC2_4 = tmp1_4 | mC2_4;\
	\
	mC2 = mC2 - AC;\
	mC2_2 = mC2_2 - AC;\
	mC2_3 = mC2_3 - AC;\
	mC2_4 = mC2_4 - AC;\
}

// Interlacing 4x SSE2
#define MD4_STEPS() { \
	MD4STEP_ROUND1      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w0, w0, w0, w0,  S11);\
	MD4STEP_ROUND1      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w1, w1_2, w1_3, w1_4,  S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w2, w2, w2, w2,  S13);\
	MD4STEP_ROUND1      (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, w3, w3, w3, w3,  S14);\
	MD4STEP_ROUND1      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w4, w4, w4, w4,  S11);\
	MD4STEP_ROUND1      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w5, w5, w5, w5,  S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w6, w6, w6, w6,  S13);\
	MD4STEP_ROUND1      (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, w7, w7, w7, w7,  S14);\
	MD4STEP_ROUND1_NULL (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, S11);\
	MD4STEP_ROUND1_NULL (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, S12);\
	MD4STEP_ROUND1_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S13);\
	MD4STEP_ROUND1_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S14);\
	MD4STEP_ROUND1_NULL (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, S11);\
	MD4STEP_ROUND1_NULL (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, S12);\
	MD4STEP_ROUND1      (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, w14, w14, w14, w14,  S13);\
	MD4STEP_ROUND1_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S14);\
	\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w0, w0, w0, w0,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w4, w4, w4, w4,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	MD4STEP_ROUND2_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S24);\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w1, w1_2, w1_3, w1_4,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w5, w5, w5, w5,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	MD4STEP_ROUND2_NULL (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, S24);\
	MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w2, w2, w2, w2,  S21);\
	MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w6, w6, w6, w6,  S22);\
	MD4STEP_ROUND2_NULL (c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, S23);\
	/*MD4STEP_ROUND2      (b, c, d, a, w14, S24);\
	MD4STEP_ROUND2      (a, b, c, d,  w3, S21);*/\
}

// 4x SSE2
#define MD4STEP_ROUND1(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, x, x2, x3, x4, s) {	\
	tmp1 = (b) & (c);\
	tmp1_2 = (b2) & (c2);\
	tmp1_3 = (b3) & (c3);\
	tmp1_4 = (b4) & (c4);\
	tmp2 = _mm_andnot_si128((b),(d));\
	tmp2_2 = _mm_andnot_si128((b2),(d2));\
	tmp2_3 = _mm_andnot_si128((b3),(d3));\
	tmp2_4 = _mm_andnot_si128((b4),(d4));\
	tmp1 = tmp1 | tmp2;\
	tmp1_2 = tmp1_2 | tmp2_2;\
	tmp1_3 = tmp1_3 | tmp2_3;\
	tmp1_4 = tmp1_4 | tmp2_4;\
    (a) = (a) + tmp1;		\
	(a2) = (a2) + tmp1_2;		\
	(a3) = (a3) + tmp1_3;		\
	(a4) = (a4) + tmp1_4;		\
	(a) = (a) + (x);				    \
	(a2) = (a2) + (x2);				    \
	(a3) = (a3) + (x3);				    \
	(a4) = (a4) + (x4);				    \
	tmp1 = _mm_slli_epi32((a), (s));\
	tmp1_2 = _mm_slli_epi32((a2), (s));\
	tmp1_3 = _mm_slli_epi32((a3), (s));\
	tmp1_4 = _mm_slli_epi32((a4), (s));\
	(a) = _mm_srli_epi32((a), (32-s));\
	(a2) = _mm_srli_epi32((a2), (32-s));\
	(a3) = _mm_srli_epi32((a3), (32-s));\
	(a4) = _mm_srli_epi32((a4), (32-s));\
	(a) = tmp1 | (a);\
	(a2) = tmp1_2 | (a2);\
	(a3) = tmp1_3 | (a3);\
	(a4) = tmp1_4 | (a4);\
}

#define MD4STEP_ROUND1_NULL(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, s) {	\
	tmp1 = (b) & (c);\
	tmp1_2 = (b2) & (c2);\
	tmp1_3 = (b3) & (c3);\
	tmp1_4 = (b4) & (c4);\
	tmp2 = _mm_andnot_si128((b),(d));\
	tmp2_2 = _mm_andnot_si128((b2),(d2));\
	tmp2_3 = _mm_andnot_si128((b3),(d3));\
	tmp2_4 = _mm_andnot_si128((b4),(d4));\
	tmp1 = tmp1 | tmp2;\
	tmp1_2 = tmp1_2 | tmp2_2;\
	tmp1_3 = tmp1_3 | tmp2_3;\
	tmp1_4 = tmp1_4 | tmp2_4;\
    (a) = (a) + tmp1;		\
	(a2) = (a2) + tmp1_2;		\
	(a3) = (a3) + tmp1_3;		\
	(a4) = (a4) + tmp1_4;		\
	tmp1 = _mm_slli_epi32((a), (s));\
	tmp1_2 = _mm_slli_epi32((a2), (s));\
	tmp1_3 = _mm_slli_epi32((a3), (s));\
	tmp1_4 = _mm_slli_epi32((a4), (s));\
	(a) = _mm_srli_epi32((a), (32-s));\
	(a2) = _mm_srli_epi32((a2), (32-s));\
	(a3) = _mm_srli_epi32((a3), (32-s));\
	(a4) = _mm_srli_epi32((a4), (32-s));\
	(a) = tmp1 | (a);\
	(a2) = tmp1_2 | (a2);\
	(a3) = tmp1_3 | (a3);\
	(a4) = tmp1_4 | (a4);\
}

#define MD4STEP_ROUND2(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, x, x2, x3, x4, s) {	\
	tmp1 = (b) & (c);\
	tmp1_2 = (b2) & (c2);\
	tmp1_3 = (b3) & (c3);\
	tmp1_4 = (b4) & (c4);\
	tmp1 = tmp1 | (d);\
	tmp1_2 = tmp1_2 | (d2);\
	tmp1_3 = tmp1_3 | (d3);\
	tmp1_4 = tmp1_4 | (d4);\
	tmp2 = (b) | (c);\
	tmp2_2 = (b2) | (c2);\
	tmp2_3 = (b3) | (c3);\
	tmp2_4 = (b4) | (c4);\
	tmp1 = tmp1 & tmp2;\
	tmp1_2 = tmp1_2 & tmp2_2;\
	tmp1_3 = tmp1_3 & tmp2_3;\
	tmp1_4 = tmp1_4 & tmp2_4;\
    (a) = (a) + tmp1;		\
	(a2) = (a2) + tmp1_2;		\
	(a3) = (a3) + tmp1_3;		\
	(a4) = (a4) + tmp1_4;		\
	(a) = (a) + (x);				    \
	(a2) = (a2) + (x2);				    \
	(a3) = (a3) + (x3);				    \
	(a4) = (a4) + (x4);				    \
	(a) = (a) + (AC);				    \
	(a2) = (a2) + (AC);				    \
	(a3) = (a3) + (AC);				    \
	(a4) = (a4) + (AC);				    \
	tmp1 = _mm_slli_epi32((a), (s));\
	tmp1_2 = _mm_slli_epi32((a2), (s));\
	tmp1_3 = _mm_slli_epi32((a3), (s));\
	tmp1_4 = _mm_slli_epi32((a4), (s));\
	(a) = _mm_srli_epi32((a), (32-s));\
	(a2) = _mm_srli_epi32((a2), (32-s));\
	(a3) = _mm_srli_epi32((a3), (32-s));\
	(a4) = _mm_srli_epi32((a4), (32-s));\
	(a) = tmp1 | (a);\
	(a2) = tmp1_2 | (a2);\
	(a3) = tmp1_3 | (a3);\
	(a4) = tmp1_4 | (a4);\
}

#define MD4STEP_ROUND2_NULL(a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, s) {	\
	tmp1 = (b) & (c);\
	tmp1_2 = (b2) & (c2);\
	tmp1_3 = (b3) & (c3);\
	tmp1_4 = (b4) & (c4);\
	tmp1 = tmp1 | (d);\
	tmp1_2 = tmp1_2 | (d2);\
	tmp1_3 = tmp1_3 | (d3);\
	tmp1_4 = tmp1_4 | (d4);\
	tmp2 = (b) | (c);\
	tmp2_2 = (b2) | (c2);\
	tmp2_3 = (b3) | (c3);\
	tmp2_4 = (b4) | (c4);\
	tmp1 = tmp1 & tmp2;\
	tmp1_2 = tmp1_2 & tmp2_2;\
	tmp1_3 = tmp1_3 & tmp2_3;\
	tmp1_4 = tmp1_4 & tmp2_4;\
    (a) = (a) + tmp1;		\
	(a2) = (a2) + tmp1_2;		\
	(a3) = (a3) + tmp1_3;		\
	(a4) = (a4) + tmp1_4;		\
	(a) = (a) + (AC);				    \
	(a2) = (a2) + (AC);				    \
	(a3) = (a3) + (AC);				    \
	(a4) = (a4) + (AC);				    \
	tmp1 = _mm_slli_epi32((a), (s));\
	tmp1_2 = _mm_slli_epi32((a2), (s));\
	tmp1_3 = _mm_slli_epi32((a3), (s));\
	tmp1_4 = _mm_slli_epi32((a4), (s));\
	(a) = _mm_srli_epi32((a), (32-s));\
	(a2) = _mm_srli_epi32((a2), (32-s));\
	(a3) = _mm_srli_epi32((a3), (32-s));\
	(a4) = _mm_srli_epi32((a4), (32-s));\
	(a) = tmp1 | (a);\
	(a2) = tmp1_2 | (a2);\
	(a3) = tmp1_3 | (a3);\
	(a4) = tmp1_4 | (a4);\
}
