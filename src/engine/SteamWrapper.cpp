#include <cassert>

#include "steam_api.h"
#include "steam_gameserver.h"

#include "Logging.h"
#include "lib/CLibrary.h"

#include "SteamWrapper.h"

#define DEFINE_WRAPPER( funcName, returnType )					\
namespace														\
{																\
static returnType ( S_CALLTYPE *g_p##funcName )() = nullptr;	\
}																\
																\
S_API returnType S_CALLTYPE funcName()							\
{																\
	assert( g_p##funcName );									\
																\
	return g_p##funcName();										\
}

#define DEFINE_ACCESSOR_WRAPPER_DIFFNAME( funcName, ifaceName )		\
namespace															\
{																	\
static ifaceName* g_p##funcName = nullptr;							\
}																	\
																	\
S_API ifaceName *S_CALLTYPE funcName()								\
{																	\
	assert( g_p##funcName );										\
																	\
	return g_p##funcName;											\
}

#define DEFINE_ACCESSOR_WRAPPER( funcName )					\
DEFINE_ACCESSOR_WRAPPER_DIFFNAME( funcName, I##funcName )

//steam_api.h

DEFINE_WRAPPER( SteamAPI_Shutdown, void )
DEFINE_WRAPPER( SteamAPI_IsSteamRunning, bool )
DEFINE_WRAPPER( SteamAPI_RunCallbacks, void )

DEFINE_ACCESSOR_WRAPPER( SteamClient )
DEFINE_ACCESSOR_WRAPPER( SteamUser )
DEFINE_ACCESSOR_WRAPPER( SteamFriends )
DEFINE_ACCESSOR_WRAPPER( SteamUtils )
DEFINE_ACCESSOR_WRAPPER( SteamMatchmaking )
DEFINE_ACCESSOR_WRAPPER( SteamUserStats )
DEFINE_ACCESSOR_WRAPPER( SteamApps )
DEFINE_ACCESSOR_WRAPPER( SteamNetworking )
DEFINE_ACCESSOR_WRAPPER( SteamMatchmakingServers )
DEFINE_ACCESSOR_WRAPPER( SteamRemoteStorage )
DEFINE_ACCESSOR_WRAPPER( SteamScreenshots )
DEFINE_ACCESSOR_WRAPPER( SteamHTTP )
DEFINE_ACCESSOR_WRAPPER( SteamUnifiedMessages )
DEFINE_ACCESSOR_WRAPPER( SteamController )
DEFINE_ACCESSOR_WRAPPER( SteamUGC )
DEFINE_ACCESSOR_WRAPPER( SteamAppList )
DEFINE_ACCESSOR_WRAPPER( SteamMusic )
DEFINE_ACCESSOR_WRAPPER( SteamMusicRemote )
DEFINE_ACCESSOR_WRAPPER( SteamHTMLSurface )

namespace
{
static void ( S_CALLTYPE* g_pSteamAPI_RegisterCallback )( class CCallbackBase *pCallback, int iCallback ) = nullptr;
static void ( S_CALLTYPE* g_pSteamAPI_UnregisterCallback )( class CCallbackBase *pCallback ) = nullptr;
static void ( S_CALLTYPE* g_pSteamAPI_RegisterCallResult )( class CCallbackBase *pCallback, SteamAPICall_t hAPICall ) = nullptr;
static void ( S_CALLTYPE* g_pSteamAPI_UnregisterCallResult )( class CCallbackBase *pCallback, SteamAPICall_t hAPICall ) = nullptr;
}

S_API void S_CALLTYPE SteamAPI_RegisterCallback( class CCallbackBase *pCallback, int iCallback )
{
	assert( g_pSteamAPI_RegisterCallback );

	g_pSteamAPI_RegisterCallback( pCallback, iCallback );
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallback( class CCallbackBase *pCallback )
{
	assert( g_pSteamAPI_UnregisterCallback );

	g_pSteamAPI_UnregisterCallback( pCallback );
}

S_API void S_CALLTYPE SteamAPI_RegisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall )
{
	assert( g_pSteamAPI_RegisterCallResult );

	g_pSteamAPI_RegisterCallResult( pCallback, hAPICall );
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall )
{
	assert( g_pSteamAPI_UnregisterCallResult );

	g_pSteamAPI_UnregisterCallResult( pCallback, hAPICall );
}

//steaM_gameserver.h

#ifndef VERSION_SAFE_STEAM_API_INTERFACES

namespace
{
static bool ( S_CALLTYPE* g_pSteamGameServer_Init )( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString ) = nullptr;
}

S_API bool SteamGameServer_Init( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString )
{
	return g_pSteamGameServer_Init( unIP, usSteamPort, usGamePort, usQueryPort, eServerMode, pchVersionString );
}

//These are not accessors to call on startup! Game server is created by above function. - Solokiller
DEFINE_WRAPPER( SteamGameServer, ISteamGameServer* )
DEFINE_WRAPPER( SteamGameServerUtils, ISteamUtils* )
DEFINE_WRAPPER( SteamGameServerNetworking, ISteamNetworking* )
DEFINE_WRAPPER( SteamGameServerStats, ISteamGameServerStats* )
DEFINE_WRAPPER( SteamGameServerHTTP, ISteamHTTP* )
DEFINE_WRAPPER( SteamGameServerUGC, ISteamUGC* )
#endif

DEFINE_WRAPPER( SteamGameServer_Shutdown, void )
DEFINE_WRAPPER( SteamGameServer_RunCallbacks, void )
DEFINE_WRAPPER( SteamGameServer_BSecure, bool )
DEFINE_WRAPPER( SteamGameServer_GetSteamID, uint64 )

namespace
{
template<typename FUNCPTR>
bool Steam_InitWrapper( CLibrary& steam_api, const char* const pszFuncName, FUNCPTR& pOutPtr )
{
	auto pFunction = reinterpret_cast<FUNCPTR>( steam_api.GetFunctionAddress( pszFuncName ) );

	if( !pFunction )
	{
		Msg( "Steam_InitWrappers: Couldn't get function \"%s\"\n", pszFuncName );
		return false;
	}

	pOutPtr = pFunction;

	return true;
}

template<typename IFACE>
bool Steam_InitAccessorWrapper( CLibrary& steam_api, const char* const pszFuncName, IFACE*& pOutInstance )
{
	auto pFunction = reinterpret_cast<IFACE* ( S_CALLTYPE* )()>( steam_api.GetFunctionAddress( pszFuncName ) );

	if( !pFunction )
	{
		Msg( "Steam_InitWrappers: Couldn't get function \"%s\"\n", pszFuncName );
		return false;
	}

	pOutInstance = pFunction();

	if( !pOutInstance )
	{
		Msg( "Steam_InitWrappers: Couldn't get instance of \"%s\"\n", pszFuncName );
		return false;
	}

	return true;
}
}

#define INIT_WRAPPER( funcName )									\
if( !Steam_InitWrapper( steam_api, #funcName, g_p##funcName ) )		\
	return false

#define INIT_ACCESSOR_WRAPPER( funcName )								\
if( !Steam_InitAccessorWrapper( steam_api, #funcName, g_p##funcName ) )	\
	return false

bool Steam_InitWrappers()
{
	CLibrary steam_api;

	//The Steam API library uses standard library naming on all platforms. - Solokiller
	if( !steam_api.Load( CLibArgs( "steam_api" ) ) )
	{
		Msg( "Steam_InitWrappers: Couldn't load steam_api library\n" );
		return false;
	}

	INIT_WRAPPER( SteamAPI_Shutdown );
	INIT_WRAPPER( SteamAPI_IsSteamRunning );
	INIT_WRAPPER( SteamAPI_RunCallbacks );

	INIT_WRAPPER( SteamAPI_RegisterCallback );
	INIT_WRAPPER( SteamAPI_UnregisterCallback );
	INIT_WRAPPER( SteamAPI_RegisterCallResult );
	INIT_WRAPPER( SteamAPI_UnregisterCallResult );

	INIT_ACCESSOR_WRAPPER( SteamClient );
	INIT_ACCESSOR_WRAPPER( SteamUser );
	INIT_ACCESSOR_WRAPPER( SteamFriends );
	INIT_ACCESSOR_WRAPPER( SteamUtils );
	INIT_ACCESSOR_WRAPPER( SteamMatchmaking );
	INIT_ACCESSOR_WRAPPER( SteamUserStats );
	INIT_ACCESSOR_WRAPPER( SteamApps );
	INIT_ACCESSOR_WRAPPER( SteamNetworking );
	INIT_ACCESSOR_WRAPPER( SteamMatchmakingServers );
	INIT_ACCESSOR_WRAPPER( SteamRemoteStorage );
	INIT_ACCESSOR_WRAPPER( SteamScreenshots );
	INIT_ACCESSOR_WRAPPER( SteamHTTP );

	//These are part of later SDK releases, the version used by GoldSource doesn't support them. - Solokiller
	/*
	INIT_ACCESSOR_WRAPPER( SteamUnifiedMessages );
	INIT_ACCESSOR_WRAPPER( SteamController );
	INIT_ACCESSOR_WRAPPER( SteamUGC );
	INIT_ACCESSOR_WRAPPER( SteamAppList );
	INIT_ACCESSOR_WRAPPER( SteamMusic );
	INIT_ACCESSOR_WRAPPER( SteamMusicRemote );
	INIT_ACCESSOR_WRAPPER( SteamHTMLSurface );
	*/

#ifndef VERSION_SAFE_STEAM_API_INTERFACES
	INIT_WRAPPER( SteamGameServer_Init );
	INIT_WRAPPER( SteamGameServer );
	INIT_WRAPPER( SteamGameServerUtils );
	INIT_WRAPPER( SteamGameServerNetworking );
	INIT_WRAPPER( SteamGameServerStats );
	INIT_WRAPPER( SteamGameServerHTTP );
	//Not supported by GoldSource Steam API. - Solokiller
	/*
	INIT_WRAPPER( SteamGameServerUGC );
	*/
#endif

	INIT_WRAPPER( SteamGameServer_Shutdown );
	INIT_WRAPPER( SteamGameServer_RunCallbacks );
	INIT_WRAPPER( SteamGameServer_BSecure );
	INIT_WRAPPER( SteamGameServer_GetSteamID );

	return true;
}