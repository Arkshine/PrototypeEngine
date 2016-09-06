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
