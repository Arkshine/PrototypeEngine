//TODO: make one header that includes the platform headers - Solokiller
#include "Platform.h"

#include <cctype>
#include <cstring>
#include <sstream>

#ifdef WIN32
#include "Shellapi.h"
#include <codecvt>
#endif

#ifdef PLAT_LINUX
#include <fstream>
#include <vector>
#endif

#ifdef _WIN32

// Required DLL entry point
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
	}
	else if( fdwReason == DLL_PROCESS_DETACH )
	{
	}
	return TRUE;
}

bool InitCommandLine( int& iOutArgC, char*** pppszArgV )
{
	const wchar_t* pszCommandLine = GetCommandLineW();

	if( !pszCommandLine )
		return false;

	int iArgC;

	wchar_t** ppszArgV = CommandLineToArgvW( pszCommandLine, &iArgC );

	if( !ppszArgV )
		return false;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	char** ppszDestArgV = new char*[ iArgC ];

	for( int iArg = 0; iArg < iArgC; ++iArg )
	{
		const std::string szArg = converter.to_bytes( ppszArgV[ iArg ] );

		ppszDestArgV[ iArg ] = new char[ szArg.length() + 1 ];

		strcpy( ppszDestArgV[ iArg ], szArg.c_str() );
	}

	LocalFree( ppszArgV );

	iOutArgC = iArgC;
	*pppszArgV = ppszDestArgV;

	return true;
}
#endif

#ifdef PLAT_LINUX
bool InitCommandLine( int& iOutArgC, char*** pppszArgV )
{
	//Read the command line from this special file.
	std::ifstream file( "/proc/self/cmdline", std::ios::in );

	if( !file.is_open() )
		return false;

	std::string szLine;

	std::vector<std::string> vecArgs;

	//Command line is a series of null terminated strings.
	while( std::getline( file, szLine, '\0' ) )
	{
		vecArgs.emplace_back( std::move( szLine ) );
	}

	//Convert to argv vector.
	const int iArgC = vecArgs.size();

	char** ppszArgV = new char*[ iArgC ];

	int iArg = 0;

	for( const auto& szArg : vecArgs )
	{
		ppszArgV[ iArg ] = new char[ szArg.length() + 1 ];

		strcpy( ppszArgV[ iArg ], szArg.c_str() );

		++iArg;
	}

	iOutArgC = iArgC;
	*pppszArgV = ppszArgV;

	return true;
}
#endif