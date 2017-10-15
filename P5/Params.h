#ifndef PARAMS_H
#define PARAMS_H

class Params {
private:
	void processOptions(int, char**);
	void processOtherArgs(int, char**);
	bool isInt(string);
	void usage();
	
public:
	string directory = "";
	string outputFileName = "";
	bool verbose = false;
	bool deleteFile = false;
	bool outputFile = false;
	bool debug = false;
	int levelNum = 0;
	int sizeLimit = 0;

	Params(int, char**);
	~Params() {}

	// Params constructor
	//Params(int argc, char* argv[]);
};

#endif