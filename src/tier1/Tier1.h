#ifndef TIER1_TIER1_H
#define TIER1_TIER1_H

/**
*	@file
*	Tier1 library definitions file. Defines export macro.
*/

#include "Platform.h"

#ifdef Tier1_EXPORTS
#define TIER1_DLLEXPORT DLLEXPORT
#else
#define TIER1_DLLEXPORT DLLIMPORT
#endif

#endif //TIER1_TIER1_H
