#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#define FILENAME "P1_Ghobrial.txt"

using namespace std;

void printOutput(char* argv[], vector<string> switches, vector<string> arguments);

int main(int argc, char* argv[])
{
	vector<string> switches;
	vector<string> arguments;

	for (int i = 1; i < argc; ++i)	// Skip the first, will always be command
	{
		if (argv[i][0] == '-')
		{
			switches.push_back(argv[i]);
		}
		else
		{
			arguments.push_back(argv[i]);
		}
	}
	
	printOutput(argv, switches, arguments);

    return 0;
}

void printOutput(char* argv[], vector<string> switches, vector<string> arguments)
{
	ofstream outStream(FILENAME, ios::out | ios::app);
	cout << "--------------------------------" << endl;
	outStream << "--------------------------------" << endl;
	cout << "Command: " << argv[0] << endl;
	outStream << "Command: " << argv[0] << endl;
	for (int i = 0; i < switches.size(); ++i)
	{
		cout << "Switch: " << switches[i].c_str() << endl;
		outStream << "Switch: " << switches[i].c_str() << endl;
	}

	for (int i = 0; i < arguments.size(); ++i)
	{
		cout << "Argument: " << arguments[i].c_str() << endl;
		outStream << "Argument: " << arguments[i].c_str() << endl;
	}

	outStream.close();
}