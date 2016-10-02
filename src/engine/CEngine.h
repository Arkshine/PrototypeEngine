#ifndef ENGINE_CENGINE_H
#define ENGINE_CENGINE_H

#include <filesystem>

#include <SDL2/SDL.h>

#include "Platform.h"

class CEngine final
{
public:
	CEngine() = default;

	/**
	*	Gets the game directory that the engine mod is in.
	*	This is essentially the working directory for the game directory.
	*/
	const char* GetMyGameDir() const { return m_szMyGameDir; }

	void SetMyGameDir( const char* const pszGameDir );

	void Run( const bool bIsListenServer );

private:
	bool RunEngine( const bool bIsListenServer );

	static SDL_Window* FindEngineWindow();

	bool HostInit();

private:
	char m_szMyGameDir[ MAX_PATH ] = {};

	std::experimental::filesystem::path m_OldCWD;

	SDL_Window* m_pEngineWindow = nullptr;
	SDL_Window* m_pWindow = nullptr;

	SDL_GLContext m_hGLContext = nullptr;

private:
	CEngine( const CEngine& ) = delete;
	CEngine& operator=( const CEngine& ) = delete;
};

#endif //ENGINE_CENGINE_H
