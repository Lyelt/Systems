#include "Mom.h"
#include "ServerHandler.h"

ServerHandler* Mom::server;
vector<Job> Mom::jobTable;
vector<Job> Mom::doneJobs;
time_t Mom::startTime;
int Mom::numKids;
int Mom::doneKids;

const char* CHOICE = "choice";
const char* DONE = "done";
const char* REQ = "req";

void Mom::init(int port) 
{
	banner();
	createJobs();
	server = new ServerHandler(port);
	
	// wait for all the clients to connect
	numKids = server->connectClients();
	
	// record the time
	time(&startTime);
	cout << "Current time: " << startTime << endl;
	
	// tell the server to poll and return a worker # whenever there's an event
	server->doPoll();
	
	delete server;
}

void Mom::createJobs()
{
	for(int i=0; i<MAXJOBS; ++i) {
		jobTable.push_back(createJob());
	}
}

Job Mom:: createJob()
{
	short speed = rand() % 5 + 1;
	short fun = rand() % 5 + 1;
	short diff = rand() % 5 + 1;
	return Job(speed, fun, diff);
}

// Mom will handle the messages
// She needs the message itself and the ID of the kid who sent it
void Mom::handleMessage(int kidId, char* message) 
{
	cout << "Server: Handling message (" << message << ") from kid (" << kidId << ")" << endl;
	
	char parsedMsg[BUFSIZ + 1];
	char parsedTag[BUFSIZ + 1];
	parseMessage(message, parsedTag, parsedMsg);
	cout << "Server: Message parsed - tag (" << parsedTag << ") and message (" << parsedMsg << ")" << endl;
	
	if (strcmp(parsedTag, CHOICE) == 0) { // job choice message
		cout << "Server: Received job choice message" << endl;
		
		int jobId = atoi(parsedMsg);
		
		for (vector<Job>::iterator it = jobTable.begin(); it != jobTable.end(); ++it) {
			if ((*it).id == jobId) {
				(*it).setKidId(kidId);	 // set kid id
				doneJobs.push_back(*it); // move job to done table
				
				jobTable.erase(it);		 		 // remove from job table
				jobTable.push_back(createJob()); // replace removed job
				break;
			}
		}
	}
	else if (strcmp(parsedTag, DONE) == 0) { // job done message
		cout << "Server: Received job done message" << endl;
		time_t now;
		time(&now);
		if (now - startTime > 21) { // it's been 21 time periods
			server->writeToClient(kidId, "<quit>");
			if (++doneKids == numKids) {
				cout << "All " << numKids << " kids done working." << endl;
				analyzeJobs();
			}
		}
		else {
			sendJobTable(kidId);
		}
	}
	else {
		sendJobTable(kidId);
	}
}

void Mom::sendJobTable(int kidId) 
{
	cout << "Sending job table to kid " << kidId << endl;
	string serialized = "<jobs>" + serializeJobTable();
	server->writeToClient(kidId, serialized.c_str());
}

string Mom::serializeJobTable()
{
	stringstream ss;
	//serialize the jobs for easy parsing
	//id:speed:fun:diff,id:speed:fun:diff,...
	for(auto& it : jobTable) {
		ss << it.id << ":" << it.speed << ":" <<
		it.fun << ":" << it.difficulty << ",";
	}
	
	ss.str().pop_back();
	cout << "Job table: " << ss.str() << endl;
	return ss.str();
}

void Mom::analyzeJobs()
{
	map<int, int> pointsEarned;
	
	for (auto& job : doneJobs) {
		pointsEarned[job.kidId] += job.getPoints();
	}
	
	for (auto& kid : pointsEarned) {
		cout << "Kid " << kid.first << " earned " << kid.second << " points." << endl;
	}
}