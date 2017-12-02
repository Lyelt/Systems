#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H
#include "Mom.h"
#include "tools.hpp"
#include "Socket.hpp"
#include "ServerHandler.h"

#define MAXCLIENTS 1
typedef struct pollfd toPoll;

class ServerHandler {
	private:
		int welcomeFd;
		int status;
		char hostname[256];
		toPoll ufds[MAXCLIENTS + 1];
		toPoll* const welcome = &ufds[0];	// entry for the welcome socket
		char greeting[256];
		
		int doWelcome(int, int*, toPoll*, const char*);
		int getPort(int);
		int doService(toPoll*);
		void printsockaddr_in(const char*, sockaddr_in);
		void acknowledge(int, toPoll*);

	public:
		ServerHandler(int);
		~ServerHandler() {}
		toPoll* const worker = &ufds[1];	// first working socket.
		int nCli = 0;
		
		void start(int);
		void connectClients();
		void doPoll();
		void writeToClient(toPoll*, char*);
		int readFromClient(toPoll*, char*);
};

#endif