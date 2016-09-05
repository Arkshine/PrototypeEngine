#ifndef ENGINE_ENGINEINTERFACE_H
#define ENGINE_ENGINEINTERFACE_H

#include "Platform.h"

#ifdef _WIN32
//The one function in Half-Life 1 that uses stdcall. Be aware of this. - Solokiller
#define GIVEFNPTRS_DLLEXPORT __stdcall
#else
#define GIVEFNPTRS_DLLEXPORT DLLEXPORT
#endif

extern "C"
{
	DLLEXPORT void* ClientFactory();

	void DLLEXPORT F( void* pv );

	int DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );

	void GIVEFNPTRS_DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals );
}

#endif //ENGINE_ENGINEINTERFACE_H
