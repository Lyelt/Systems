#ifndef KID_H
#define KID_H
#include <vector>
#include "ClientHandler.h"
#include "tools.hpp"
#include "Job.h"

class ClientHandler;
enum Mood { Lazy, Tired, Prissy, Greedy, Impatient };

class Kid {
	private:
		ClientHandler* client;
		Mood mood;
		int points;
		int id;
		
		void doJob(int);
		bool isQuit(char*);
		int chooseJob(char*);
		vector<Job> deserializeJobTable(char*);
		
	public:
		Kid(ClientHandler*);
		~Kid() {}
		
		int handleMessage(char*);
};
#endif