#include "Mom.h"
#include "tools.hpp"
#include "Socket.hpp"
#include "Job.h"


#define MAXJOBS 10

Mom::Mom(int port) 
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
	for(;;) {
		int toService = server->doPoll();
		int status = doService(&(server->worker[toService]));
	}
	
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

//==============================================================================
// Returns -1 if the connection was closed; 0 otherwise
int Mom::doService(toPoll* p) {
    
    int retval = 0;		// Change in number of workers.

    // ------------------------------------------- Test for a message event.
    if (p->revents & POLLIN) {   // This is a read event--read it
		char buf[BUFSIZ + 1];
        server->readFromClient(p, buf);
		int kidId = getKid(p);
    }
    // ---------------------------- It wasn't a message, so test for hangup.
    else if (p->revents & POLLHUP) {  // Caller hung up.
        //say("Removing dead socket %d\n", getPort(p->fd));
        close(p->fd);
        retval = -1;
    }	// end if p has events.
    return retval;
}

// 
void Mom::handleMessage(char* buf, int kidId, time_t startTime) {
	if (strncmp(buf, "<choice>", 8) == 0) { // job choice message
		cout << "Received job choice message" << endl;
		char* idSubstr;
		strncpy(idSubstr, buf + 8, strlen(buf) - 8);
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
	else if (strncmp(buf, "<done>", 6) == 0) { // job done message
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


