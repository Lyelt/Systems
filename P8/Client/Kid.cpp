#include "tools.hpp"
#include "Kid.h"
#include "Socket.hpp"
#include "ClientHandler.h"
#include "Job.h"
const char* QUIT = "quit";
const char* ACK = "ack";
const char* JOB = "job";
// Initialize the kid's points and pick a random mood
Kid::Kid(ClientHandler* cli)
{
	client = cli;
	points = 0;
	mood = static_cast<Mood>(rand() % 6);
}

int Kid::handleMessage(char* message)
{
	cout << "Handling message " << message << endl;
	

	// If this is a quit message, tell the client handler to quit
	if (messageTagEquals(message, QUIT)) {
		return -1;
	}
	else if (messageTagEquals(message, ACK)) {
		// id = atoi(message + 5);
		return 1;
	}
		
	// Otherwise, the message contains a job table and we need to choose one
	int wait = chooseJob(message);
	doJob(wait);
	return 1;
}

// Sleep for the required number of time periods
void Kid::doJob(int wait)
{
	sleep(wait);
	
	char buf[BUFSIZ + 1];
	sprintf(buf, "<done>");
	client->writeToServer(buf);
}

// Choose a job and return the time it will take to do it
int Kid::chooseJob(char* message)
{
	short speed = 0;
	short fun = 0;
	short diff = 0;
	int jobId = 0;
	
	vector<Job> jobs = deserializeJobTable(message);
	
	switch (mood) {
		case Lazy:
			break;
		case Tired:
			break;
		case Prissy:
			break;
		case Greedy:
			break;
		case Impatient:
			break;
		default:
			break;
	}
	
	char buf[BUFSIZ + 1];
	sprintf(buf, "<choice>%d", jobId);
	client->writeToServer(buf);
	
	return speed;
}

vector<Job> Kid::deserializeJobTable(char* message)
{
	vector<Job> jobs;
	
	return jobs;
}