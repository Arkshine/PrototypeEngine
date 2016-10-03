#ifndef ENGINE_CENGINE_H
#define ENGINE_CENGINE_H

#include <filesystem>

#include <SDL2/SDL.h>

#include "Platform.h"

namespace vgui
{
class Panel;
}

class CEngine final
{
public:
	CEngine() = default;

	/**
	*	Gets the game directory that the engine mod is in.
	*	This is essentially the working directory for the game directory.
	*/
	const char* GetMyGameDir() const { return m_szMyGameDir; }

	SDL_Window* GetWindow() { return m_pWindow; }

	void SetMyGameDir( const char* const pszGameDir );

	void Run( const bool bIsListenServer );

private:
	bool RunEngine( const bool bIsListenServer );

	static SDL_Window* FindEngineWindow();

	bool CreateGameWindow();

	bool HostInit();

private:
	char m_szMyGameDir[ MAX_PATH ] = {};

	std::experimental::filesystem::path m_OldCWD;

	SDL_Window* m_pEngineWindow = nullptr;
	SDL_Window* m_pWindow = nullptr;

	SDL_GLContext m_hGLContext = nullptr;

	vgui::Panel* m_pRootPanel = nullptr;

private:
	CEngine( const CEngine& ) = delete;
	CEngine& operator=( const CEngine& ) = delete;
};

#endif //ENGINE_CENGINE_H
