typedef float vec_t;
typedef int HSPRITE;	// handle to a graphic

#include "vector.h"

#include "const.h"
#include "APIProxy.h"

#include "CEngine.h"

#include "EngineInterface.h"

DLLEXPORT void* ClientFactory()
{
	return nullptr;
}

DLLEXPORT void F( void* pv )
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

DLLEXPORT int Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	CEngine engine;

	return engine.Run( true );
}
