#include "Platform.h"

#include "CEngine.h"

bool CEngine::Run( const bool bIsListenServer )
{
	if( !engine::InitCommandLine() )
		return false;

	return true;
}
