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

#ifdef _WIN32
	#include <windows.h>
#endif
#include <pthread.h>
#include <string>
#include "md4.h"
using namespace std;

#ifdef _WIN32
	#define uint64 unsigned __int64
#else
	#ifndef u_int64_t
		#define uint64 unsigned long long
	#else
		#define uint64 u_int64_t
	#endif
#endif

// Set plaintext to start with
// For NTLM we use Unicode characters
// Therefore set characters followed by 0x00
#define firstPlain(plain, sse_index, pos) \
	plain[pos] = min_char; \
	plain[pos+1] = 0x00; \
	nth = (sse_index % cnt_char); \
	while (nth > 0) \
	{ \
		plain[pos]++; \
		if (plain[pos] == jumpFrom1_char) plain[pos] = jumpTo1_char; \
		if (plain[pos] == jumpFrom2_char) plain[pos] = jumpTo2_char; \
		nth--; \
	} \
	sse_index /= cnt_char;

// Generate and set next plaintext
#define nextPlain(plain, update) \
	pos = 0; \
	while (plain[pos] >= max_char) pos+=2; \
	if (pos <= (length*2)) { \
		if (pos <= 3 || update) { /* do we need to re-reverse at this point? */ \
			plain[pos]++; \
			if (plain[pos] == jumpFrom1_char) plain[pos] = jumpTo1_char; \
			if (plain[pos] == jumpFrom2_char) plain[pos] = jumpTo2_char; \
			for (k = pos-2; k >= 0; k-=2) \
				plain[k] = min_char; /* reset to first char in map */ \
			if (update) \
				needUpdate = true; \
		} \
		else { \
			needUpdate = true; \
			thread_index--; \
		} \
	} \

// Generate and set next w0
#define nextW0(plain) \
	pos = 0; \
	if (plain[0] >= max_char) { \
		pos = 2; \
		if (plain[2] >= max_char) { \
			pos = 4; \
		} \
	} \
	if (pos < 4) { \
		plain[pos]++; \
		if (plain[pos] == jumpFrom1_char) plain[pos] = jumpTo1_char; \
		if (plain[pos] == jumpFrom2_char) plain[pos] = jumpTo2_char; \
		if (pos > 0) { \
			plain[0] = min_char; \
		} \
	} \
	

// Generate and set next w1's
#define nextW1(plain, update) \
	/*pos = 4; \
	if (plain[4] >= max_char) { \
		pos = 6; \
		if (plain[6] >= max_char) { \
			pos = 8; \
		} \
	} */\
	pos = 4; \
	while (plain[pos] >= max_char) pos+=2; \
	if (pos <= (length*2)) { \
		if (pos <= 7 || update) { /* do we need to re-reverse at this point? */ \
			plain[pos]++; \
			if (plain[pos] == jumpFrom1_char) plain[pos] = jumpTo1_char; \
			if (plain[pos] == jumpFrom2_char) plain[pos] = jumpTo2_char; \
			for (k = pos-2; k >= 0; k-=2) \
				plain[k] = min_char; /* reset to first char in map */ \
			if (pos > 7) \
				needUpdate2 = false; \
		}\
		else {\
			needUpdate2 = true;\
			thread_index-=4;\
		}\
	}\
	else { \
		endOfLength = true; \
	}

class crackThread
{
private:
	void run();
	string uint64tostr(uint64 n);

	int threadID;
	int min_char;
	int max_char;
	int jumpFrom1_char;
	int jumpTo1_char;
	int jumpFrom2_char;
	int jumpTo2_char;
	int cnt_char;
	int length;
	int maxThreads;
	UINT4 uDigest0;
	UINT4 uDigest1;
	UINT4 uDigest2;
	UINT4 uDigest3;
	bool keepRunning;
	bool hashFound;
	pthread_mutex_t running_mutex;
	pthread_cond_t keeprunning_cv;
	uint64 thread_startindex;
	uint64 thread_index;

public:
	crackThread(int id, UINT4* ud, int len, int max_Threads, pthread_mutex_t runningMutex, pthread_cond_t keeprunningCv, int min_chr, int max_chr, int jumpFrom1_chr, int jumpTo1_chr, int jumpFrom2_chr, int jumpTo2_chr, int cnt_chr);
	static void * crackThreadStaticEntryPointPthread(void * pThis);
	uint64 getCounter();
	bool isHashFound();
	bool isRunning();
	void stopRunning();
};

class crackThreadSmall
{
private:
	void run();
	string uint64tostr(uint64 n);

	int threadID;
	int min_char;
	int max_char;
	int jumpFrom1_char;
	int jumpTo1_char;
	int jumpFrom2_char;
	int jumpTo2_char;
	int cnt_char;
	int length;
	int maxThreads;
	UINT4 uDigest0;
	UINT4 uDigest1;
	UINT4 uDigest2;
	UINT4 uDigest3;
	bool keepRunning;
	bool hashFound;
	pthread_mutex_t running_mutex;
	pthread_cond_t keeprunning_cv;
	uint64 thread_startindex;
	uint64 thread_index;

public:
	crackThreadSmall(int id, UINT4* ud, int len, int max_Threads, pthread_mutex_t runningMutex, pthread_cond_t keeprunningCv, int min_chr, int max_chr, int jumpFrom1_chr, int jumpTo1_chr, int jumpFrom2_chr, int jumpTo2_chr, int cnt_chr);
	static void * crackThreadStaticEntryPointPthread(void * pThis);
	uint64 getCounter();
	bool isHashFound();
	bool isRunning();
	void stopRunning();
};
