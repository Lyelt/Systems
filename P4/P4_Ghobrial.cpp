#include "tools.hpp"
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

class Stats : stat {
private:

public:
	ino_t inode() { return st_ino; }
	off_t size() { return st_size; }
	nlink_t links() { return st_nlink; }

	void print(ostream& out)
	{
		out << inode() << size() << links() << endl;
	}
};

class Direntry : public dirent {
private:

public:
	char* name() { return d_name; }
	ino_t inode() { return d_ino; }
	__uint8_t type() { return d_type; }

	void print(ostream& out)
	{
		out << name() << inode() << type() << endl;
	}
};

class FileID {
private:
	string pathname;
	ino_t inodeNum;
	int fileLen;

public:
	FileID() {}
	FileID(string name, ino_t num, int len)
	{
		pathname = name;
		inodeNum = num;
		fileLen = len;
	}

	static bool inodeCompare(ino_t node1, ino_t node2) { return (node1 < node2); }

	static bool lenCompare(int len1, int len2) { return (len1 < len2); }
};

class Params {
public:
//private:
	//ofstream out;
	string directory = "";
	string outputFileName = "";
	bool verbose = false;
	bool deleteFile = false;
	bool outputFile = false;
	bool debug = false;
	int levelNum = 0;
	int sizeLimit = 0;

//public:
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
				//out.open(optarg);
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

		path = param->directory;
		convertToAbsolute();

		print();
		Sweep();
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

	void Sweep()
	{
		Direntry *dir;
		DIR *d;
		vector<FileID> files;

		if ((d = opendir(path.c_str())) == NULL)
			fatal("Directory could not be opened."); // Replace with errno check

		fbanner(out);
		while ((dir = readdir(d)) != NULL)
		{
			dir->print(out);
		}

		closedir(d);
	}

	// Wrapper for printing to both file and cout
	void print()
	{
		if (param->outputFile)
		{
			//fbanner(out);
			printOut(out);
			out.close();
		}
		printOut(cout);
	}

	void printOut(ostream& outstream)
	{
		outstream << "Verbose? " << (param->verbose ? "Yes" : "No") << endl;
		outstream << "Delete? " << (param->deleteFile ? "Yes" : "No") << endl;
		outstream << "Debug? " << (param->debug ? "Yes" : "No") << endl;
		if (param->debug)
			outstream << "Debug level: " << param->levelNum << endl;
		outstream << "Output file name: " << (param->outputFile ? param->outputFileName : "None") << endl;
		outstream << "Size: " << param->sizeLimit << " K or greater" << endl;
		outstream << "Directory: " << param->directory << endl;
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
