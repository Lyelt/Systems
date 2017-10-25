#include "tools.hpp"
#include "Params.h"
#include <getopt.h>

// -----------------------------------------------------------------------------
// Params constructor 
Params::Params(int argc, char* argv[])
{
	processOptions(argc, argv);
	
	processOtherArgs(argc, argv);
	
}

// -----------------------------------------------------------------------------
// Use longopts and getopt_long to process switches
void Params::processOptions(int argc, char* argv[])
{
	struct option longOpts[] = {
		{ "verbose"   , no_argument      , NULL, 'v' },
		{ "output"    , required_argument, NULL, 'o' },
		{ "debug"     , optional_argument, NULL,  0  },
		{ NULL        , 0                , NULL,  0  }
	};

	int c, code;
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
}

// -----------------------------------------------------------------------------
// Handle the arguments not processed by getopt
void Params::processOtherArgs(int argc, char* argv[])
{
	int required = optind;
	// If directory name is an int, don't try to set the size again
	bool sizeSet = false;
	for (int optx = required; optx < argc; ++optx)
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

// -----------------------------------------------------------------------------
// Test if a string contains all digits
bool Params::isInt(string text)
{
	for (int i = 0; i < text.length(); ++i)
	{
		if (!isdigit(text[i]))
			return false;
	}
	return true;
}

// -----------------------------------------------------------------------------
// Usage comment
void Params::usage()
{
	cout << "Usage: options [-vd --debug[=level] -o filename][min size(kB)] sweepDirectory" << endl;
	exit(1);
}