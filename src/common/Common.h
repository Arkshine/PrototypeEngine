#ifndef COMMON_COMMON_H
#define COMMON_COMMON_H

#include <cassert>

#include "tier1/CommandLine.h"

//TODO: for now, map ASSERT to assert. - Solokiller
#ifndef NDEBUG
#define ASSERT assert
#else
#define ASSERT
#endif

#endif //COMMON_COMMON_H
