#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "CEngine.h"

#include "console/CCommandBuffer.h"
#include "console/CCVarSystem.h"

/**
*	The command buffer.
*/
extern CCommandBuffer g_CommandBuffer;
extern cvar::CCVarSystem g_CVar;

extern CEngine g_Engine;

#endif //ENGINE_ENGINE_H
