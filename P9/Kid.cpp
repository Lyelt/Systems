#include "tools.hpp"
#include "Kid.h"
#include "Job.h"

Kid::Kid() = default;
// Initialize the kid's points and pick a random mood
Kid::Kid(const char* myName, Shared* share)
{
	name = myName;
	shared = share;
	mood = static_cast<Mood>(rand() % 5 + 1);
	siggy = new SignalHandler();
}

// Choose a job and return the time it will take to do it
Job Kid::chooseJob()
{
	short currMood = mood - 1; // for accessing the enum values 0-4
	
	for (;;) {
		switch (currMood) {
		case Lazy:
			chosenJob = getLazy();
			break;
		case Tired:
			chosenJob = getTired();
			break;
		case Prissy:
			chosenJob = getPrissy();
			break;
		case Greedy:
			chosenJob = getGreedy();
			break;
		case Impatient:
			chosenJob = shared->jobTable[9];
			break;
		default:
			fatal("Error choosing a job for mood %d.", currMood);
			break;
		}

		if (chosenJob.kidName[0] == '\0') { // No kid has claimed the job yet
			chosenJob.kidName = name;
			return chosenJob;
		}
	}
}

Job Kid::getLazy()
{
	Job easiest = shared->jobTable[0];
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].difficulty < easiest.speed)
			easiest = shared->jobTable[i];
	}
	
	return easiest;
}

Job Kid::getTired()
{
	Job shortest = shared->jobTable[0];
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].speed < shortest.speed)
			shortest = shared->jobTable[i];
	}
	
	return shortest;
}

Job Kid::getPrissy()
{
	Job mostFun = shared->jobTable[0];
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].fun < mostFun.fun)
			mostFun = shared->jobTable[i];
	}
	
	return mostFun;
}

Job Kid::getGreedy()
{
	Job mostPoints = shared->jobTable[0];
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].getPoints() < mostPoints.getPoints())
			mostPoints = shared->jobTable[i];
	}
	
	return mostPoints;
}