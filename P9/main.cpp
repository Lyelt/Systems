#include "tools.hpp"
#include "Mom.h"
#include "Kid.h"
#include "Shared.h"
	
int main(int argc, char* argv[])
{
	banner();
	Mom* mom = new Mom();
	mom->run();
	delete mom;
}