#ifndef FILEWRAPPERS_H
#define FILEWRAPPERS_H
#include <dirent.h>
#include <sys/stat.h>
// -----------------------------------------------------------------------------
// Stats class
class Stats : public stat {
public:
	ino_t inode() { return st_ino; }
	off_t size() { return st_size; }
	nlink_t links() { return st_nlink; }

	void print(ostream&);
};

// -----------------------------------------------------------------------------
// Direntry class
class Direntry : public dirent {
public:
	char* name() { return d_name; }
	ino_t inode() { return d_ino; }
	__uint8_t type() { return d_type; }

	void printVerbose(ostream&);
};

// -----------------------------------------------------------------------------
// FileID class
class FileID {
public:
	string pathName;
	ino_t inodeNum;
	int fileLen;
	int nLinks;

	FileID() {}
	FileID(string, ino_t, int, int);
	static bool inodeCompare(ino_t node1, ino_t node2) { return (node1 < node2); }
	static bool lenCompare(int len1, int len2) { return (len1 < len2); }
};

#endif