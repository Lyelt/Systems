#include "tools.hpp"
#include "Socket.hpp"

// -----------------------------------------------------------------------------
// arg 1 is remote host. 
enum Mood { Lazy, Tired, Prissy, Greedy, Impatient };

int main( int argc, char *argv[] )
{
    if (argc<4) fatal("usage: %s host port file\n", argv[0]);
    // define meaningful names for the command-line arguments.
    char* proc = argv[0];
    char* servername = argv[1];  // registered network name of the server.
    int   port = strtol(argv[2],NULL,10); 
    char* fname = argv[3];      // We need a file for this demo application.

    ifstream in (fname);
    if (!in) fatal("Can't open data file %s\n", fname);

    char buf[BUFSIZ+1];         // An input buffer for the client socket
    int nBytes;
	
	// =========================================================================
    // Install a socket in the client's file table.
	// Make an internet-transmitted, file-i/o-style, protocol-whatever plug.
    // Error checking is done in the Socket class.
    Socket client;
    client.connect(servername, port);
    client.printPeer(cout);        // Print peer

    // wait for server to acknowledge the connection. 
    nBytes = read( client.sockfd(), buf, sizeof buf );
    if ( nBytes > 0 )  cout << buf;			// the connection message.
    else fatalp("%s: Error while reading from socket.", proc );
    
    // Write lines until message is complete.
    string line;
    while (!in.eof()) {
        usleep(500000);         // Sleep time is measured in microseconds.
        getline( in, line );
    	if (!in.good() && !in.eof())
            fatal("Error reading input file %s\n", fname);

    	nBytes = write( client.sockfd(), line.data(), line.size() );
        if( nBytes < 0 ) fatalp( "%s: Error while writing to socket.", proc );
		cout << "@";
    }
    cout << endl;
    
    // all the socket connections are closed automatically 
    exit(0);
}