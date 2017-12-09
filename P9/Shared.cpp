#include "Shared.h"

bool startFlag = false;
bool quitFlag = false;

Shared::Shared() = default;

void* doKid(void* kidp)
{
	Kid* kid = static_cast<Kid*>(kidp);
	kid->siggy->setSignals(kid);
	
	while (!startFlag); // busy wait for startflag to be set
	// Useful shorthand for shared objects
	Shared* myShared = kid->shared;
	pthread_mutex_t* job_lock = &(myShared->job_mutex);
	// ---------------------------------------------
	// Choose and do the jobs
	for (;;) {
		// Choosing a job needs to be atomic so that we can assure that no other
		// kid is trying to choose the same job.
		// If they are, we just have to try again and wait for mom to put up
		// a new job. If not, we can claim it for ourselves.
		pthread_mutex_lock(job_lock);
			int chosen = kid->chooseJob();
		pthread_mutex_unlock(job_lock);
		
		if (quitFlag) 	  break;	// time to quit
		if (chosen == -1) continue; // job was taken, try again
		
		Job chosenJob = myShared->jobTable[chosen];
		// Simulate doing the job
		sleep(chosenJob.speed);
		if (quitFlag) break;

		// Lock the job table so we can set done = true now that we're done
		// in case mom is trying to scan the job table at the same time
		pthread_mutex_lock(job_lock);
			myShared->jobTable[chosen].done = true;
		pthread_mutex_unlock(job_lock);

		kid->doneJobs.push_back(chosenJob);
		chosenJob.print(cout);
	}
	// ---------------------------------------------
	// Get ready to quit
	// Using a write mutex for some intelligible output
	pthread_mutex_lock(&(kid->shared->write_mutex));
		cout << kid->name << " received quit signal" << endl;
	pthread_mutex_unlock(&(kid->shared->write_mutex));
	
	kid->printJobs();
	
	delete kid->siggy;
}
