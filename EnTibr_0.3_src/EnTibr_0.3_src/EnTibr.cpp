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
 * Maybe todo:
 *
 * - multi hash support
 * - save and restore points
 *
 */

#include "EnTibr.h"

void printUsage()
{
	printf("EnTibr, the NTLM password brute forcer (version 0.3)\n");
	printf("By Daniel Niggebrugge: http://blog.distracted.nl/\n");
	printf("\n");
	printf("usage: EnTibr NTLM_hash -c charset\n");
	printf("\n");
	printf("NTLM_hash:      32 character hex encoded NTLM hash\n");
	printf("-c charset      all | loweralpha | upperalpha | mixalpha | numeric |\n");
	printf("                loweralpha-numeric | upperalpha-numeric | mixalpha-numeric\n");
	printf("\n");
	printf("[Optional:]\n");
	printf("-B              priority below normal\n");
	printf("-Q              quiet, less stats while running\n");
	printf("-t threads      amount of threads to use\n");
	printf("-l max length   default 15\n");
	printf("-m min length   default 1\n");
	printf("\n");
	printf("example: EnTibr 066ddfd4ef0e9cd7c256fe77191ef43c -c loweralpha -t 4\n");
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printUsage();
		return 0;
	}

	string sHash	= "";
	string sCharset = "";
	bool belowNormal = false;
	maxThreads = 1;
	minLength = 1;
	maxLength = 15;
	bool quiet = false;

	// check command line arguments
	int i;
	for (i = 1; i < argc; i++)
	{
		string cla = argv[i];
		if (cla == "-c") {
			i++;
			if (i < argc)
				sCharset = argv[i];
		}
		else if (cla == "-t") {
			i++;
			if (i < argc)
				maxThreads = atoi(argv[i]);
		}
		else if (cla == "-l") {
			i++;
			if (i < argc)
				maxLength = atoi(argv[i]);
		}
		else if (cla == "-m") {
			i++;
			if (i < argc)
				minLength = atoi(argv[i]);
		}
		else if (cla == "-B") {
			belowNormal = true;
		}
		else if (cla == "-Q") {
			quiet = true;
		}
		else {
			if (cla.substr(0,1) == "-")
				printf("Unknown argument: %s\n", cla.c_str());
			else
				sHash = cla;
		}
	}

	// verify variables
	if (maxThreads<1)
		maxThreads = 1;

	if (maxLength < minLength)
	{
		printf("Max length < min length!\n");
		return 0;
	}

	if (minLength < 1 || minLength > 15)
		minLength = 1;

	if (maxLength < 1 || maxLength > 15)
		maxLength = 15;

	// set variables for the specified charset
	if (setupCharset(sCharset) == false)
	{
		printf("Invalid charset.\n");
		return 0;
	}

	// lowercase and verify hash
	if (NormalizeHexString(sHash) == false)
	{
		printf("NTLM hash contains non-hex characters.\n");
		return 0;
	}

	// verify length and load hex hash into array
	unsigned char pCrackMe[16];
	int nHashLen;
	ParseHash(sHash, pCrackMe, nHashLen);
	if (nHashLen != 16)
	{
		printf("NTLM hash is %d bytes long, but should be 16 bytes.\n", nHashLen);
		return 0;
	}

	// so we can show the actual characters in the charset
	string charsetContent = "";
	int chr;
	for (chr = min_char; chr <= max_char; chr++)
	{
		if (chr == jumpFrom1_char) chr = jumpTo1_char;
		if (chr == jumpFrom2_char) chr = jumpTo2_char;
		charsetContent += chr;
	}

	// show the runtime variables
	printf("NTLM hash:      %s\n", sHash.c_str());
	printf("Threads:        %d\n", maxThreads);
	printf("Lower priority: %s\n", (belowNormal)?"yes":"no");
	printf("Quiet:          %s\n", (quiet)?"yes":"no");
	printf("Length:         %d-%d\n", minLength, maxLength);
	printf("Charset:        %s\n", sCharset.c_str());
	printf("%s\n", charsetContent.c_str());
	
	// so we can show stats on ctrl+c
	signal(SIGINT, wrapUp);

	memcpy(uDigest, pCrackMe, 16);

	// setup thread attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	#ifdef _WIN32
	if (belowNormal)
	{
		sched_param param;
		param.sched_priority = THREAD_PRIORITY_BELOW_NORMAL;
		pthread_attr_setschedparam (&attr, &param);
	}
	#endif

	// these will store the crack threads
	c_Threads.clear();
	pThreads.clear();

	// mutex for synchronizing threads
	pthread_mutex_init(&running_mutex, NULL);
	pthread_cond_init(&keeprunning_cv, NULL);

	bool hashFound = false;

	for(length = minLength; length <= maxLength && !hashFound; length++)
	{
		printf("\n");
		totalCombinations = 1;

		// calculate total possible combinations for progress check
		int i;
		for (i=0; i < length; i++)
		{
			totalCombinations *= cnt_char;
		}

		counter=0;

		// create threads
		int thread_ID;
		for (thread_ID = 0; thread_ID < maxThreads; thread_ID++)
		{
			if (length <= 2) {
				crackThreadSmall* c_Thread = new crackThreadSmall(thread_ID, uDigest, length, maxThreads, running_mutex, keeprunning_cv, min_char, max_char, jumpFrom1_char, jumpTo1_char, jumpFrom2_char, jumpTo2_char, cnt_char);
				if (c_Thread)
				{
					pthread_t pThread;
					int returnValue = pthread_create( &pThread, &attr, crackThreadSmall::crackThreadStaticEntryPointPthread, (void *) c_Thread);

					if( returnValue != 0 )
					{
						printf("pThread creation failed, returnValue: %d\n", returnValue);
					}
					else
					{
						pThreads.push_back(pThread);
						c_ThreadsSmall.push_back(c_Thread);
					}
				}
				else 
				{
					printf("c_Thread creation failed!\n");
				}
			}
			else {
				crackThread* c_Thread = new crackThread(thread_ID, uDigest, length, maxThreads, running_mutex, keeprunning_cv, min_char, max_char, jumpFrom1_char, jumpTo1_char, jumpFrom2_char, jumpTo2_char, cnt_char);
				if (c_Thread)
				{
					pthread_t pThread;
					int returnValue = pthread_create( &pThread, &attr, crackThread::crackThreadStaticEntryPointPthread, (void *) c_Thread);

					if( returnValue != 0 )
					{
						printf("pThread creation failed, returnValue: %d\n", returnValue);
					}
					else
					{
						pThreads.push_back(pThread);
						c_Threads.push_back(c_Thread);
					}
				}
				else 
				{
					printf("c_Thread creation failed!\n");
				}
			}

		}

		// start timer
		startClock = clock();
		
		// update stats every 'something close to 2' seconds
		// thread synchronization
		bool keepRunning = true;
		while (keepRunning)
		{
			timespec ts;
			time_t currentTime = time(NULL);
			ts.tv_sec = currentTime+2;
			
			pthread_mutex_lock(&running_mutex);
			int res = pthread_cond_timedwait(&keeprunning_cv, &running_mutex, &ts);
			
			pthread_mutex_unlock(&running_mutex);
			if (res == ETIMEDOUT) {
				if (!quiet)
					writeStats();
			}

			// check status of cracking threads
			int runningThreads = 0;
			if (length <= 2) {
				for (thread_ID = 0; thread_ID < c_ThreadsSmall.size(); thread_ID++)
				{
					crackThreadSmall* c_Thread = c_ThreadsSmall[thread_ID];
					if (c_Thread->isHashFound())
						hashFound = true;
					if (c_Thread->isRunning())
						runningThreads++;
				}
			}
			else {
				for (thread_ID = 0; thread_ID < c_Threads.size(); thread_ID++)
				{
					crackThread* c_Thread = c_Threads[thread_ID];
					if (c_Thread->isHashFound())
						hashFound = true;
					if (c_Thread->isRunning())
						runningThreads++;
				}
			}

			// stop threads if one of them found the hash
			if (hashFound)
			{
				if (length <= 2) {
					for (thread_ID = 0; thread_ID < c_ThreadsSmall.size(); thread_ID++)
					{
						crackThreadSmall* c_Thread = c_ThreadsSmall[thread_ID];
						c_Thread->stopRunning();
						//printf("\nstopping thread %d\n", thread_ID);
					}
				}
				else {
					for (thread_ID = 0; thread_ID < c_Threads.size(); thread_ID++)
					{
						crackThread* c_Thread = c_Threads[thread_ID];
						c_Thread->stopRunning();
						//printf("\nstopping thread %d\n", thread_ID);
					}
				}
				runningThreads = 0;
			}

			// kill this loop if all cracking threads stopped
			if (runningThreads == 0)
				keepRunning = false;
		}
		
		// wait untill all threads stopped (should already have stopped)
		for (thread_ID = 0; thread_ID < c_Threads.size(); thread_ID++)
		{
			pthread_t pThread = pThreads[thread_ID];
			int returnValue = pthread_join(pThread, NULL);
			if( returnValue != 0 )
			{
				printf("pThread join failed, returnValue: %d\n", returnValue);
			}
		}

		// show progress & speed, clear thread pool and start with next length
		writeStats();
		c_Threads.clear();
		pThreads.clear();
	}

	return 0;
}

bool NormalizeHexString(string& sHash)
{
	string sNormalizedHash = sHash;

	// Make lower
	int i;
	for (i = 0; i < sNormalizedHash.size(); i++)
	{
		if (sNormalizedHash[i] >= 'A' && sNormalizedHash[i] <= 'F')
			sNormalizedHash[i] = sNormalizedHash[i] - 'A' + 'a';
	}

	// Character check
	for (i = 0; i < sNormalizedHash.size(); i++)
	{
		if (   (sNormalizedHash[i] < 'a' || sNormalizedHash[i] > 'f')
			&& (sNormalizedHash[i] < '0' || sNormalizedHash[i] > '9'))
			return false;
	}

	sHash = sNormalizedHash;
	return true;
}

// parse hex encoded string into bytes
void ParseHash(string sHash, unsigned char* pHash, int& nHashLen)
{
	int i;
	for (i = 0; i < sHash.size() / 2; i++)
	{
		string sSub = sHash.substr(i * 2, 2);
		int nValue;
		sscanf(sSub.c_str(), "%02x", &nValue);
		pHash[i] = (unsigned char)nValue;
	}

	nHashLen = sHash.size() / 2;
}

bool setupCharset(string& sCharset)
{
	if(sCharset.compare("loweralpha") == 0)
	{
		min_char = 'a';
		max_char = 'z';
		cnt_char = 26;
		jumpFrom1_char = 0;
		jumpTo1_char = 0;
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("upperalpha") == 0)
	{
		min_char = 'A';
		max_char = 'Z';
		cnt_char = 26;
		jumpFrom1_char = 0;
		jumpTo1_char = 0;
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("numeric") == 0)
	{
		min_char = '0';
		max_char = '9';
		cnt_char = 10;
		jumpFrom1_char = 0;
		jumpTo1_char = 0;
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("mixalpha") == 0)
	{
		min_char = 'A';
		max_char = 'z';
		cnt_char = 52;
		jumpFrom1_char = 'Z'+1;
		jumpTo1_char = 'a';
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("loweralpha-numeric") == 0)
	{
		min_char = '0';
		max_char = 'z';
		cnt_char = 36;
		jumpFrom1_char = '9'+1;
		jumpTo1_char = 'a';
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("upperalpha-numeric") == 0)
	{
		min_char = '0';
		max_char = 'Z';
		cnt_char = 36;
		jumpFrom1_char = '9'+1;
		jumpTo1_char = 'A';
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else if(sCharset.compare("mixalpha-numeric") == 0)
	{
		min_char = '0';
		max_char = 'z';
		cnt_char = 62;
		jumpFrom1_char = '9'+1;
		jumpTo1_char = 'A';
		jumpFrom2_char = 'Z'+1;
		jumpTo2_char = 'a';
	}
	else if(sCharset.compare("all") == 0)
	{
		min_char = 0x20; //space
		max_char = 0x7E; //~
		cnt_char = 95;
		jumpFrom1_char = 0;
		jumpTo1_char = 0;
		jumpFrom2_char = 0;
		jumpTo2_char = 0;
	}
	else
	{
		return false;
	}
	return true;
}

// catch CTRL+C, write stats and exit
void wrapUp(int sig)
{
	printf("Aborting...\n");
	writeStats();
	exit(0);
}

void writeStats()
{
	int thread_ID;
	if (length <= 2) {
		for (thread_ID = 0; thread_ID < c_ThreadsSmall.size(); thread_ID++)
		{
			crackThreadSmall* c_Thread = c_ThreadsSmall[thread_ID];
			counter += c_Thread->getCounter();
		}
	}
	else {
		for (thread_ID = 0; thread_ID < c_Threads.size(); thread_ID++)
		{
			crackThread* c_Thread = c_Threads[thread_ID];
			counter += c_Thread->getCounter();
		}
	}

	clock_t currentClock = clock();
	if (currentClock - startClock > 0)
	{
		float fTime = 1.0f * (currentClock - startClock) / CLOCKS_PER_SEC;
		float speedOverall = counter / fTime / 1000000;
		float percentage = (100.0f * counter) / (1.0f * totalCombinations);
		if (percentage > 100.0)
			percentage = 100.0;
		printf("Length %d - %.0f%% in %.2f s (%.2f Mhashes/s)\r", length, percentage, fTime, speedOverall);
	}
	counter = 0;
}

string uint64tostr(uint64 n)
{
	char str[32];

	#ifdef _WIN32
		sprintf(str, "%I64u", n);
	#else
		sprintf(str, "%llu", n);
	#endif

	return str;
}