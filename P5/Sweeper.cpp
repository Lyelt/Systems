#include "tools.hpp"
#include "Sweeper.h"
#include "FileWrappers.h"
#include "Params.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <map>
// -----------------------------------------------------------------------------
// Sweeper constructor
Sweeper::Sweeper(int argc, char* argv[])
{
	if (argc < 2)
		fatal("At least one parameter required: sweep directory");
	param = new Params(argc, argv);
}

// -----------------------------------------------------------------------------
// Print some output and run the actual sweep
void Sweeper::run()
{	
	if (param->outputFile)
		out.open(param->outputFileName);

	fbanner(out);
	
	// normalize the pathname
	//path = param->directory;
	path = convertToAbsolute(param->directory);
	
	// print some header info
	if (param->outputFile)
		printHeader(out);
	printHeader(cout);
		
	sweep(); // the actual sweep
	
	// print the file stat info
	if (param->outputFile)
		printStats(out);
	printStats(cout);
	out.close();
	delete param;
}

// -----------------------------------------------------------------------------
// Echo command line args
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
	outstream << "------------------------------------------------------" << endl;
}

// Print stats for all output
void Sweeper::printStats(ostream& outstream)
{
	outstream << endl;
	map<ino_t, vector<FileID>> fileMap;
	for (int i = 0; i < files.size(); ++i)
	{
		fileMap[files[i].inodeNum].push_back(files[i]);
	}
	
	for (map<ino_t, vector<FileID>>::iterator it = fileMap.begin(); it != fileMap.end(); ++it)
	{
		outstream << "I-Node " << it->first << " links " << it->second[0].nLinks << endl;
		for (int k = 0; k < it->second.size(); k++)
		{
			outstream << "\t" << it->second[k].pathName << endl;
		}
	}
}

// -----------------------------------------------------------------------------
// Convert to absolute file path
string Sweeper::convertToAbsolute(string path)
{
	char cwd[255];
	string absolutePath;
	string relativePath;
	if (path.substr(0, 1).compare("/") == 0)
	{
		return path; // it's already absolute
	}
	else if (path.substr(0, 2).compare("./") == 0) // path begins with ./
	{
		relativePath = path.substr(2, string::npos);
	}
	else // the relative path is just the name of the dir
	{
		relativePath = path;
	}

	getcwd(cwd, 255); // magic number because getcwd won't let me omit it
	absolutePath = strcat(cwd, "/");
	absolutePath = absolutePath + relativePath;

	return absolutePath;
}

// -----------------------------------------------------------------------------
// Open, read, and process the directory entries
void Sweeper::sweep()
{
	Direntry *dir;
	DIR *d;

	if ((d = opendir(path.c_str())) == NULL)
		errorCheck(path);

	// read all the entries in the directory
	while ((dir = static_cast<Direntry*>(readdir(d))) != NULL)
	{
		if (param->verbose)
			dir->printVerbose(out);	// Print all the verbose output up front

		if (dir->type() == DT_REG)
		{
			FileID f(getFileID(dir));
			if (f.fileLen >= param->sizeLimit) // size limit
				files.push_back(f); // process regular files using FileIDs
		}
			
	}
	closedir(d);
}

// Construct the file ID using lstat info of the object
FileID Sweeper::getFileID(Direntry *dir)
{
	// the absolute path of the file
	string filePath = path + "/" + dir->name();
	//Stats stats;
	struct stat s;

	// store the file stats in the struct
	if (lstat(filePath.c_str(), &s) < 0)
		errorCheck(filePath);

	FileID f(dir->name(), s.st_ino, s.st_size, s.st_nlink);
	return f;
}

// Check errno and print a useful comment
void Sweeper::errorCheck(string failPath)
{
	string errorMessage;
	cout << "Directory or file failed to be read: " << failPath << endl;
	switch (errno) {
		case EACCES:
			errorMessage = "Permission denied.";
			break;
		case EIO:
			errorMessage = "I/O read error.";
			break;
		case ELOOP:
			errorMessage = "Symbolic link loop.";
			break;
		case ENAMETOOLONG:
			errorMessage = "Path name too long.";
			break;
		case EOVERFLOW:
			errorMessage = "File size too large.";
			break;
		case EBADF:
		case ENOENT:
		case ENOTDIR:
			errorMessage = "Bad path name.";
			break;
		default:
			errorMessage = "Unknown error.";
			break;
	}
	cout << errorMessage << endl;
	exit(1);
}