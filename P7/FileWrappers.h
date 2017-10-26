#define SHA_DIGEST_LENGTH 100
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
	~Direntry() {}
};

// -----------------------------------------------------------------------------
// FileID class
class FileID {
public:
	string pathName;
	ino_t inodeNum;
	int fileLen;
	int nLinks;
	unsigned char fingerprint[SHA_DIGEST_LENGTH];

	FileID() {}
	~FileID() {}
	FileID(string, ino_t, int, int);
	
	void calculateSHA256();

	static bool byInode(ino_t node1, ino_t node2) { return (node1 < node2); }
	static bool bySize(FileID file1, FileID file2) { return (file1.fileLen < file2.fileLen); }
	//static bool byFprint(unsigned char fp1[], unsigned char fp2[]) { return (fp1 < fp2); }
};

#endif