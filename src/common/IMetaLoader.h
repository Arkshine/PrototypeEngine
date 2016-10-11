#ifndef COMMON_IMETALOADER_H
#define COMMON_IMETALOADER_H

#include <cstddef>

#include "SDL2/SDL.h"

#include "interface.h"

/**
*	@defgroup MetaLoader Meta library loader
*
*	@{
*/

/**
*	Interface to the meta loader. Provides basic information about the directory in which the loader resides, and allows other libraries to be loaded.
*/
class IMetaLoader : public IBaseInterface
{
public:
	/**
	*	Gets the game directory in which this loader resides.
	*	@param pszDest Destination buffer.
	*	@param uiSizeInCharacters Size of the buffer, in characters.
	*	@return If successful, returns pszDest. Otherwise, returns null.
	*/
	virtual const char* GetGameDirectory( char* pszDest, size_t uiSizeInCharacters ) const = 0;

	/**
	*	Gets the original engine window.
	*/
	virtual SDL_Window* GetEngineWindow() const = 0;

	/**
	*	@return Whether this is a listen server or a dedicated server.
	*/
	virtual bool IsListenServer() const = 0;
};

/**
*	Interface name.
*/
#define IMETALOADER_NAME "IMetaLoaderV001"

/** @} */

#endif //COMMON_IMETALOADER_H
