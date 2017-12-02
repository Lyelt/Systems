#ifndef MOM_H
#define MOM_H
#include <vector>
#include "ServerHandler.h"
#include "tools.hpp"
#include "Job.h"

class Mom {
	private:
		ServerHandler* server;
		vector<Job> jobTable;
		vector<Job> doneJobs;
		time_t startTime;
		
		void handleMessage(char*, int, time_t);
		int doService(toPoll*);
		int getKid(toPoll*);
		void createJobs();
		
	public:
		Mom(int);
		~Mom() {}
};

#endif