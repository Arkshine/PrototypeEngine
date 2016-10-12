#include "Common.h"
#include "Engine.h"
#include "Logging.h"
#include "IMetaLoader.h"

#include "GLUtils.h"

#include "CVideo.h"

bool CVideo::Initialize()
{
	if( g_Engine.GetLoader()->IsListenServer() )
	{
		if( !CreateGameWindow() )
			return false;
	}

	return true;
}

void CVideo::Shutdown()
{
	if( g_Engine.GetLoader()->IsListenServer() )
	{
		if( m_pWindow )
		{
			SDL_DestroyWindow( m_pWindow );
			m_pWindow = nullptr;
		}

		//SDL_Quit is handled by the loader.
	}
}

bool CVideo::Run( CEngine& engine )
{
	glEnable( GL_TEXTURE_2D );

	bool bQuit = false;

	SDL_Event event;

	while( !bQuit )
	{
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_WINDOWEVENT )
			{
				//Close if the main window receives a close request.
				if( event.window.event == SDL_WINDOWEVENT_CLOSE )
				{
					if( SDL_GetWindowID( m_pWindow ) == event.window.windowID )
					{
						bQuit = true;
					}
				}
			}
		}

		engine.RunFrame();
	}

	return true;
}

bool CVideo::CreateGameWindow()
{
	Uint32 windowFlags = /*SDL_WINDOW_HIDDEN |*/ SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	if( GetCommandLine()->GetValue( "-noborder" ) )
		windowFlags |= SDL_WINDOW_BORDERLESS;

	//OpenGL 2.0 or newer. Shader support. - Solokiller
	const int iGLMajor = 2;
	const int iGLMinor = 0;

	Msg( "Requested OpenGL version: %d.%d\n", iGLMajor, iGLMinor );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, iGLMajor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, iGLMinor );

	m_pWindow = SDL_CreateWindow( "Half-Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWidth, m_iHeight, windowFlags );

	if( !m_pWindow )
		return false;

	m_flXScale = m_iWidth / 640.0f;
	m_flYScale = m_iHeight / 480.0f;

	SDL_RaiseWindow( m_pWindow );

	m_hGLContext = SDL_GL_CreateContext( m_pWindow );

	if( !m_hGLContext )
	{
		Msg( "Couldn't create OpenGL context\n" );
		return false;
	}

	uint32_t uiMajor, uiMinor;

	if( !gl::GetContextVersion( uiMajor, uiMinor ) )
		return false;

	Msg( "OpenGL context version: %u.%u\n", uiMajor, uiMinor );

	return true;
}
