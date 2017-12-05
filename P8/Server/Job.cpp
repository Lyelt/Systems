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

Job::Job(int idVal, short speedVal, short funVal, short diffVal)
{
	id = idVal;
	speed = speedVal;
	fun = funVal;
	difficulty = diffVal;
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
	out << "\tTotal points: " << getPoints() << endl;
}

int Job::getPoints() const
{
	return this->speed * (this->fun + this->difficulty); 
}