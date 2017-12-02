#include "Mom.h"
#include "tools.hpp"
#include "Socket.hpp"
#include "Job.h"
#define MAXJOBS 10

ServerHandler* Mom::server;
vector<Job> Mom::jobTable;
vector<Job> Mom::doneJobs;
time_t Mom::startTime;

void Mom::init(int port) 
{
	banner();
	createJobs();
	server = new ServerHandler(port);
	
	// wait for all the clients to connect
	server->connectClients();
	
	// record the time
	time(&startTime);
	cout << "All "<< server->nCli <<" clients connected." << endl;
	cout << "Current time: " << startTime << ". Now polling." << endl;
	
	// tell the server to poll and return a worker # whenever there's an event
	server->doPoll();
	
	delete server;
}

void Mom::createJobs()
{
	srand(time(NULL));
	for(int i=0; i<MAXJOBS; ++i)
	{
		short speed = rand() % 5 + 1;
		short fun = rand() % 5 + 1;
		short diff = rand() % 5 + 1;
		
		Job job(speed, fun, diff);
		jobTable.push_back(job);
	}
}

// 
void Mom::handleMessage(toPoll* p, char* message) {
	int kidId = getKid(p);
	if (strncmp(message, "<choice>", 8) == 0) { // job choice message
		cout << "Received job choice message" << endl;
		char* idSubstr;
		strncpy(idSubstr, message + 8, strlen(message) - 8);
		int id = atoi(idSubstr);
		for (vector<Job>::iterator it = jobTable.begin(); it != jobTable.end(); ++it) {
			if ((*it).id == id) {
				(*it).setKidId(kidId);	 // set kid id
				doneJobs.push_back(*it); // move job to done table
				jobTable.erase(it);		 // remove from job table
				break;
			}
		}
		//serializeJobTable(); 
	}
	else if (strncmp(message, "<done>", 6) == 0) { // job done message
		cout << "Received job done message" << endl;
		time_t now;
		time(&now);
		if (now - startTime > 21) { // it's been 21 time periods
			//return "<quit>";
		}
		else {
			//serializeJobTable();
		}
		// If 3.5 hrs passed, send quit message
		// Otherwise, send job table
	}
	else {
		cout << "Received some other message" << endl;
		// Send job table? Would never get another message?
	}
}

int Mom::getKid(toPoll* p) {
	for(int k=0; k<server->nCli; ++k) {
		if (p->fd == server->worker[k].fd) {
			return k;
		}
	}
	return -1;
}


