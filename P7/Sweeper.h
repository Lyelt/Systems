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
	vector<FileID>::iterator start;
	vector<FileID>::iterator end;
	
	// --- Traversing the directory tree
	void normalizePathName(string);
	FileID getFileID(Direntry*, string);
	void travel(string, string);
	// --- Duplicate checking
	void reportDups();
	void checkDups();
	void dupLoop();
	bool areDups(string, string);
	// --- Print handlers
	void printHeader(ostream&);
	void printStats(ostream&);
	// --- Error checking
	void errorCheck(string);
	
public:
	Sweeper(int, char**);
	~Sweeper() {}

	void run();
};

#endif