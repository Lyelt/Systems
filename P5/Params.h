#ifndef PARAMS_H
#define PARAMS_H

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

	// Params constructor
	Params(int argc, char* argv[]);

	bool isInt(string text);

	void usage();
};

#endif