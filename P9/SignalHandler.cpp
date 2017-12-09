#include "SignalHandler.h"

SignalHandler::SignalHandler() = default;

void SignalHandler::setSignals(Kid* kid) 
{
	cout << "Installing signal handler for "<< kid->name << endl;
	// ---------------------------------------------
	// Set containing the two signals
	/*sigset_t sigSet;
	sigemptyset(&sigSet);
	sigaddset(&sigSet, SIGUSR1);
	sigaddset(&sigSet, SIGQUIT);*/
	
	// Empty set
	sigset_t emptySet;
	sigemptyset(&emptySet);
	
	// Construct an action
    sigAct newact;
    newact.sa_handler = handleSignals;    // handler to call
    newact.sa_mask = emptySet;            // signals to disable in handler
    newact.sa_flags = 0;                  // 0 means the default settings

    //sigaction(SIGQUIT, &newact, NULL);    // install new handler for SIGINT
	sigaction(SIGUSR1, &newact, NULL);
	cout << "Signal handler installed for "<< kid->name << endl;
}

void SignalHandler::handleSignals(int sig)
{
	if (sig == SIGQUIT) {
		quitFlag = true;
		cout << "Received sigquit" << endl;
	}
	else if (sig == SIGUSR1) {
		startFlag = true;
		cout << "Received sigusr1" << endl;
	}	
}