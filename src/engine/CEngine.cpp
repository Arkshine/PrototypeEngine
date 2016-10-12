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
#include "IMetaLoader.h"
#include "interface.h"
#include "Logging.h"
#include "steam/SteamWrapper.h"

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

	m_steam_api = Steam_LoadSteamAPI( filepaths::BIN_DIR );

	m_bSteamAPIInitialized = Steam_InitWrappers( m_steam_api, true );

	if( !m_bSteamAPIInitialized )
	{
		return false;
	}

	if( !g_SteamAPIContext.Init() )
	{
		UTIL_ShowMessageBox( "Failed to initialize Steam API Context. Exiting...\n", "Fatal Error", LogType::ERROR );
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

	if( !g_Video.Initialize() )
	{
		return false;
	}

	Msg( "HostInit\n" );

	if( !HostInit() )
	{
		UTIL_ShowMessageBox( "Error initializing host", "Fatal Error", LogType::ERROR );
		return false;
	}

	return true;
}

bool CEngine::Run()
{
	return g_Video.Run( *this );
}

void CEngine::Shutdown()
{
	g_Video.Shutdown();

	if( m_steam_api.IsLoaded() )
	{
		if( m_bSteamAPIInitialized )
			SteamAPI_Shutdown();

		m_steam_api.Free();
	}
}

void CEngine::RunFrame()
{
	RenderFrame();
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

	m_pRootPanel = new vgui::Panel( 0, 0, g_Video.GetWidth(), g_Video.GetHeight() );

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

	const float flXScale = g_Video.GetWidth() / 800.0f;
	const float flYScale = g_Video.GetHeight() / 600.0f;
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

void CEngine::RenderFrame()
{
	RenderVGUI1();
}

void CEngine::RenderVGUI1()
{
	glClearColor( 0, 0, 0, 1 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glViewport( 0, 0, g_Video.GetWidth(), g_Video.GetHeight() );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho( 0.0f, ( float ) g_Video.GetWidth(), ( float ) g_Video.GetHeight(), 0.0f, 1.0f, -1.0f );

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
