#include "tools.hpp"
#include "Job.h"

int Job::idCount = 0;

Job::Job(short speedVal, short funVal, short diffVal)
{
	speed = speedVal;
	fun = funVal;
	difficulty = diffVal;
	id = ++idCount;
}

void Job::setKidId(int id)
{
	kidId = id;
}

void Job::print(ostream& out)
{
	out << "Job " << id << " performed by kid " << kidId << ":" << endl;
	out << "\tDifficulty: " << difficulty << endl;
	out << "\tSpeed: " << speed << endl;
	out << "\tFun: " << fun << endl;
}