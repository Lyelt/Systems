#include "tools.hpp"
#include "Kid.h"
#include "Socket.hpp"
#include "ClientHandler.h"
#include "Job.h"
const char* QUIT = "quit";
const char* ACK = "ack";
// Initialize the kid's points and pick a random mood
Kid::Kid(ClientHandler* cli)
{
	client = cli;
	points = 0;
	mood = static_cast<Mood>(rand() % 6);
}

int Kid::handleMessage(char* message)
{
	cout << "Client: Handling message " << message << endl;
	char parsedMsg[BUFSIZ + 1];
	char parsedTag[BUFSIZ + 1];
	parseMessage(message, parsedTag, parsedMsg);
	cout << "Client: Message parsed - tag (" << parsedTag << ") and message (" << parsedMsg << ")" << endl;
	// If this is a quit message, tell the client handler to quit
	if (strcmp(parsedTag, QUIT) == 0) {
		cout << "Client: Kid "<<id << " received quit message" << endl;
		return -1;
	}
	else if (strcmp(parsedTag, ACK) == 0) {
		id = atoi(parsedMsg); // message should contain this kid's id
		cout << "Client: Kid "<< id<<" received ack message" << endl;
		client->writeToServer("<req>");
		return 1;
	}
	cout << "Client: Kid "<< id<<" received job table" << endl;	
	// Otherwise, the message contains a job table and we need to choose one
	int wait = chooseJob(parsedMsg);
	doJob(wait);
	return 1;
}

// Sleep for the required number of time periods
void Kid::doJob(int wait)
{
	sleep(wait);
	
	const char* done = "<done>";
	client->writeToServer(done);
}

// Choose a job and return the time it will take to do it
int Kid::chooseJob(char* message)
{
	vector<Job> jobs = deserializeJobTable(message);
	Job chosen;
	cout << "Client: Job table deserialized" << endl;
	switch (mood - 1) {
		case Lazy:
			chosen = getLazy(jobs);
			break;
		case Tired:
			chosen = getTired(jobs);
			break;
		case Prissy:
			chosen = getPrissy(jobs);
			break;
		case Greedy:
			chosen = getGreedy(jobs);
			break;
		case Impatient:
			chosen = jobs[9];
			break;
		default:
			fatal("Error choosing a job.");
			break;
	}

	cout << "Child with mood " << mood << " chose job with id " << chosen.id << endl;
	int speed = chosen.speed;
	int jobId = chosen.id;
	
	char buf[BUFSIZ + 1];
	sprintf(buf, "<choice>%d", jobId);
	client->writeToServer(buf);
	
	return speed;
}

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