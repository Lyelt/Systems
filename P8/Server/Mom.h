#ifndef MOM_H
#define MOM_H
#include <vector>
#include "ServerHandler.h"
#include "tools.hpp"
#include "Job.h"
#define MAXJOBS 10
class ServerHandler;
class Mom {
	private:
		static ServerHandler* server;
		static vector<Job> jobTable;
		static vector<Job> doneJobs;
		static time_t startTime;

		static void createJobs();
		
	public:
		Mom() {}
		~Mom() {}
		
		static void init(int);
		static void handleMessage(int, char*);
};

#endif