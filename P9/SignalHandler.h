#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include "tools.hpp"
#include "Shared.h"
#include "Kid.h"
#include <signal.h>
class Kid;
class SignalHandler 
{
	public:
		SignalHandler();
		void setSignals(Kid*);
		static void handleSignals(int);
};
#endif