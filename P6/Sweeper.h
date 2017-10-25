#ifndef SWEEPER_H
#define SWEEPER_H
#include "Params.h"
#include "FileWrappers.h"

class Sweeper {
private:
	Params* param;
	vector<FileID> files;
	string path;
	string relativePath;
	string absolutePath;
	ofstream out;
	
	void normalizePathName(string);
	FileID getFileID(Direntry*, string);
	void travel(string, string);
	void printHeader(ostream&);
	void printStats(ostream&);
	void errorCheck(string);
	
public:
	Sweeper(int, char**);
	~Sweeper() {}

	void run();
};

#endif