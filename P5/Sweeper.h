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
	FileID getFileID(Direntry *dir);
	
public:
	Sweeper(int, char**);
	~Sweeper() {}

	void Run();
	void sweep();

	void print();
	void printHeader(ostream& outstream);
	void printStats(ostream& outstream);
};

#endif