#include <limits>

#include "Platform.h"

#include "CCommandLine.h"

#include "CNetworkBuffer.h"
#include "Common.h"
#include "Engine.h"
#include "Logging.h"

#include "CEngine.h"

namespace
{
static CCommandLine g_CommandLine;
}

//TODO: should be moved. - Solokiller
ICommandLine* GetCommandLine()
{
	return &g_CommandLine;
}

void CEngine::Run( const bool bIsListenServer )
{
	const bool bResult = RunEngine( bIsListenServer );

	exit( bResult ? EXIT_SUCCESS : EXIT_FAILURE );
}

bool CEngine::RunEngine( const bool bIsListenServer )
{
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

		Uint32 windowFlags = /*SDL_WINDOW_HIDDEN |*/ SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

		if( GetCommandLine()->GetValue( "-noborder" ) )
			windowFlags |= SDL_WINDOW_BORDERLESS;

		m_pWindow = SDL_CreateWindow( "Half-Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, windowFlags );

		if( !m_pWindow )
			return false;

		SDL_RaiseWindow( m_pWindow );
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

bool CEngine::HostInit()
{
	CNetworkBuffer::InitMasks();

	if( !g_CommandBuffer.Initialize() )
		return false;

	return true;
}
