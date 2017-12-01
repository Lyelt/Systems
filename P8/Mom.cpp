//#include "Mom.h"
#include "tools.hpp"
#include "Socket.hpp"
#include "Job.h"
#include <vector>
#define MAXCLIENTS 4				// Unrealistically small, for testing.
#define MAXJOBS 10
typedef struct pollfd    toPoll;
vector<Job> jobTable;
vector<Job> doneJobs;
		
int doWelcome(int, int*, toPoll*, const char*);
int doService(toPoll*);
int getPort(int);
void printsockaddr_in(const char*, sockaddr_in);
void createJobs();
void initializeSocket();
void acknowledge(int, toPoll*);
		
int main(int argc, char *argv[]) {
    //const char* proc = argv[0];
    //if (argc<2) fatal("usage: %s port\n", proc);
	banner();
	createJobs();
	initializeSocket();
}

void createJobs()
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

void initializeSocket()
{
	int port = 15000;//strtol(argv[1], NULL, 10);
    char hostname[256];					// to store name of local host.
    gethostname(hostname, 256);			// get name of local host.
    say("server is %s", hostname);

    int nCli = 0;						// number of working sockets still alive
    int welcomeFd;						// fd for the welcoming socket

    toPoll ufds[MAXCLIENTS + 1];		// These are the connections to poll.
    toPoll* const welcome = &ufds[0];	// entry for the welcome socket
    toPoll* const worker = &ufds[1];	// first working socket.

    int status;							// return values from system calls.

    char greeting[256];
    sprintf(greeting, "Connected to (%s.%d)\n", hostname, port);

    //==========================================================================
    // Create an internet socket in the file table; return the file descriptor.
    // Use INET protocol family, stream i/o, exact protocol irrelevant (== 0)
    Socket server;
    server.listen(port);
    welcomeFd = server.sockfd();

    // Install welcome socket in ufds[] polling table
    welcome->fd = welcomeFd;
    welcome->events = POLLIN;		// let socket listen for calls from clients

    cout <<"Opened socket as fd (" <<welcomeFd <<") on port ("
    <<ntohs(server.port()) <<") for stream i/o\n" ;
    cout << "welcome" << server;

	// =====================================================================
    // If welcome socket has a caller, service it.
	for (;;) {
        if (welcome->revents != 0 )	{
            if (welcome->revents & POLLIN)	{
				// create a new client connection. 
                if (nCli < MAXCLIENTS) {
					int newWorker = doWelcome(welcomeFd, &nCli, worker, greeting);
					if (newWorker > 0) {
						acknowledge(nCli, worker);
						nCli += newWorker;
					}
				}
            }
            else fatalp("Error involving welcome mat: %d", welcome->revents);
        }
		
		if (nCli == MAXCLIENTS) break; // All kids have connected
		// stop looking for new clients if max has been reached ----------------
        welcome->events = (nCli < MAXCLIENTS) ? POLLIN : 0;
	}
	// record current time
	// =====================================================================
    // Scan the working sockets and process whatever tasks you find
    for (;;) {
		status = poll(ufds, 1 + nCli, -1);
        if (status < 0)  fatalp("Error in poll().\t");
        //if (status == 0) cout <<proc <<": poll timed out\n";
		
        int k;
        for (k = 0; k < nCli; k++) {
            if (worker[k].revents != 0) {
                status = doService( &worker[k] );
                if ( status == -1){		// Remove dead socket from polling table
                    worker[k] = worker[--nCli];		// decrement # of workers
                }
            }
        }
    }	// end polling loop
}

// Send an acknowledgement to a newly connected child
void acknowledge(int id, toPoll* worker)
{
	char ack[256];
    sprintf(ack, "welcome %d", id);
	
	int bytes = write(worker[id].fd, ack, strlen(ack));
	if (bytes < 1) say("Error during ack write.");
	
	cout << "New client (" << id << ") acknowledged." << endl;
}

//==============================================================================
// Returns 1 if connection succeeded; 0 if no new worker was created
int doWelcome(int welcomeSock, int* nClip, toPoll* worker,
              const char* greeting) {
    int nCli = *nClip;	// Make a local copy of the in/out parameter
    sockaddr_in newCaller;
    socklen_t sockLen = sizeof newCaller;

    int newfd = accept(welcomeSock, (sockaddr*) &newCaller, &sockLen);
    if (newfd < 0) {
        say("False alarm: packet was rejected.");
        return 0;  // False alarm, nobody there, 0 new clients.
    }

    printsockaddr_in("working--from newCaller", newCaller);
    // -------------------------------- Put new socket into our polling list.
    worker[nCli].fd = newfd;
    worker[nCli].events = POLLIN;
    worker[nCli].revents = 0;

    // --------------------------------- We have a new caller -- send an ack.
	// ACK should have child ID number
    int bytes = write(newfd, greeting, strlen(greeting));
    if (bytes < 1) say("Error while writing to socket");
	printf("New client welcomed.\n");
    *nClip = nCli;		// Return the possibly-modified index of last client.
    return 1;           // Success.  We got 1 new client.
}

//==============================================================================
// Returns -1 if the connection was closed; 0 otherwise
int doService(toPoll* p) {
    char buf[BUFSIZ + 1];
    int retval = 0;		// Change in number of workers.

    // ------------------------------------------- Test for a message event.
    if (p->revents & POLLIN) {   // This is a read event--read it
		// deserialize the message 
		// if its a choice and its good, move to doneJobs vector
		// if its a job-done message, check if 21 periods have elapsed, send message
        int bytes = read(p->fd, buf, (sizeof buf) -1);   // -1 = don't time out.
        // --------------------------------- We got a message, so handle it.
        if (bytes > 0) {
            buf[bytes] = '\0';	 // read does not null terminate.
            cout <<"\nFrom port " <<getPort(p->fd) <<":\t\t( " <<buf;
        }
        // -----------------------No message, so handle the possible errors.
        else if (bytes == 0) {	// Indicates end of file.
            printf("\nclosing socket on port %d\n", getPort(p->fd));
            close(p->fd);
            retval = -1;
        } else if (errno == ECONNRESET) {
            sayp("socket %d disappeared", getPort(p->fd));
            close(p->fd);
            retval = -1;
        } else {
            fatalp("Error %d from read, port %d", bytes, getPort(p->fd));
        }
    }
    // ---------------------------- It wasn't a message, so test for hangup.
    else if (p->revents & POLLHUP) {  // Caller hung up.
        say("Removing dead socket %d\n", getPort(p->fd));
        close(p->fd);
        retval = -1;
    }	// end if p has events.
    return retval;
}

//==============================================================================
int getPort(int fd){
    sockaddr_in client;
    socklen_t sockLen = sizeof client;
    int status = getpeername(fd, (sockaddr*) &client, &sockLen);
    if (status<0) fatalp("Can't get port# of socket (%d)", fd);
    return ntohs(client.sin_port);
}

// =============================================================================
void printsockaddr_in(const char* who, sockaddr_in sock) {
    printf("%s socket is  {\n\t"
           "sin_family        = %d\n\t"
           "sin_addr.s_addr   = %s\n\t"
           "sin_port          = %d\n\t"
           "}\n", who, sock.sin_family, inet_ntoa(sock.sin_addr),
           ntohs(sock.sin_port));
}
