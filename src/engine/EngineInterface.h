#ifndef ENGINE_ENGINEINTERFACE_H
#define ENGINE_ENGINEINTERFACE_H

#include "Platform.h"

extern "C"
{
	DLLEXPORT void* ClientFactory();

	DLLEXPORT void F( void* pv );

	DLLEXPORT int Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
}

#endif //ENGINE_ENGINEINTERFACE_H
