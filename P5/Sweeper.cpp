#include "tools.hpp"
#include "Sweeper.h"
#include "FileWrappers.h"
#include "Params.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

//Sweeper constructor
Sweeper::Sweeper(int argc, char* argv[])
{
	if (argc < 2)
		fatal("At least one parameter required: sweep directory");
	param = new Params(argc, argv);
}

void Sweeper::Run()
{	
	if (param->outputFile)
		out.open(param->outputFileName);

	fbanner(out);
	path = param->directory;
	convertToAbsolute();
		
	if (param->outputFile)
		printHeader(out);
	printHeader(cout);
		
	sweep();
	print();
	delete param;
}

void Sweeper::convertToAbsolute()
{
	if (path.substr(0, 2).compare("./") == 0)
	{
		char cwd[255];
		string relative = path.substr(3, string::npos);
		getcwd(cwd, 255); // magic number
		path = strcat(cwd, "/");
		path = path + relative;
	}
}

void Sweeper::sweep()
{
	Direntry *dir;
	DIR *d;

	if ((d = opendir(path.c_str())) == NULL)
		fatal("Directory could not be opened: %i", errno); // Replace with errno check

	// read all the entries in the directory
	while ((dir = static_cast<Direntry*>(readdir(d))) != NULL)
	{
		if (param->verbose)
			dir->printVerbose(out);

		if (dir->type() == DT_REG)	// process regular files using FileIDs
			files.push_back(getFileID(dir));
	}
	closedir(d);
}

FileID Sweeper::getFileID(Direntry *dir)
{
	// the absolute path of the file
	string filePath = path + "/" + dir->name();
	//Stats stats;
	struct stat s;

	// store the file stats in the struct
	if (lstat(filePath.c_str(), &s) < 0)
		fatal("Could not get stats on file: %s, Errno: %i", filePath, errno);

	FileID f(dir->name(), s.st_ino, s.st_size, s.st_nlink);
	return f;
}

// Wrapper for printing to both file and cout
void Sweeper::print()
{
	if (param->outputFile)
		printStats(out);

	printStats(cout);

	out.close();
}

void Sweeper::printHeader(ostream& outstream)
{
	outstream << "Verbose? " << (param->verbose ? "Yes" : "No") << endl;
	outstream << "Delete? " << (param->deleteFile ? "Yes" : "No") << endl;
	outstream << "Debug? " << (param->debug ? "Yes" : "No") << endl;
	if (param->debug)
		outstream << "Debug level: " << param->levelNum << endl;
	outstream << "Output file name: " << (param->outputFile ? param->outputFileName : "None") << endl;
	outstream << "Size: " << param->sizeLimit << " K or greater" << endl;
	outstream << "Directory: " << param->directory << endl;
	outstream << "-------------------------------------------------------" << endl;
}

void Sweeper::printStats(ostream& outstream)
{
	//outstream << "Regular files: " << files.size() << endl;
	outstream << endl;
	for (int i = 0; i < files.size(); ++i)
	{
		outstream << "I-Node " << files[i].inodeNum << " links " << files[i].nLinks << endl;
		outstream << "\t" << files[i].pathname << endl;
	}
}
