#include "tools.hpp"
#include "ClientHandler.h"

int main(int argc, char* argv[]) {
	ClientHandler* ch = new ClientHandler();
	ch->start(argc, &argv[0]);
	delete ch;
}