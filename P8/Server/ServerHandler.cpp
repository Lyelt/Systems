#include "ServerHandler.h"

ServerHandler::ServerHandler(int port) {
    gethostname(hostname, 256);			// get name of local host.
    say("server is %s", hostname);

    //==========================================================================
    // Create an internet socket in the file table; return the file descriptor.
    // Use INET protocol family, stream i/o, exact protocol irrelevant (== 0)
    server.listen(port);
    welcomeFd = server.sockfd();

    // Install welcome socket in ufds[] polling table
    welcome->fd = welcomeFd;
    welcome->events = POLLIN;		// let socket listen for calls from clients

    cout <<"Opened socket as fd (" <<welcomeFd <<") on port ("
    <<ntohs(server.port()) <<") for stream i/o\n" ;
    cout << "welcome" << server;
}

// =====================================================================
// If welcome socket has a caller, service it.
int ServerHandler::connectClients() {
	cout << "Waiting for " << MAXCLIENTS << " client(s) to connect." << endl;
	for (;;) {
		
		status = poll(ufds, 1 + nCli, -1);
        if (status < 0)  fatalp("Error in poll().\t");
        if (status == 0) cout  <<": poll timed out\n";

        if (welcome->revents != 0 )	{
            if (welcome->revents & POLLIN)	{
				// create a new client connection. 
                if (nCli < MAXCLIENTS) {
					int newWorker = doWelcome(welcomeFd, &nCli, worker);
					if (newWorker > 0) {
						nCli += newWorker;
						cout << "New client connected." << endl;
					}
				}
            }
            else fatalp("Error involving welcome mat: %d", welcome->revents);
        }
		
		if (nCli == MAXCLIENTS) break; // All kids have connected
		// stop looking for new clients if max has been reached ----------------
        welcome->events = (nCli < MAXCLIENTS) ? POLLIN : 0;
	}
	
	cout << "All "<< nCli <<" clients connected." << endl;
	return nCli;
}

//==============================================================================
// Returns 1 if connection succeeded; 0 if no new worker was created
int ServerHandler::doWelcome(int welcomeSock, int* nClip, toPoll* worker) 
{
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
	char ack[256];
    sprintf(ack, "<ack>%d", nCli);
    int bytes = write(newfd, ack, strlen(ack));
    if (bytes < 1) say("Error while writing to socket");
	cout << "New client welcomed." << endl;
    *nClip = nCli;		// Return the possibly-modified index of last client.
    return 1;           // Success.  We got 1 new client.
}

// =====================================================================
// Scan the working sockets and process whatever tasks you find
void ServerHandler::doPoll() 
{
	cout << "Starting polling loop." << endl;
    for (;;) {
		status = poll(ufds, 1 + nCli, -1);
        if (status < 0)  fatalp("Error in poll().\t");
		
        for (int k = 0; k < nCli; k++) {
            if (worker[k].revents != 0) {
                status = doService( &worker[k] );
                 if ( status == -1){		// Remove dead socket from polling table
                     worker[k] = worker[--nCli];		// decrement # of workers
                  }
            }
        }
    }
}

int ServerHandler::doService(toPoll* p) 
{
	int retval = 0;		// Change in number of workers.
	char buf[BUFSIZ + 1];
    // ------------------------------------------- Test for a message event.
    if (p->revents & POLLIN) {   // This is a read event--read it
		
		int bytes = read(p->fd, buf, (sizeof buf) -1);   // -1 = don't time out.
		// --------------------------------- We got a message, so handle it.
		if (bytes > 0) {
			buf[bytes] = '\0';
			Mom::handleMessage(getClientNum(p), buf);
		}
		// -----------------------No message, so handle the possible errors.
		else if (bytes == 0) {	// Indicates end of file.
			printf("\nclosing socket on port %d\n", getPort(p->fd));
			close(p->fd);
			retval = -1;
		} 
		else if (errno == ECONNRESET) {
			sayp("socket %d disappeared", getPort(p->fd));
			close(p->fd);
			retval = -1;
		} 
		else {
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
int ServerHandler::getPort(int fd) 
{
    sockaddr_in client;
    socklen_t sockLen = sizeof client;
    int status = getpeername(fd, (sockaddr*) &client, &sockLen);
    if (status<0) fatalp("Can't get port# of socket (%d)", fd);
    return ntohs(client.sin_port);
}

// =============================================================================
void ServerHandler::printsockaddr_in(const char* who, sockaddr_in sock) {
    printf("%s socket is  {\n\t"
           "sin_family        = %d\n\t"
           "sin_addr.s_addr   = %s\n\t"
           "sin_port          = %d\n\t"
           "}\n", who, sock.sin_family, inet_ntoa(sock.sin_addr),
           ntohs(sock.sin_port));
}

// Send an acknowledgement to a newly connected child
void ServerHandler::acknowledge(int id, toPoll* worker)
{
	cout << "About to acknowledge" << endl;
	char ack[256];
    sprintf(ack, "<ack>%d", id);
	
	int bytes = write(worker[id].fd, ack, strlen(ack));
	if (bytes < 1) say("Error during ack write.");
	
	cout << "New client (" << id << ") acknowledged." << endl;
}

void ServerHandler::writeToClient(int kidId, const char* message)
{
	writeToClient(&worker[kidId], message);
}

void ServerHandler::writeToClient(toPoll* p, const char* message)
{
	int bytes = write(p->fd, message, strlen(message));
	if (bytes < 1) say ("Error writing to client.");
}

int ServerHandler::getClientNum(toPoll* p) 
{
	for(int k=0; k<nCli; ++k) {
		if (p->fd == worker[k].fd) {
			return k;
		}
	}
	return -1;
}