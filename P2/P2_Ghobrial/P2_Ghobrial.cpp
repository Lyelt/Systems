// P2_Ghobrial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tools.hpp"

class Params
{
	private:
		ostream& outStream;
		string pathName = "";
		int sizeLimit = 0;
		string deleteFileName = "";
		bool verbose = false;
		bool deleteFile = false;

	public:
		Params(int, char*);

	// --------------------------------------
	// Params constructor
	Params(int argc, char* argv[]) : outStream(cout)
	{
		for (int i = 0; i < argc; ++i)
		{
			if (isSwitch(argv[i]))
			{
				switch (argv[i][1])
				{
					case 'v':
					case 'V':
						verbose = true;
						break;
					case 'o':
					case 'O':
						outStream(argv[++i]);
						break;
					case 'd':
						if (argv[i] == "-delete") {
							deleteFile = true;
							if (i != argc - 1)
								deleteFileName = (argv[++i]);
							break;
						}
						// Fall through to default if switch wasn't -delete
					default:
						fatal("Switch not recognized: %s", argv[i]);
						break;
				}
				continue;
			}
			else if (isInt(argv[i]))
			{
				sizeLimit = atoi(argv[i]);
				continue;
			}
			else
			{
				if (pathName == "")
				{
					pathName = argv[i];
				}
			}
		}
	}

	bool isSwitch(string text)
	{
		return text[0] == '-';
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

	void print()
	{

	}
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		fatal("At least one argument required: path name");
	}

	Params p(argc, argv);
	p.print();

    return 0;
}