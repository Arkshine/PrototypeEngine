#include <limits>

#include <gl/glew.h>

#include "Platform.h"

#include "CNetworkBuffer.h"
#include "Common.h"
#include "Engine.h"
#include "GLUtils.h"
#include "Logging.h"

#include "CEngine.h"

void CEngine::SetMyGameDir( const char* const pszGameDir )
{
	strncpy( m_szMyGameDir, pszGameDir, sizeof( m_szMyGameDir ) );
	m_szMyGameDir[ sizeof( m_szMyGameDir ) - 1 ] = '\0';
}

void CEngine::Run( const bool bIsListenServer )
{
	const bool bResult = RunEngine( bIsListenServer );

	exit( bResult ? EXIT_SUCCESS : EXIT_FAILURE );
}

bool CEngine::RunEngine( const bool bIsListenServer )
{
	if( !( *m_szMyGameDir ) )
	{
		UTIL_ShowMessageBox( "No game directory set", "Error", LogType::ERROR );
		return false;
	}

	//Set the working directory to the game directory that the engine is running in.
	//Needed so asset loading works. Note that any mods that rely on ./valve to exist will break. - Solokiller
	//TODO: Consider adding symlinks or requiring the copying of assets to game directories. Windows won't allow it when targeting XP.
	{

		m_OldCWD = std::experimental::filesystem::current_path();

		const std::string szPath = std::string( "./" ) + m_szMyGameDir;

		std::error_code error;

		std::experimental::filesystem::current_path( std::experimental::filesystem::path( szPath ), error );

		if( error )
		{
			UTIL_ShowMessageBox( ( std::string( "Error while setting current working directory \"" ) + szPath + "\": " + error.message() ).c_str(), "Error", LogType::ERROR );
			return false;
		}
	}

	{
		int iArgC;
		char** ppszArgV;

		if( !InitCommandLine( iArgC, &ppszArgV ) )
			return false;

		if( !GetCommandLine()->Initialize( iArgC, ppszArgV, true ) )
			return false;
	}

	//TODO: until we can draw the console onscreen, use a console window. - Solokiller
	AllocConsole();
	freopen( "CONOUT$", "w", stdout );

	if( bIsListenServer )
	{
		//Already done by GoldSource. - Solokiller
		/*
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			return false;
		}
		*/

		//Find the engine's window and make it invisible.
		m_pEngineWindow = FindEngineWindow();

		if( m_pEngineWindow )
		{
			SDL_HideWindow( m_pEngineWindow );
		}

		if( !CreateGameWindow() )
			return false;
	}

	Msg( "HostInit\n" );

	if( HostInit() )
	{
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
		}
	}
	else
	{
		UTIL_ShowMessageBox( "Error initializing host", "Fatal Error", LogType::ERROR );
	}

	if( bIsListenServer )
	{
		if( m_pWindow )
		{
			SDL_DestroyWindow( m_pWindow );
			m_pWindow = nullptr;
		}

		SDL_Quit();
	}

	return true;
}

SDL_Window* CEngine::FindEngineWindow()
{
	for( Uint32 uiID = 0; uiID < std::numeric_limits<Uint32>::max(); ++uiID )
	{
		SDL_Window* pWindow = SDL_GetWindowFromID( uiID );

		if( pWindow )
			return pWindow;
	}

	return nullptr;
}

bool CEngine::CreateGameWindow()
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

	m_pWindow = SDL_CreateWindow( "Half-Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, windowFlags );

	if( !m_pWindow )
		return false;

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

bool CEngine::HostInit()
{
	CNetworkBuffer::InitMasks();

	if( !g_CVar.Initialize() )
		return false;

	if( !g_CommandBuffer.Initialize( &g_CVar ) )
		return false;

	return true;
}
