#include "tools.hpp"
#include "Kid.h"
#include "Job.h"

Kid::Kid() = default;

// Initialize the kid object with shared, mood, and signal handler
Kid::Kid(const char* myName, Shared* share)
{
	name = myName;
	shared = share;
	mood = static_cast<Mood>(rand() % 5 + 1);
	siggy = new SignalHandler();
}

// Print total points earned
void Kid::printJobs()
{
	int totalPoints = 0;
	for (auto& job : doneJobs) {
		totalPoints += job.getPoints();
	}
	
	pthread_mutex_lock(&(shared->write_mutex));
		cout << name << " earned total: " << totalPoints << " points" << endl;
	pthread_mutex_unlock(&(shared->write_mutex));
}

// Choose a job and return the time it will take to do it
int Kid::chooseJob()
{
	short currMood = mood - 1; // for accessing the enum values 0-4
	
	switch (currMood) {
	case Lazy:
		chosenJobIndex = getLazy();
		break;
	case Tired:
		chosenJobIndex = getTired();
		break;
	case Prissy:
		chosenJobIndex = getPrissy();
		break;
	case Greedy:
		chosenJobIndex = getGreedy();
		break;
	case Impatient:
		chosenJobIndex = 9;
		break;
	default:
		fatal("Error choosing a job for mood %d.", currMood);
		break;
	}
	
	// No kid has claimed the job yet
	if (shared->jobTable[chosenJobIndex].kidName[0] == '\0') { 
		shared->jobTable[chosenJobIndex].kidName = name;	
		return chosenJobIndex;
	}
	
	return -1;
}

// ----------------------------------------------------------------
// Pick jobs based on mood
int Kid::getLazy()
{
	int easiest = 0;
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].difficulty < shared->jobTable[easiest].difficulty)
			easiest = i;
	}
	
	return easiest;
}

int Kid::getTired()
{
	int shortest = 0;
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].speed < shared->jobTable[shortest].speed)
			shortest = i;
	}
	
	return shortest;
}

int Kid::getPrissy()
{
	int mostFun = 0;
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].fun < shared->jobTable[mostFun].fun)
			mostFun = i;
	}
	
	return mostFun;
}

int Kid::getGreedy()
{
	int mostPoints = 0;
	for (int i = 0; i < 10; ++i) {
		if (shared->jobTable[i].getPoints() < shared->jobTable[mostPoints].getPoints())
			mostPoints = i;
	}
	
	return mostPoints;
}