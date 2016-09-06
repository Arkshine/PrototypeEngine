typedef float vec_t;
typedef int HSPRITE;	// handle to a graphic

typedef struct entvars_s entvars_t;
typedef struct edict_s edict_t;
typedef struct globalvars_s globalvars_t;

#include "vector.h"

#include "const.h"
#include "APIProxy.h"
#include "eiface.h"

#include "CEngine.h"

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
	CEngine engine;

	engine.Run( true );

	return false;
}

extern "C"
{
void GIVEFNPTRS_DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals )
{
	CEngine engine;

	engine.Run( false );
}
}
