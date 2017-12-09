#include "SignalHandler.h"

SignalHandler::SignalHandler() = default;

void SignalHandler::setSignals(Kid* kid) 
{
	// Empty set
	sigset_t emptySet;
	sigemptyset(&emptySet);
	
	// Construct an action
    sigAct newact;
    newact.sa_handler = handleSignals;    // handler to call
    newact.sa_mask = emptySet;            // signals to disable in handler
    newact.sa_flags = 0;                  // 0 means the default settings

    sigaction(SIGQUIT, &newact, NULL);    // install quit handler
	sigaction(SIGUSR1, &newact, NULL);	  // install start handler
}

void SignalHandler::handleSignals(int sig)
{
	if (sig == SIGQUIT) {
		quitFlag = true;
	}
	else if (sig == SIGUSR1) {
		startFlag = true;
	}	
}