#ifndef TIER1_COMMANDLINE_H
#define TIER1_COMMANDLINE_H

/**
*	@file
*	Contains the export command line getter function.
*/

#include "Tier1.h"
#include "ICommandLine.h"

#undef GetCommandLine

/**
*	@return The command line.
*/
TIER1_DLLEXPORT ICommandLine* GetCommandLine();

#endif //TIER1_COMMANDLINE_H
