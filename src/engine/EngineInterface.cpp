typedef float vec_t;
typedef int HSPRITE;	// handle to a graphic

typedef struct entvars_s entvars_t;
typedef struct edict_s edict_t;
typedef struct globalvars_s globalvars_t;

#include "vector.h"

#include "const.h"
#include "APIProxy.h"
#include "eiface.h"

#include "Engine.h"

#include "EngineInterface.h"

DLLEXPORT void* ClientFactory()
{
	return nullptr;
}

void DLLEXPORT F( void* pv )
{
	cldll_func_t *pcldll_func = ( cldll_func_t * ) pv;

	cldll_func_t cldll_func =
	{
		Initialize,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		ClientFactory,	//NOTE: Needed for the engine to query CreateInterface for interface instantiation. - Solokiller
	};

	*pcldll_func = cldll_func;
}

int DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	g_Engine.SetMyGameDir( pEnginefuncs->pfnGetGameDirectory() );

	g_Engine.Run( true );

	return false;
}

extern "C"
{
void GIVEFNPTRS_DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals )
{
	{
		char szGameDir[ MAX_PATH ];

		pengfuncsFromEngine->pfnGetGameDir( szGameDir );

		g_Engine.SetMyGameDir( szGameDir );
	}

	g_Engine.Run( false );
}
}

#ifdef WIN32
//See post VS 2015 update 3 delayimp.h for the reason why this has to be defined. - Solokiller
#define DELAYIMP_INSECURE_WRITABLE_HOOKS
#include <delayimp.h>

FARPROC WINAPI DelayHook(
	unsigned        dliNotify,
	PDelayLoadInfo  pdli
)
{
	if( dliNotify == dliNotePreLoadLibrary )
	{
		if( strcmp( pdli->szDll, "Tier1.dll" ) == 0 )
		{
			char szPath[ MAX_PATH ];

			if( !( *g_Engine.GetMyGameDir() ) )
				return nullptr;

			const int iResult = snprintf( szPath, sizeof( szPath ), "%s/%s", g_Engine.GetMyGameDir(), pdli->szDll );

			if( iResult < 0 || static_cast<size_t>( iResult ) >= sizeof( szPath ) )
				return nullptr;

			HMODULE hLib = LoadLibraryA( szPath );

			return ( FARPROC ) hLib;
		}
	}

	return nullptr;
}

ExternC PfnDliHook __pfnDliNotifyHook2 = DelayHook;

ExternC PfnDliHook   __pfnDliFailureHook2 = nullptr;
#endif
