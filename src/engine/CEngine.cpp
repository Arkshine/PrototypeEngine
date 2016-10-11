#include <cmath>
#include <cstring>
#include <limits>

#include <gl/glew.h>

#include <VGUI_App.h>
#include <VGUI_Panel.h>
#include <VGUI_BitmapTGA.h>
#include <VGUI_ImagePanel.h>
#include <VGUI1/VGUI_RDBitmapTGA.h>

#include "Platform.h"

#include "CNetworkBuffer.h"
#include "Common.h"
#include "Engine.h"
#include "FilePaths.h"
#include "GLUtils.h"
#include "IMetaLoader.h"
#include "interface.h"
#include "Logging.h"
#include "SteamWrapper.h"

#include "FileSystem2.h"
#include "CFileSystemWrapper.h"

#include "VGUI1/vgui_loadtga.h"

#include "CEngine.h"

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CEngine, IMetaTool, DEFAULT_IMETATOOL_NAME, g_Engine );

void CEngine::SetMyGameDir( const char* const pszGameDir )
{
	strncpy( m_szMyGameDir, pszGameDir, sizeof( m_szMyGameDir ) );
	m_szMyGameDir[ sizeof( m_szMyGameDir ) - 1 ] = '\0';
}

bool CEngine::Startup( IMetaLoader& loader, CreateInterfaceFn* pFactories, const size_t uiNumFactories )
{
	m_pLoader = &loader;

	if( !m_pLoader->GetGameDirectory( m_szMyGameDir, sizeof( m_szMyGameDir ) ) )
		return false;

	if( !( *m_szMyGameDir ) )
	{
		UTIL_ShowMessageBox( "No game directory set", "Error", LogType::ERROR );
		return false;
	}

	for( size_t uiIndex = 0; uiIndex < uiNumFactories; ++uiIndex )
	{
		auto factory = pFactories[ uiIndex ];

		if( !g_pFileSystem )
		{
			g_pFileSystem = static_cast<IFileSystem2*>( factory( FILESYSTEM2_INTERFACE_VERSION, nullptr ) );
		}
	}

	if( !g_pFileSystem )
	{
		Msg( "Couldn't instantiate the filesystem\n" );
		return false;
	}

	//Load the original filesystem and overwrite its filesystem's vtable with one that points to ours.
	//Note: if the original engine regains control, it might try to use preexisting handles. Don't let that happen. - Solokiller
	{
		CLibrary fileSystem;

		if( !fileSystem.Load( CLibArgs( "filesystem_stdio" ).DisablePrefixes( true ) ) )
		{
			Msg( "Couldn't load filesystem_stdio\n" );
			return false;
		}

		auto filesystemFactory = reinterpret_cast<CreateInterfaceFn>( fileSystem.GetFunctionAddress( CREATEINTERFACE_PROCNAME ) );

		if( !filesystemFactory )
		{
			Msg( "Couldn't find filesystem_stdio factory\n" );
			return false;
		}

		auto pFileSystem = static_cast<IFileSystem*>( filesystemFactory( FILESYSTEM_INTERFACE_VERSION, nullptr ) );

		if( !pFileSystem )
		{
			Msg( "Couldn't instantiate the filesystem from filesystem_stdio\n" );
			return false;
		}

		CFileSystemWrapper wrapper;

		//Don't try this at home.
		memcpy( pFileSystem, &wrapper, sizeof( IFileSystem ) );
	}

	return true;
}

bool CEngine::Run()
{
	if( m_pLoader->IsListenServer() )
	{
		if( !CreateGameWindow() )
			return false;
	}

	Msg( "HostInit\n" );

	glEnable( GL_TEXTURE_2D );

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

			glClearColor( 0, 0, 0, 1 );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glViewport( 0, 0, m_iWidth, m_iHeight );

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();

			glOrtho( 0.0f, ( float ) m_iWidth, ( float ) m_iHeight, 0.0f, 1.0f, -1.0f );

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			glDisable( GL_CULL_FACE );
			glDisable( GL_BLEND );
			glDisable( GL_DEPTH_TEST );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

			vgui::App::getInstance()->externalTick();
			m_pRootPanel->repaintAll();
			m_pRootPanel->paintTraverse();

			//g_pVGUI1Surface->swapBuffers();

			glPopMatrix();
		}
	}
	else
	{
		UTIL_ShowMessageBox( "Error initializing host", "Fatal Error", LogType::ERROR );
	}

	return true;
}

void CEngine::Shutdown()
{
	if( m_pLoader->IsListenServer() )
	{
		if( m_pWindow )
		{
			SDL_DestroyWindow( m_pWindow );
			m_pWindow = nullptr;
		}

		//SDL_Quit is handled by the loader.
	}
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

bool CEngine::HostInit()
{
	CNetworkBuffer::InitMasks();

	if( !g_CVar.Initialize() )
		return false;

	if( !g_CommandBuffer.Initialize( &g_CVar ) )
		return false;

	auto pApp = vgui::App::getInstance();

	pApp->reset();

	m_pRootPanel = new vgui::Panel( 0, 0, m_iWidth, m_iHeight );

	m_pRootPanel->setPaintBorderEnabled( false );
	m_pRootPanel->setPaintBackgroundEnabled( false );
	m_pRootPanel->setPaintEnabled( false );

	vgui::Scheme* pScheme = pApp->getScheme();

	m_pRootPanel->setCursor( pScheme->getCursor( vgui::Scheme::scu_none ) );

	g_pVGUI1Surface = new CVGUI1Surface( m_pRootPanel );

	CreateMainMenuBackground();

	return true;
}

void CEngine::CreateMainMenuBackground()
{
	auto pBackground = new vgui::Panel();

	pBackground->setParent( m_pRootPanel );

	int wide, tall;
	
	m_pRootPanel->getSize( wide, tall );

	pBackground->setSize( wide, tall );

	const float flXScale = m_iWidth / 800.0f;
	const float flYScale = m_iHeight / 600.0f;
	const int iXOffsetScale = static_cast<int>( ceil( 256 * flXScale ) );
	const int iYOffsetScale = static_cast<int>( ceil( 256 * flYScale ) );

	char szFileName[ MAX_PATH ];

	for( size_t uiIndex = 0; uiIndex < 4 * 3; ++uiIndex )
	{
		snprintf( szFileName, sizeof( szFileName ), "resource/background/800_%u_%c_loading.tga", ( uiIndex / 4 ) + 1, 'a' + ( uiIndex % 4 ) );

		auto pImage = static_cast<vgui::RDBitmapTGA*>( vgui_LoadTGA( szFileName, true, true ) );

		//Resize the images so they fit the default resolution better. The resolution scaling will take care of the rest. - Solokiller
		pImage->SetXScale( 640 / 800.0f );
		pImage->SetYScale( 480 / 600.0f );

		auto pImagePanel = new vgui::ImagePanel( pImage );

		pImagePanel->setParent( pBackground );

		pImagePanel->setPos( iXOffsetScale * ( uiIndex % 4 ), iYOffsetScale * ( uiIndex / 4 ) );
	}
}
