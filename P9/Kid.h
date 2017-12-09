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

class Kid {
	private:
		Mood mood;
		
		Job getLazy();
		Job getTired();
		Job getPrissy();
		Job getGreedy();
		
	public:
		Kid();
		Kid(const char*, Shared*);
		~Kid() {}
		
		const char* name;
		vector<Job> doneJobs;
		Job chosenJob;
		Shared *shared;
		SignalHandler *siggy;
		
		void doJob();
		Job chooseJob();
};
#endif