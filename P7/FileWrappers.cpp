#include "tools.hpp"
#include "FileWrappers.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

// -----------------------------------------------------------------------------
// Stats functions
void Stats::print(ostream& out)
{
	out << "I-node " << inode() << " links " << links() << endl;	
}

// -------------------------------------------------------------------
// Direntry functions
void Direntry::printVerbose(ostream& out)
{
	string typeString;
	switch (type()) {
		case DT_REG:
			typeString = "file"; break;
		case DT_DIR:
			typeString = "directory"; break;
		case DT_LNK:
			typeString = "link"; break;
		default:
			typeString = "unknown"; break;
	}
	out << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
	cout << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
}

// -------------------------------------------------------------------
// FileID functions
FileID::FileID(string name, ino_t num, int len, int links)
{
	pathName = name;
	inodeNum = num;
	fileLen = len;
	nLinks = links;
}

void FileID::calculateSHA256()
{
	
}