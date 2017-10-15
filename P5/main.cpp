#include "tools.hpp"
#include "Sweeper.h"
#include "Params.h"
#include "FileWrappers.h"
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	banner();
	Sweeper *s = new Sweeper(argc, argv);
	s->Run();
	delete s;

	return 0;
}
