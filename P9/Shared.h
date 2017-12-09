#ifndef SHARED_H
#define SHARED_H
#include "tools.hpp"
#include "Job.h"
#include "Kid.h"

typedef struct sigaction sigAct;

// Global variables for handling signals and spawning kid threads
extern void* doKid(void*);
extern bool startFlag;
extern bool quitFlag;

class Shared 
{
	public:
		Shared();
		~Shared() {}
		Job jobTable[10];
		pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif