#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include "Kid.h"
#include "tools.hpp"
#include "Socket.hpp"

class Kid;

class ClientHandler {
	private:
		Socket client;
		Kid* kid;
		
	public:
		ClientHandler() {}
		~ClientHandler() {}
		
		void start(int, char**);
		void writeToServer(char*);
};
#endif