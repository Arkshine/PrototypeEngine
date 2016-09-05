#include "Platform.h"

#include "CCommandLine.h"

#include "Common.h"

#include "CEngine.h"

namespace
{
static CCommandLine g_CommandLine;
}

//TODO: should be moved. - Solokiller
ICommandLine* GetCommandLine()
{
	return &g_CommandLine;
}

bool CEngine::Run( const bool bIsListenServer )
{
	int iArgC;
	char** ppszArgV;

	if( !InitCommandLine( iArgC, &ppszArgV ) )
		return false;

	if( !GetCommandLine()->Initialize( iArgC, ppszArgV, true ) )
		return false;

	return true;
}
