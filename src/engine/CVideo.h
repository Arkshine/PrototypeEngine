#ifndef ENGINE_CVIDEO_H
#define ENGINE_CVIDEO_H

#include <SDL2/SDL.h>

class CEngine;

class CVideo final
{
public:
	CVideo() = default;

	unsigned int GetWidth() const { return m_iWidth; }

	unsigned int GetHeight() const { return m_iHeight; }

	float GetXScale() const { return m_flXScale; }

	float GetYScale() const { return m_flYScale; }

	SDL_Window* GetWindow() { return m_pWindow; }

	bool Initialize();

	void Shutdown();

	bool Run( CEngine& engine );

private:
	bool CreateGameWindow();

private:
	unsigned int m_iWidth = 640;
	unsigned int m_iHeight = 480;

	float m_flXScale = 1;
	float m_flYScale = 1;

	SDL_Window* m_pWindow = nullptr;

	SDL_GLContext m_hGLContext = nullptr;

private:
	CVideo( const CVideo& ) = delete;
	CVideo& operator=( const CVideo& ) = delete;
};

#endif //ENGINE_CVIDEO_H
