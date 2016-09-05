#ifndef COMMON_COMMON_H
#define COMMON_COMMON_H

#include "ICommandLine.h"

#undef GetCommandLine

/**
*	@return The command line.
*	TODO: should be in a load time linked library. - Solokiller
*/
ICommandLine* GetCommandLine();

#endif //COMMON_COMMON_H
