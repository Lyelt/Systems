#ifndef KID_H
#define KID_H
#include <vector>
#include "Shared.h"
#include "tools.hpp"
#include "Job.h"
#include "SignalHandler.h"

enum Mood { Lazy, Tired, Prissy, Greedy, Impatient };
class Shared;
class SignalHandler;

class Kid 
{
	private:
		Mood mood;
		
		int getLazy();
		int getTired();
		int getPrissy();
		int getGreedy();
		
	public:
		Kid();
		Kid(const char*, Shared*);
		~Kid() {}
		
		const char* name;
		vector<Job> doneJobs;

		int chosenJobIndex;	  // currently chosen job index
		Shared *shared;
		SignalHandler *siggy; // signal handler for start and quit
		
		// Kid's job functions
		void printJobs();
		void doJob();
		int chooseJob();
};
#endif