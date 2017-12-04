#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H
#include "Mom.h"
#include "tools.hpp"
#include "Socket.hpp"

#define MAXCLIENTS 2
typedef struct pollfd toPoll;

class ServerHandler {
	private:
	    Socket server;
		int welcomeFd;
		int status;
		int nCli = 0;
		char hostname[256];
		toPoll ufds[MAXCLIENTS + 1];		
		toPoll* const welcome = &ufds[0];	// entry for the welcome socket
		toPoll* const worker = &ufds[1];	// first working socket.
		
		int doWelcome(int, int*, toPoll*);
		int getPort(int);
		int doService(toPoll*);
		int getClientNum(toPoll*);
		void printsockaddr_in(const char*, sockaddr_in);
		void acknowledge(int, toPoll*);

	public:
		ServerHandler(int);
		~ServerHandler() {}
		
		void start(int);
		int connectClients();
		void doPoll();
		void writeToClient(toPoll*, const char*);
		void writeToClient(int, const char*);
};

#endif