#include "tools.hpp"
#include "Kid.h"
#include "Socket.hpp"
#include "ClientHandler.h"
#include "Job.h"
const char* QUIT = "quit";
const char* ACK = "ack";
const char* JOBS = "jobs";
const char* GOOD = "good";
// Initialize the kid's points and pick a random mood
Kid::Kid(ClientHandler* cli)
{
	client = cli;
	points = 0;
	mood = static_cast<Mood>(rand() % 5 + 1);
}

int Kid::handleMessage(char* message)
{
	//cout << "Client: Handling message " << message << endl;
	
	// Parse the message and tag we received
	char parsedMsg[BUFSIZ + 1];
	char parsedTag[BUFSIZ + 1];
	parseMessage(message, parsedTag, parsedMsg);
	
	//cout << "Client: Message parsed - tag (" << parsedTag << ") and message (" << parsedMsg << ")" << endl;
	
	// If this is a quit message, tell the client handler to quit
	if (strcmp(parsedTag, QUIT) == 0) {
		cout << "Client: Kid "<< id << " received quit message" << endl;
		return -1;
	}
	// If this is an ack, store our id and request the job table
	else if (strcmp(parsedTag, ACK) == 0) {
		id = atoi(parsedMsg);
		cout << "Client: Kid "<< id <<" received ack message" << endl;
		client->writeToServer("<req>");
	}
	// If mom sent us the job table, tell her what job we want
	else if (strcmp(parsedTag, JOBS) == 0) {
		cout << "Client: Kid "<< id <<" received job table" << endl;
		chooseJob(parsedMsg);
	}
	// If our choice is good, do the job
	else if (strcmp(parsedTag, GOOD) == 0) {
		doJob();
	}
	else {
		fatal("Kid %d received an unknown message", id);
	}

	return 1;
}

// Sleep for the required time and tell mom we're done
void Kid::doJob()
{
	sleep(chosenJob.speed);
	chosenJob.print(cout);
	
	const char* done = "<done>";
	client->writeToServer(done);
}

// Choose a job and return the time it will take to do it
void Kid::chooseJob(char* message)
{
	vector<Job> jobs = deserializeJobTable(message);

	short currMood = mood - 1; // for accessing the enum values 0-4
	switch (currMood) {
		case Lazy:
			chosenJob = getLazy(jobs);
			break;
		case Tired:
			chosenJob = getTired(jobs);
			break;
		case Prissy:
			chosenJob = getPrissy(jobs);
			break;
		case Greedy:
			chosenJob = getGreedy(jobs);
			break;
		case Impatient:
			chosenJob = jobs[9];
			break;
		default:
			fatal("Error choosing a job for mood %d.", currMood);
			break;
	}

	cout << "Child with mood " << currMood << " chose job with id " << chosenJob.id << endl;
	
	char buf[BUFSIZ + 1];
	sprintf(buf, "<choice>%d", chosenJob.id);
	client->writeToServer(buf);
}

// Job table is given to us as a string, we need to parse it for useful information
vector<Job> Kid::deserializeJobTable(char* message)
{
	vector<Job> jobs;
	// Get a stream representing the job table
	string msg(message);
	stringstream ss(msg);
	string line;
	
	while(getline(ss, line, ',')) {
		// Each , delimits one job
		string token;
		stringstream inner(line);
		vector<string> tokens;
		
		// Each : delimits the job traits
		while(getline(inner, token, ':')) {
			tokens.push_back(token);
		}
		
		// Tokens should contain the necessary info to construct a job
		int id = atoi(tokens[0].c_str());
		short speed = atoi(tokens[1].c_str());
		short fun = atoi(tokens[2].c_str());
		short diff = atoi(tokens[3].c_str());
		jobs.push_back(Job(id, speed, fun, diff));
	}
	
	return jobs;
}

Job Kid::getLazy(vector<Job> jobs)
{
	auto j = std::min_element(jobs.begin(), jobs.end(),
	[](const Job& x, const Job& y) {  return x.difficulty < y.difficulty; });
	
	return *j;
}

Job Kid::getTired(vector<Job> jobs)
{
	auto j = std::min_element(jobs.begin(), jobs.end(),
	[](const Job& x, const Job& y) {  return x.speed < y.speed; });
	
	return *j;
}

Job Kid::getPrissy(vector<Job> jobs)
{
	auto j = std::min_element(jobs.begin(), jobs.end(),
	[](const Job& x, const Job& y) {  return x.fun < y.fun; });
	
	return *j;
}

Job Kid::getGreedy(vector<Job> jobs)
{
	auto j = std::min_element(jobs.begin(), jobs.end(),
	[](const Job& x, const Job& y) {  return x.getPoints() < y.getPoints(); });
	
	return *j;
}