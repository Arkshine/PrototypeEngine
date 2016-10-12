#ifndef ENGINE_CVIDEO_H
#define ENGINE_CVIDEO_H

#include <SDL2/SDL.h>

class CEngine;

/**
*	Manages video functionality.
*/
class CVideo final
{
public:
	CVideo() = default;

	/**
	*	@return The width of the main window.
	*/
	unsigned int GetWidth() const { return m_iWidth; }

	/**
	*	@return The height of the main window.
	*/
	unsigned int GetHeight() const { return m_iHeight; }

	/**
	*	@return The X scale to use when scaling images for resolution dependent usage.
	*/
	float GetXScale() const { return m_flXScale; }

	/**
	*	@return The Y scale to use when scaling images for resolution dependent usage.
	*/
	float GetYScale() const { return m_flYScale; }

	/**
	*	@return The main window.
	*/
	SDL_Window* GetWindow() { return m_pWindow; }

	/**
	*	Initializes the video system.
	*	@return Whether initialization succeeded.
	*/
	bool Initialize();

	/**
	*	Shuts down the video system.
	*/
	void Shutdown();

	/**
	*	Runs the main loop. Calls engine.RunFrame every frame.
	*	@return Whether the loop completed successfully.
	*/
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
