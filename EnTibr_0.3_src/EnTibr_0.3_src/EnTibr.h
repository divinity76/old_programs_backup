/*
 * EnTibr, the NTLM password brute forcer
 *
 * Author: Daniël Niggebrugge - http://blog.distracted.nl/
 * License: Use and share as you wish at your own risk, please keep this header ;)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, [...] etc :p
 *
 *
 * Todo:
 *
 * - multi hash support
 * - save and restore points
 *
 */

#include <vector>
#include "signal.h"

#include "crackThread.h"

// methods
bool NormalizeHexString(string& sHash);
void ParseHash(string sHash, unsigned char* pHash, int& nHashLen);
bool setupCharset(string& sCharset);
void wrapUp(int sig);
void writeStats();
string uint64tostr(uint64 n);

// variables
clock_t startClock;
int maxThreads;
uint64 counter;

UINT4 uDigest[4];

vector<crackThread *> c_Threads;
vector<crackThreadSmall *> c_ThreadsSmall;
vector<pthread_t> pThreads;

pthread_mutex_t running_mutex;
pthread_cond_t keeprunning_cv;

int length;
int minLength;
int maxLength;
uint64 totalCombinations;

int min_char;
int max_char;
int jumpFrom1_char;
int jumpTo1_char;
int jumpFrom2_char;
int jumpTo2_char;
int cnt_char;
