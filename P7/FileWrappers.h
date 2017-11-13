#ifndef FILEWRAPPERS_H
#define FILEWRAPPERS_H
#include <dirent.h>
#include <sys/stat.h>
#include <openssl/sha.h>

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
	string absolutePath;
	ino_t inodeNum;
	int fileLen;
	int nLinks;
	unsigned char fingerprint[SHA_DIGEST_LENGTH] = {0};

	FileID() {}
	~FileID() {}
	FileID(string, string, ino_t, int, int);
	
	void calculateSHA256(/*string*/);

	static bool byInode(FileID file1, FileID file2) { return (file1.inodeNum < file2.inodeNum); }
	static bool bySize(FileID file1, FileID file2) { return (file1.fileLen < file2.fileLen); }
	static bool byFprint(FileID file1, FileID file2) 
	{ 
		for (int k = 0; k < SHA_DIGEST_LENGTH; ++k) 
			if (file1.fingerprint[k] != file2.fingerprint[k])
				return file1.fingerprint[k] < file2.fingerprint[k];
	}
};

#endif