/*
 * Code that runs per cracking thread
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

#include "crackThread.h"


crackThread::crackThread(int id, UINT4* ud, int len, int max_Threads, pthread_mutex_t runningMutex, pthread_cond_t keeprunningCv, int min_chr, int max_chr, int jumpFrom1_chr, int jumpTo1_chr, int jumpFrom2_chr, int jumpTo2_chr, int cnt_chr)
{
	threadID = id;

	min_char = min_chr;
	max_char = max_chr;
	cnt_char = cnt_chr;
	jumpFrom1_char = jumpFrom1_chr;
	jumpTo1_char = jumpTo1_chr;
	jumpFrom2_char = jumpFrom2_chr;
	jumpTo2_char = jumpTo2_chr;

	uDigest0 = ud[0];
	uDigest1 = ud[1];
	uDigest2 = ud[2];
	uDigest3 = ud[3];
	
	length = len;
	maxThreads = max_Threads;
	running_mutex = runningMutex;
	keeprunning_cv = keeprunningCv;
	
	thread_index = 0;
	thread_startindex = 0;
	keepRunning = true;
	hashFound = false;
}

// this is the entry point when a thread is created
void * crackThread::crackThreadStaticEntryPointPthread(void * pThis)
{
	crackThread* pTT = (crackThread*)pThis;

	pTT->run();
	pTT->stopRunning();

	pthread_mutex_lock(&pTT->running_mutex);
	pthread_cond_signal(&pTT->keeprunning_cv);
	pthread_mutex_unlock(&pTT->running_mutex);
	pthread_exit(NULL);

	return NULL;
}

void crackThread::run()
{
	__m128i mDigest0, mDigest1, mDigest2, mDigest3, mCa, mCb, mCc, mCd;
	mDigest0 = _mm_set1_epi32(uDigest0);
	mDigest1 = _mm_set1_epi32(uDigest1);
	mDigest2 = _mm_set1_epi32(uDigest2);
	mDigest3 = _mm_set1_epi32(uDigest3);

	mCa = _mm_set1_epi32(Ca);
	mCb = _mm_set1_epi32(Cb);
	mCc = _mm_set1_epi32(Cc);
	mCd = _mm_set1_epi32(Cd);


	// we are looking for this hash
	__m128i mA2, mB2, mC2, mD2, mCandD, mCorD;
	__m128i mA2_2, mB2_2, mC2_2, mD2_2, mCandD_2, mCorD_2;
	__m128i mA2_3, mB2_3, mC2_3, mD2_3, mCandD_3, mCorD_3;
	__m128i mA2_4, mB2_4, mC2_4, mD2_4, mCandD_4, mCorD_4;

	__m128i mA, mB, mC; //, mD;
	__m128i mA_2, mB_2, mC_2; //, mD_2;
	__m128i mA_3, mB_3, mC_3; //, mD_3;
	__m128i mA_4, mB_4, mC_4; //, mD_4;


	// calculate total possible combinations
	uint64 totalCombinations = 1;

	int i;
	for (i=0; i < length; i++)
	{
		totalCombinations *= cnt_char;
	}
	
	// calculate combinations for this thread
	uint64 threadCombinations = totalCombinations / maxThreads;
	
	thread_startindex = threadCombinations * threadID;
	uint64 thread_stopindex;

	if (threadID+1 < maxThreads)
		thread_stopindex = thread_startindex + threadCombinations;
	else
		thread_stopindex = totalCombinations;

	uint64 sse_index1 = thread_startindex;

	thread_index = thread_startindex;

	// plain
#ifdef _WIN32
	__declspec(align(16)) unsigned char pMuteMe1[32];
#else
	__attribute__((aligned(16))) unsigned char pMuteMe1[32];
#endif

	memset(pMuteMe1, 0, 32);

	// padding byte
	pMuteMe1[length*2] = 0x80;
	
	// set first plain
	int nth = 0;
	for (i = 0; i < length; i++)
	{
		firstPlain(pMuteMe1, sse_index1, i*2);
	}

	//reset first 2 chars to start character
	pMuteMe1[0] = min_char;
	pMuteMe1[2] = min_char;


	int pos = 0;
	int k;
	bool needUpdate = false;	
	bool needUpdate2 = false;
	bool endOfLength = false;

	__m128i w0, w1, w2, w3, w4, w5, w6, w7, w14, w1_2, w1_3, w1_4;
	__m128i a, b, c, d, tmp1, tmp2, AC, AC2;
	__m128i a2, b2, c2, d2, tmp1_2, tmp2_2;
	__m128i a3, b3, c3, d3, tmp1_3, tmp2_3;
	__m128i a4, b4, c4, d4, tmp1_4, tmp2_4;

	AC = _mm_set1_epi32(0x5a827999);
	AC2 = _mm_set1_epi32(0x6ed9eba1);


	w0 = _mm_set1_epi32(wIn[0]);
	w2 = _mm_set1_epi32(wIn[2]);
	w3 = _mm_set1_epi32(wIn[3]);
	w4 = _mm_set1_epi32(wIn[4]);
	w5 = _mm_set1_epi32(wIn[5]);
	w6 = _mm_set1_epi32(wIn[6]);
	w7 = _mm_set1_epi32(wIn[7]);

	w14 = _mm_set1_epi32(length*8*2);

	UINT4 w1Mute[16];
	w1Mute[0] = wIn[1];
	for (i = 1; i < 16; i++)
	{
		nextW1(pMuteMe1, false);
		w1Mute[i] = wIn[1];
	}

	w1 = _mm_set_epi32(w1Mute[0], w1Mute[1], w1Mute[2], w1Mute[3]);
	w1_2 = _mm_set_epi32(w1Mute[4], w1Mute[5], w1Mute[6], w1Mute[7]);
	w1_3 = _mm_set_epi32(w1Mute[8], w1Mute[9], w1Mute[10], w1Mute[11]);
	w1_4 = _mm_set_epi32(w1Mute[12], w1Mute[13], w1Mute[14], w1Mute[15]);

	updateReverse();
	
	uint64 char4Correction = cnt_char*cnt_char;

	// Setup plaintext cache for first 2 characters
	// Maybe use this for next groups of 2 characters as well (mind even/oneven lengths)
	//int indexCache2 = 0;
	int maxCache = cnt_char*cnt_char;
	if (maxCache > totalCombinations)
		maxCache = totalCombinations;
	unsigned int * cache = new unsigned int[maxCache];
	cache[0] = wIn[0];
	for (i = 1; i < maxCache; i++)
	{
		nextW0(pMuteMe1);
		cache[i] = wIn[0];
		//if (cache[i] == shortMute[1])
		//	indexCache2 = i;
	}
	int indexCache = 1;

	int counter = 0;

	// actual cracking loop, might do 1 or 2 rounds too much, little overhead to make sure no plains get skipped.
	while (keepRunning)
	{
		counter++;
		if (counter > 1000000)
		{
			// allow thread to be signalled by the main thread and stop faster
			pthread_mutex_lock(&running_mutex);
			pthread_mutex_unlock(&running_mutex);
			counter = 0;
		}

		thread_index+=16;

		//string check1 = "";
		//string check2 = "";
		//string check3 = "";
		//string check4 = "";
		//string check5 = "";
		//string check6 = "";
		//string check7 = "";
		//string check8 = "";
		//string check9 = "";
		//string check10 = "";
		//string check11 = "";
		//string check12 = "";
		//string check = "";
		//string first = "";
		//string last = "";
		////	if (i < 4) {
		////		first += char(pMuteMe1[i]);
		////	}
		//int i;
		//for (i = 0; i < length; i++) {
		//	if (i < 2) {
		//		first += char(pMuteMe1[i*2]);
		//	}
		//	else if (i >= 2 && i < 4) {
		//			/*check1 += char(w0.m128i_i8[12+i*2]);
		//			check2 += char(w0.m128i_i8[8+i*2]);
		//			check3 += char(w0.m128i_i8[4+i*2]);
		//			check4 += char(w0.m128i_i8[i*2]);
		//			check5 += char(w0_2.m128i_i8[12+i*2]);
		//			check6 += char(w0_2.m128i_i8[8+i*2]);
		//			check7 += char(w0_2.m128i_i8[4+i*2]);
		//			check8 += char(w0_2.m128i_i8[i*2]);*/
		//		check1 += char(w1.m128i_i8[12+i*2-4]);
		//		check2 += char(w1.m128i_i8[8+i*2-4]);
		//		check3 += char(w1.m128i_i8[4+i*2-4]);
		//		check4 += char(w1.m128i_i8[i*2-4]);
		//		check5 += char(w1_2.m128i_i8[12+i*2-4]);
		//		check6 += char(w1_2.m128i_i8[8+i*2-4]);
		//		check7 += char(w1_2.m128i_i8[4+i*2-4]);
		//		check8 += char(w1_2.m128i_i8[i*2-4]);
		//		check9 += char(w1_3.m128i_i8[12+i*2-4]);
		//		check10 += char(w1_3.m128i_i8[8+i*2-4]);
		//		check11 += char(w1_3.m128i_i8[4+i*2-4]);
		//		check12 += char(w1_3.m128i_i8[i*2-4]);
		//		//check += char(pMuteMe1[i]);
		//	}
		//	else {
		//		last += char(pMuteMe1[i*2]);
		//	}
		//}

		//debug =false;
		////if (check == "llo" && (check1 == "he" || check2 == "he" || check3 == "he" || check4 == "he" || check5 == "he" || check6 == "he" || check7 == "he" || check8 == "he"))
		////{
		////	debug=true;
		//if (first == "12" && check4 == "3") {
		//	debug = true;
		//	debug2 = true;
		//	//printf("%d: %s %s %s %s %s %s %s %s %s\n", threadID, check.c_str(), check1.c_str(), check2.c_str(), check3.c_str(), check4.c_str(), check5.c_str(), check6.c_str(), check7.c_str(), check8.c_str());
		//printf("%d-%s: %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", threadID, uint64tostr(thread_index).c_str(), first.c_str(), check1.c_str(), check2.c_str(), check3.c_str(), check4.c_str(), check5.c_str(), check6.c_str(), check7.c_str(), check8.c_str(), check9.c_str(), check10.c_str(), check11.c_str(), check12.c_str(), last.c_str());
		//}

		a = mCa;
		a2 = mCa;
		a3 = mCa;
		a4 = mCa;

		b = mCb;
		b2 = mCb;
		b3 = mCb;
		b4 = mCb;

		c = mCc;
		c2 = mCc;
		c3 = mCc;
		c4 = mCc;

		d = mCd;
		d2 = mCd;
		d3 = mCd;
		d4 = mCd;

		// shorter 1st step, but not faster
		//a = ROTATE_LEFT((_mm_set1_epi32(0xFFFFFFFF) + w0), S11);
		
		MD4_STEPS();

		//if (debug)
		//{
		//	for (i = 0; i<4; i++)
		//	{
		//		printf(" %u\t%u\t%u\t%u\n", a.m128i_i32[i], b.m128i_i32[i], c.m128i_i32[i], d.m128i_i32[i]);
		//		//printf("%u\t%u\t%u\t%u\n", a2.m128i_i32[i], b2.m128i_i32[i], c2.m128i_i32[i], d2.m128i_i32[i]);
		//	}
		//}

		mA   = mA2 - w0;
		mA_2 = mA2_2 - w0;
		mA_3 = mA2_3 - w0;
		mA_4 = mA2_4 - w0;
		
		//B2 = B - G(A2, C, D);
		tmp1 = mCandD | mA;
		tmp1_2 = mCandD_2 | mA_2;
		tmp1_3 = mCandD_3 | mA_3;
		tmp1_4 = mCandD_4 | mA_4;

		mB   = mB2 - (tmp1 & mCorD);
		mB_2 = mB2_2 - (tmp1_2 & mCorD_2);
		mB_3 = mB2_3 - (tmp1_3 & mCorD_3);
		mB_4 = mB2_4 - (tmp1_4 & mCorD_4);

		// further interlacing hits a limit on my system, probably the code is getting to large.
		// with 3x interlacing this could be fully 'unrolled', but that's slower in the end without intel compiler.
		/*tmp1 = tmp1 & mCorD;
		tmp1_2 = tmp1_2 & mCorD_2;
		tmp1_3 = tmp1_3 & mCorD_3;
		tmp1_4 = tmp1_4 & mCorD_4;

		mB   = mB2 - tmp1;
		mB_2 = mB2_2 - tmp1_2;
		mB_3 = mB2_3 - tmp1_3;
		mB_4 = mB2_4 - tmp1_4;*/

		/*mB   = mB2 - ((mCandD | mA) & mCorD);
		mB_2 = mB2_2 - ((mCandD_2 | mA_2) & mCorD_2);
		mB_3 = mB2_3 - ((mCandD_3 | mA_3) & mCorD_3);
		mB_4 = mB2_4 - ((mCandD_4 | mA_4) & mCorD_4);*/

		//#define G(x, y, z)   ((((x&y)) | (z)) & ((x|y)))

		/*tmp1 = mA & mB;
		tmp2 = mA | mB;
		tmp1_2 = mA_2 & mB_2;
		tmp2_2 = mA_2 | mB_2;
		tmp1_3 = mA_3 & mB_3;
		tmp2_3 = mA_3 | mB_3;
		tmp1_4 = mA_4 & mB_4;
		tmp2_4 = mA_4 | mB_4;*/

		/*tmp1 = tmp1 | mD2;
		tmp1_2 = tmp1_2 | mD2_2;
		tmp1_3 = tmp1_3 | mD2_3;
		tmp1_4 = tmp1_4 | mD2_4;*/

		

		/*mC = mC2 - (tmp1 & tmp2);
		mC_2 = mC2_2 - (tmp1_2 & tmp2_2);
		mC_3 = mC2_3 - (tmp1_3 & tmp2_3);
		mC_4 = mC2_4 - (tmp1_4 & tmp2_4);*/

		/*mC = mC2 - ((tmp1 | mD2) & tmp2);
		mC_2 = mC2_2 - ((tmp1_2 | mD2_2) & tmp2_2);
		mC_3 = mC2_3 - ((tmp1_3 | mD2_3) & tmp2_3);
		mC_4 = mC2_4 - ((tmp1_4 | mD2_4) & tmp2_4);*/

		mC   = mC2 - G(mA, mB, mD2);
		mC_2 = mC2_2 - G(mA_2, mB_2, mD2_2);
		mC_3 = mC2_3 - G(mA_3, mB_3, mD2_3);
		mC_4 = mC2_4 - G(mA_4, mB_4, mD2_4);


		// compare to reversed value of A
		int result_mask = _mm_movemask_epi8(_mm_cmpeq_epi32(mC, c)) + _mm_movemask_epi8(_mm_cmpeq_epi32(mC_2, c2)) + _mm_movemask_epi8(_mm_cmpeq_epi32(mC_3, c3))  + _mm_movemask_epi8(_mm_cmpeq_epi32(mC_4, c4));
		if (result_mask > 0)
		{
			//printf("partial\n");

			MD4STEP_ROUND2      (b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, a, a2, a3, a4, w14, w14, w14, w14, S24);\
			MD4STEP_ROUND2      (a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, d, d2, d3, d4, w3, w3,  w3, w3, S21);\
			MD4STEP_ROUND2      (d, d2, d3, d4, a, a2, a3, a4, b, b2, b3, b4, c, c2, c3, c4, w7, w7,  w7, w7, S22);\

			// compare to reversed values of A, B, C, D
			// result_mask contains 0x0000 on no match.
			// result_mask contains 0xF000 on match with first hash, 0x0F00 on match with second, etc.
			//unsigned char pFirst[16];
			unsigned char pSecond[16];
			int	result_mask_full = _mm_movemask_epi8(_mm_cmpeq_epi32(mA, a) \
					& _mm_cmpeq_epi32(mB, b) \
					& _mm_cmpeq_epi32(mC, c) \
					& _mm_cmpeq_epi32(mD2, d) \
					);
			if (result_mask_full > 0) {
				_mm_storeu_si128 ((__m128i *)pSecond, w1);
			}
			else {
				result_mask_full = _mm_movemask_epi8(_mm_cmpeq_epi32(mA_2, a2) \
					& _mm_cmpeq_epi32(mB_2, b2) \
					& _mm_cmpeq_epi32(mC_2, c2) \
					& _mm_cmpeq_epi32(mD2_2, d2) \
					);
				if (result_mask_full > 0) {
					_mm_storeu_si128 ((__m128i *)pSecond, w1_2);
				}
				else {
					result_mask_full = _mm_movemask_epi8(_mm_cmpeq_epi32(mA_3, a3) \
						& _mm_cmpeq_epi32(mB_3, b3) \
						& _mm_cmpeq_epi32(mC_3, c3) \
						& _mm_cmpeq_epi32(mD2_3, d3) \
						);
					if (result_mask_full > 0) {
						_mm_storeu_si128 ((__m128i *)pSecond, w1_3);
					}
					else {
						result_mask_full = _mm_movemask_epi8(_mm_cmpeq_epi32(mA_4, a4) \
							& _mm_cmpeq_epi32(mB_4, b4) \
							& _mm_cmpeq_epi32(mC_4, c4) \
							& _mm_cmpeq_epi32(mD2_4, d4) \
							);
						if (result_mask_full > 0) {
							_mm_storeu_si128 ((__m128i *)pSecond, w1_4);
						}
					}
				}
			}

			if (result_mask_full > 0)
			{
				string found = "";

				for (i = 0; i < length; i++) {
					if (i < 2) {
						found += char(pMuteMe1[i*2]);
					}
					else if (i >= 2 && i < 4) {
						if (result_mask_full == 0xF000)
							found += char(pSecond[12+(i*2)-4]);
						else if (result_mask_full == 0x0F00)
							found += char(pSecond[8+(i*2)-4]);
						else if (result_mask_full == 0x00F0)
							found += char(pSecond[4+(i*2)-4]);
						else if (result_mask_full == 0x000F)
							found += char(pSecond[(i*2)-4]);
					}
					else {
						found += char(pMuteMe1[i*2]);
					}
				}

				printf("\n\n>>> Found: %s <<<\n\n", found.c_str());

				// signal main thread and stop this thread
				pthread_mutex_lock(&running_mutex);
				pthread_cond_signal(&keeprunning_cv);
				pthread_mutex_unlock(&running_mutex);
				hashFound = true;
				return;
			}
		}

		if (indexCache < maxCache)
		{
			wIn[0] = cache[indexCache];
			indexCache++;
		}
		else
		{
			wIn[0] = cache[0];
			indexCache = 1;
			
			if (!endOfLength)
			{
				if (thread_index < thread_stopindex+16)
				{
					nextW1(pMuteMe1, true);
					w1Mute[0] = wIn[1];

					for (i = 1; i < 16; i++)
					{
						if (!needUpdate2) {
							nextW1(pMuteMe1, false);
							w1Mute[i] = wIn[1];
						}
						else
						{
							thread_stopindex += (16-i) * char4Correction;
							break;
						}
					}

					w1 = _mm_set_epi32(w1Mute[0], w1Mute[1], w1Mute[2], w1Mute[3]);
					w1_2 = _mm_set_epi32(w1Mute[4], w1Mute[5], w1Mute[6], w1Mute[7]);
					w1_3 = _mm_set_epi32(w1Mute[8], w1Mute[9], w1Mute[10], w1Mute[11]);
					w1_4 = _mm_set_epi32(w1Mute[12], w1Mute[13], w1Mute[14], w1Mute[15]);

					w2 = _mm_set1_epi32(wIn[2]);
					w3 = _mm_set1_epi32(wIn[3]);
					w4 = _mm_set1_epi32(wIn[4]);
					w5 = _mm_set1_epi32(wIn[5]);
					w6 = _mm_set1_epi32(wIn[6]);
					w7 = _mm_set1_epi32(wIn[7]);

					updateReverse();
				}
				else {
					keepRunning = false;
				}
			}
			else {
				keepRunning = false;
			}
		}
		w0 = _mm_set1_epi32(wIn[0]);
	
	}
}

// for main thread to gather statistics & progress
uint64 crackThread::getCounter()
{
	return thread_index - thread_startindex;
}

bool crackThread::isHashFound()
{
	return hashFound;
}

bool crackThread::isRunning()
{
	return keepRunning;
}

// for main thread to be able and stop threads
void crackThread::stopRunning()
{
	//printf("Thread %d stopping\n", threadID);
	keepRunning = false;
}

string crackThread::uint64tostr(uint64 n)
{
	char str[32];

	#ifdef _WIN32
		sprintf(str, "%I64u", n);
	#else
		sprintf(str, "%llu", n);
	#endif

	return str;
}