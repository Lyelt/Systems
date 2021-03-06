#ifndef MOM_H
#define MOM_H

#include <vector>
#include <pthread.h>
#include "Kid.h"
#include "tools.hpp"
#include "Job.h"
#include "Shared.h"
#define MAXJOBS 10

class Mom {
	private:
		Shared* shared;
		const char* kidNames[4] = { "Avi", "Geoff", "Steph", "Chloe" };
		Kid kids[4];
		pthread_t kidThreads[4];
		
		vector<Job> doneJobs;
		time_t startTime;
		time_t currTime;
		
		// Mom's job functions
		void createJobs();
		Job createJob();
		void scanJobs();
		void analyzeJobs();
		
	public:
		Mom() {}
		~Mom() {}
		
		void run();
};

#endif