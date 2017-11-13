#include "tools.hpp"
#include "FileWrappers.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <openssl/sha.h>
#define BUFSIZE 4096
// -----------------------------------------------------------------------------
// Stats functions
void Stats::print(ostream& out)
{
	out << "I-node " << inode() << " links " << links() << endl;	
}

// -------------------------------------------------------------------
// Direntry functions
void Direntry::printVerbose(ostream& out)
{
	string typeString;
	switch (type()) {
		case DT_REG:
			typeString = "file"; break;
		case DT_DIR:
			typeString = "directory"; break;
		case DT_LNK:
			typeString = "link"; break;
		default:
			typeString = "unknown"; break;
	}
	out << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
	cout << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
}

// -------------------------------------------------------------------
// FileID functions
FileID::FileID(string name, string abs, ino_t num, int len, int links)
{
	pathName = name;
	absolutePath = abs;
	inodeNum = num;
	fileLen = len;
	nLinks = links;
}

void FileID::calculateSHA256(/*string file*/)
{
	// Setup =============================================================
    // (argc!=2) fatal("usage: $s fname", argv[0]);
    string file = this->absolutePath;
    ifstream in (file);
    if (!in) fatal("can't open %s for reading", file);
    
    SHA_CTX ctx;   // A structure of state info used in computing SHA.
    if (!SHA1_Init(&ctx)) fatal("SHA1_Init failure");
    
    // Fingerprinting ====================================================
    char buf[BUFSIZE];
    
    for (;;) {
        in.read( buf, BUFSIZE );
        size_t bytes_read = in.gcount();
        if (bytes_read == 0) break;
        // Update the state by processing the next block of data.
        if (!SHA1_Update( &ctx, buf, bytes_read) ) fatal("SHA1_Update failure");
    }
    
    unsigned char md[SHA_DIGEST_LENGTH];
    // Terminate SHA algorithm and extract the answer from the context. 
    if (!SHA1_Final(md, &ctx)) fatal("SHA1_Final failure");
	strcpy((char*)this->fingerprint, (char*)md);
    //return md;
    /*cout <<"The SHA1 message digest is: ";
    for (int k=0; k<SHA_DIGEST_LENGTH; k++) {
        cout <<hex <<setfill('0') <<setw(2) <<(int)md[k];
    }
    cout <<"\n\n" <<dec;*/
}