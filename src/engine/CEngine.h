#ifndef ENGINE_CENGINE_H
#define ENGINE_CENGINE_H

#include <SDL2/SDL.h>

class CEngine final
{
public:
	CEngine() = default;

	void Run( const bool bIsListenServer );

private:
	bool RunEngine( const bool bIsListenServer );

private:
	static SDL_Window* FindEngineWindow();

private:
	SDL_Window* m_pEngineWindow = nullptr;
	SDL_Window* m_pWindow = nullptr;

private:
	CEngine( const CEngine& ) = delete;
	CEngine& operator=( const CEngine& ) = delete;
};

#endif //ENGINE_CENGINE_H
