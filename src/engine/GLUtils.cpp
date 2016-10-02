#include <sstream>
#include <string>

#include <GL/glew.h>

#include "Logging.h"

#include "GLUtils.h"

namespace gl
{
bool GetContextVersion( uint32_t& uiOutMajor, uint32_t& uiOutMinor )
{
	uiOutMajor = 1;
	uiOutMinor = 0;

	const char* pszVersion = reinterpret_cast<const char*>( glGetString( GL_VERSION ) );

	if( !pszVersion )
	{
		Msg( "gl::GetContextVersion: Couldn't query OpenGL version info\n" );
		return false;
	}

	std::string szVersion( pszVersion );

	//Vendor specific information follows after a space, so trim it. - Solokiller
	const auto space = szVersion.find( ' ' );

	if( space != std::string::npos )
		szVersion = szVersion.substr( 0, space );

	std::stringstream stream( szVersion );

	std::string szToken;

	if( !std::getline( stream, szToken, '.' ) )
	{
		Msg( "gl::GetContextVersion: Invalid version string\n" );
		return false;
	}

	const uint32_t uiMajor = std::stoul( szToken );

	if( !std::getline( stream, szToken, '.' ) )
	{
		Msg( "gl::GetContextVersion: Invalid version string\n" );
		return false;
	}

	uiOutMajor = uiMajor;
	uiOutMinor = std::stoul( szToken );

	return true;
}
}