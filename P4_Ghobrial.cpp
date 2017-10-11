#include "tools.hpp"
#include <getopt.h>

class Params
{
private:
	ofstream out;
	string directory = "";
	string outputFileName = "";
	bool verbose = false;
	bool deleteFile = false;
	bool outputFile = false;
	bool debug = false;
	bool recursive = false;
	int levelNum = 0;
	int sizeLimit = 0;

public:
	Params(int, char*);
	~Params() {}

	//-----------------------------
	// Params constructor
	Params(int argc, char* argv[])
	{
		struct option longOpts[] = {
			{ "verbose"   , no_argument      , NULL, 'v' },
			{ "output"    , required_argument, NULL, 'o' },
			{ "recursive" , no_argument      , NULL, 'R' },
			{ "debug"     , optional_argument, NULL,  0  },
			{ NULL        , 0                , NULL,  0  }
		};

		int optx, c, code;
		for(;;)
		{
			c = getopt_long(argc, argv, "o:vdR", longOpts, &code);
			if(c==-1) break;
			switch (c)
			{
			case 'v':
				verbose = true;
				break;
			case 'o':
				outputFile = true;
				outputFileName = optarg;
				out.open(optarg);
				break;
			case 'd':
				deleteFile = true;
				break;
			case 'R':
				recursive = true;
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

	// Wrapper class for printing to both file and cout
	void print()
	{
		if (outputFile)
		{
			fbanner(out);
			printOut(out);
			out.close();
		}
		printOut(cout);
	}

	void printOut(ostream& outstream)
	{		
		outstream << "Verbose? " << (verbose ? "Yes" : "No") << endl;
		outstream << "Delete? " << (deleteFile ? "Yes" : "No") << endl;
		outstream << "Recursive? " << (recursive ? "Yes" : "No") << endl;
		outstream << "Debug? " << (debug ? "Yes" : "No") << endl;
		if (debug)
			outstream << "Debug level: " << levelNum << endl;
		outstream << "Output file name: " << (outputFile ? outputFileName : "None") << endl;
		outstream << "Size: " << sizeLimit << " K or greater" << endl;
		outstream << "Directory: " << directory << endl;	
	}

	void usage()
	{
		cout << "Usage: options [-vdR --debug[=level] -o filename][min size(kB)] sweepDirectory" << endl;
		exit(1);
	}
};

class Sweeper
{
private:
	Params* param;


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
		param->print();
		delete param;
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
