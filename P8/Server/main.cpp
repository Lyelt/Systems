#include "tools.hpp"
#include "Mom.h"

int main(int argc, char* argv[]) {
	if (argc != 2) 
		fatal("Usage %s port\n", argv[0]);
	
	int port = atoi(argv[1]);
	
	Mom::init(port);
}