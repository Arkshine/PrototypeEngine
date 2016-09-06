#include <cstdarg>
#include <cstdio>

#include <SDL2/SDL.h>

#include "Logging.h"

void UTIL_ShowMessageBox( const char* const pszMessage, const char* const pszCaption, const LogType logType )
{
	Uint32 type;

	switch( logType )
	{
	default:
	case LogType::INFO:		type = SDL_MESSAGEBOX_INFORMATION; break;
	case LogType::WARNING:	type = SDL_MESSAGEBOX_WARNING; break;
	case LogType::ERROR:	type = SDL_MESSAGEBOX_ERROR; break;
	}

	SDL_ShowSimpleMessageBox( type, pszCaption, pszMessage, nullptr );
}

void Msg( const char* const pszFormat, ... )
{
	char szBuffer[ 4096 ];

	va_list list;

	va_start( list, pszFormat );

	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );

	va_end( list );

	//Log to stdout for now. - Solokiller
	printf( "%s", szBuffer );
}

void Warning( const char* const pszFormat, ... )
{
	char szBuffer[ 4096 ];

	va_list list;

	va_start( list, pszFormat );

	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );

	va_end( list );

	//Log to stdout for now. - Solokiller
	printf( "Warning: %s", szBuffer );
}
