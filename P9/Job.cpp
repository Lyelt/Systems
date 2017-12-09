#include "tools.hpp"
#include "Job.h"

int Job::idCount = 0;
Job::Job() = default;

Job::Job(short speedVal, short funVal, short diffVal, const char* kid, bool isDone)
{
	speed = speedVal;
	fun = funVal;
	difficulty = diffVal;
	kidName = kid;
	done = isDone;
	id = idCount++;
}

void Job::print(ostream& out)
{
	out << "Job " << id << " performed by kid " << kidName << ":" << endl;
	out << "\tDifficulty: " << difficulty << endl;
	out << "\tSpeed: " << speed << endl;
	out << "\tFun: " << fun << endl;
	out << "\tTotal points: " << getPoints() << endl;
}

int Job::getPoints() const
{
	return this->speed * (this->fun + this->difficulty); 
}