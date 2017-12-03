#include "tools.hpp"
#include "Socket.hpp"
#include "ClientHandler.h"
#include "Kid.h"

// -----------------------------------------------------------------------------
// arg 1 is remote host. 
void ClientHandler::start(int argc, char *argv[] )
{
    if (argc<3) fatal("usage: %s host port\n", argv[0]);
    // define meaningful names for the command-line arguments.
    char* proc = argv[0];
    char* servername = argv[1];  // registered network name of the server.
    int   port = strtol(argv[2],NULL,10); 

    char buf[BUFSIZ+1];         // An input buffer for the client socket
    int nBytes;
	
	// =========================================================================
    // Install a socket in the client's file table.
	// Make an internet-transmitted, file-i/o-style, protocol-whatever plug.
    // Error checking is done in the Socket class.
    client.connect(servername, port);
    client.printPeer(cout);        // Print peer
	kid = new Kid(this);
	int status = 0;
    while (status != -1) {
		// wait for server to acknowledge the connection. 
		nBytes = read( client.sockfd(), buf, sizeof buf );
		buf[nBytes + 1] = '\0';
		cout << nBytes << " bytes read from server." << endl;
		if ( nBytes > 0 ) {
			status = kid->handleMessage(buf);
		}
		else 
			fatalp("%s: Error while reading from socket.", proc );
    }
    
	cout << "Client received quit message." << endl;
	delete kid;
    exit(0);
}

void ClientHandler::writeToServer(const char* message)
{
	int nBytes = write( client.sockfd(), message, strlen(message) );
    if( nBytes < 0 ) fatalp( "Error while writing to socket.");
}