#include "tools.hpp"
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

class Stats : public stat {
private:

public:
	Stats() {}
	~Stats() {}
	ino_t inode() { return st_ino; }
	off_t size() { return st_size; }
	nlink_t links() { return st_nlink; }

	void print(ostream& out)
	{
		out << "I-node " << inode() << " links " << links() << endl;
		
	}
};

class Direntry : public dirent {
private:

public:
	Direntry() {}
	~Direntry() {}
	char* name() { return d_name; }
	ino_t inode() { return d_ino; }
	__uint8_t type() { return d_type; }

	void printVerbose(ostream& out)
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
		out << setw(12) << typeString << setw(12) << inode() << setw(26) << name() << endl;
		cout << setw(12) << typeString << setw(12) << inode() << setw(26) << name() << endl;
	}
};

class FileID {
public:
	string pathname;
	ino_t inodeNum;
	int fileLen;
	int nLinks;


	FileID() {}
	~FileID() {}
	FileID(string name, ino_t num, int len, int links)
	{
		pathname = name;
		inodeNum = num;
		fileLen = len;
		nLinks = links;
	}

	static bool inodeCompare(ino_t node1, ino_t node2) { return (node1 < node2); }

	static bool lenCompare(int len1, int len2) { return (len1 < len2); }
};

class Params {
public:
	string directory = "";
	string outputFileName = "";
	bool verbose = false;
	bool deleteFile = false;
	bool outputFile = false;
	bool debug = false;
	int levelNum = 0;
	int sizeLimit = 0;

	Params(int, char*);
	~Params() {}

	//-----------------------------
	// Params constructor
	Params(int argc, char* argv[])
	{
		struct option longOpts[] = {
			{ "verbose"   , no_argument      , NULL, 'v' },
			{ "output"    , required_argument, NULL, 'o' },
			{ "debug"     , optional_argument, NULL,  0  },
			{ NULL        , 0                , NULL,  0  }
		};

		int optx, c, code;
		for(;;)
		{
			c = getopt_long(argc, argv, "o:vd", longOpts, &code);
			if(c==-1) break;
			switch (c)
			{
			case 'v':
				verbose = true;
				break;
			case 'o':
				outputFile = true;
				outputFileName = optarg;
				break;
			case 'd':
				deleteFile = true;
				break;
			case 0:
				debug = true;
				if (optarg != NULL && isInt(optarg))
				{
					levelNum = atoi(optarg);
				}
				break;
			case '?':
			default:
				usage();
				break;
			}
		}

		int required = optind;
		// If directory name is an int, don't try to set the size again
		bool sizeSet = false;
		for (optx = required; optx < argc; ++optx)
		{
			// Handle the rest of the parameters not handled by getopt
			if (isInt(argv[optx]) && !sizeSet)
			{
				sizeSet = true;
				sizeLimit = atoi(argv[optx]);
			}
			else 		
			{
				directory = argv[optx];
				if (optx != argc - 1) // Directory should be our last argument
				{
					usage();
				}
			}
			
		}
		if (directory == "") usage();
	}

	bool isInt(string text)
	{
		for (int i = 0; i < text.length(); ++i)
		{
			if (!isdigit(text[i]))
				return false;
		}
		return true;
	}

	void usage()
	{
		cout << "Usage: options [-vd --debug[=level] -o filename][min size(kB)] sweepDirectory" << endl;
		exit(1);
	}
};

class Sweeper {
private:
	Params* param;
	vector<FileID> files;
	string path;
	ofstream out;

public:
	Sweeper(int, char*);
	~Sweeper() {}

	//-----------------------------
	//Sweeper constructor
	Sweeper(int argc, char* argv[])
	{
		if (argc < 2)
			fatal("At least one parameter required: sweep directory");
		param = new Params(argc, argv);
	}

	void Run()
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

	void convertToAbsolute()
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

	void sweep()
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

	FileID getFileID(Direntry *dir)
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
	void print()
	{
		if (param->outputFile)
			printStats(out);

		printStats(cout);

		out.close();
	}

	void printHeader(ostream& outstream)
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

	void printStats(ostream& outstream)
	{
		//outstream << "Regular files: " << files.size() << endl;
		outstream << endl;
		for (int i = 0; i < files.size(); ++i)
		{
			outstream << "I-Node " << files[i].inodeNum << " links " << files[i].nLinks << endl;
			outstream << "\t" << files[i].pathname << endl;
		}
	}
};


int main(int argc, char* argv[])
{
	banner();
	Sweeper *s = new Sweeper(argc, argv);
	s->Run();
	delete s;

	return 0;
}
