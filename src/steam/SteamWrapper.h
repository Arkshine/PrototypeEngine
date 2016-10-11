#ifndef ENGINE_STEAMWRAPPER_H
#define ENGINE_STEAMWRAPPER_H

#include "lib/CLibrary.h"

/**
*	@file
*	Provides wrapper functions for the Steam API. Needed since we don't have an import library to link with. - Solokiller
*/

CLibrary Steam_LoadSteamAPI( const char* const pszPath );

/**
*	Initialize the wrappers for the Steam API.
*	Unloads the library after completion.
*/
bool Steam_InitWrappers( const char* const pszPath = "", const bool bInitAPI = false );

/**
*	Initialize the wrappers for the Steam API.
*/
bool Steam_InitWrappers( CLibrary& steam_api, const bool bInitAPI = false );

#endif //ENGINE_STEAMWRAPPER_H
