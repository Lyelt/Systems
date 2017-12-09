#include "Mom.h"

void Mom::run() 
{
	shared = new Shared();
	createJobs();
	
	// ---------------------
	// Initialize the four kids and their threads
	for (int i = 0; i < 4; ++i) {
		Kid kid( kidNames[i], shared );
		kids[i] = kid;
		
		pthread_create( &kidThreads[i], NULL, &doKid, (void*)&kids[i] );
		cout << "Kid " << kidNames[i] << " created" << endl;
	}
	sleep(2);
	// Record the time and send a start signal
	time(&startTime);
	pthread_kill( kidThreads[0], SIGUSR1 ); // start signal
	
	// --------------------
	// Scan the job table every second
	for (;;) {
		sleep(1);
		time(&currTime);
		time_t diff = currTime - startTime;
		
		if (diff >= 21) {
			break;
		}
		else {
			scanJobs();
		}
	}
	
	// Send quit signal when time is over
	pthread_kill( kidThreads[0], SIGQUIT );
	// Scan jobs one last time
	scanJobs(); 
	
	// Wait for threads to join
	for (int i = 0; i < 4; ++i) {
		pthread_join( kidThreads[i], NULL );
	}
	
	analyzeJobs();
	delete shared;
}

void Mom::createJobs()
{
	pthread_mutex_lock(&(shared->job_mutex));
	for(int i = 0; i < MAXJOBS; ++i) {
		shared->jobTable[i] = createJob();
	}
	pthread_mutex_unlock(&(shared->job_mutex));
}

Job Mom::createJob()
{
	short speed = rand() % 5 + 1;
	short fun = rand() % 5 + 1;
	short diff = rand() % 5 + 1;
	bool done = false;
	const char* kidName = "";
	return Job(speed, fun, diff, kidName, done);
}

void Mom::scanJobs()
{
	for (int i = 0; i < MAXJOBS; ++i) {
		if (shared->jobTable[i].done) {
			pthread_mutex_lock(&(shared->job_mutex));
				doneJobs.push_back(shared->jobTable[i]);
				shared->jobTable[i] = createJob();
			pthread_mutex_unlock(&(shared->job_mutex));
		}
	}
}

void Mom::analyzeJobs()
{
	map<const char*, int> pointsEarned;
	cout << "-------------------------------------------------" << endl;
	cout << "All kids done working." << endl;
	for (auto& job : doneJobs) {
		pointsEarned[job.kidName] += job.getPoints();
		job.print(cout);
	}
	cout << "---------------------------------------" << endl;
	const char* winnerName;
	int max = 0;
	for (auto& kid : pointsEarned) {
		cout << "Kid " << kid.first << " earned " << kid.second << " points." << endl;
		if (kid.second > max) {
			max = kid.second;
			winnerName = kid.first;
		}
	}
	cout << winnerName << " is the winner with " << max << " points!" << endl;
}