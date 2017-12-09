#include "Shared.h"

bool startFlag = false;
bool quitFlag = false;

Shared::Shared() = default;

void* doKid(void* kidp)
{
	Kid* kid = static_cast<Kid*>(kidp);
	cout << kid->name << " beginning work..." << endl;
	kid->siggy->setSignals(kid);
	while (!startFlag); // wait for startflag to be set
	cout << "Start flag is true: " << kid->name << endl;
	// ---------------------------------------------
	// Choose and do the jobs
	for (;;) {
		Job chosen = kid->chooseJob();
		sleep(chosen.speed);
		
		if (quitFlag) break; // the quit signal has been sent
		
		chosen.done = true;
		kid->doneJobs.push_back(chosen);
	}
	
	// ---------------------------------------------
	// Get ready to quit
	cout << kid->name << " received quit signal" << endl;
	
	int totalPoints = 0;
	for (auto& job : kid->doneJobs) {
		job.print(cout);
		totalPoints += job.getPoints();
	}
	cout << "------------------------" << endl;
	cout << "Earned total: " << totalPoints << " points" << endl;
	
	delete kid->siggy;
}
