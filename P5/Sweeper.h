#ifndef SWEEPER_H
#define SWEEPER_H
#include "Params.h"
#include "FileWrappers.h"

class Sweeper {
private:
	Params* param;
	vector<FileID> files;
	string path;
	ofstream out;
	
	void convertToAbsolute();
	FileID getFileID(Direntry*);
	void sweep();
	void printHeader(ostream&);
	void printStats(ostream&);
	
public:
	Sweeper(int, char**);
	~Sweeper() {}

	void run();
};

#endif