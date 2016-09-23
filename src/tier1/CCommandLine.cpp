#include <cassert>
#include <cstring>
#include <sstream>

#include "Platform.h"

#include "CCommandLine.h"

CCommandLine::~CCommandLine()
{
	Clear();
}

bool CCommandLine::Initialize( const int iArgC, char** ppszArgV, const bool bTakeOwnership )
{
	Clear();

	m_iArgC = iArgC;

	if( bTakeOwnership )
	{
		m_ppszArgV = ppszArgV;
	}
	else
	{
		m_ppszArgV = new char*[ iArgC ];

		for( int iArg = 0; iArg < iArgC; ++iArg )
		{
			const size_t uiLength = strlen( ppszArgV[ iArg ] );

			m_ppszArgV[ iArg ] = new char[ uiLength + 1 ];

			strcpy( m_ppszArgV[ iArg ], ppszArgV[ iArg ] );
		}
	}

	std::stringstream stream;

	for( int iArg = 0; iArg < iArgC; ++iArg )
	{
		const char* pszArg = ppszArgV[ iArg ];

		//TODO: refactor into function. - Solokiller
		const bool bHasWhitespace = []( const char* pszString ) -> bool
		{
			while( *pszString )
			{
				if( isspace( *pszString ) )
					return true;

				++pszString;
			}

			return false;
		}( pszArg );

		if( bHasWhitespace )
		{
			stream << '\"';
		}

		stream << pszArg;

		if( bHasWhitespace )
		{
			stream << '\"';
		}

		stream << ' ';
	}

	std::string szCommandline = stream.str();

	//The last character will be a space unless there were no commandline arguments, so don't copy that. - Solokiller
	const size_t uiLength = !szCommandline.empty() ? szCommandline.length() : 1;

	m_pszCommandLine = new char[ uiLength ];

	strncpy( m_pszCommandLine, szCommandline.c_str(), uiLength );

	m_pszCommandLine[ uiLength - 1 ] = '\0';

	return true;
}

const char* CCommandLine::GetCommandLineString() const
{
	return m_pszCommandLine;
}

int CCommandLine::GetArgumentCount() const
{
	return m_iArgC;
}

const char* CCommandLine::GetArgument( const int iArgument ) const
{
	assert( iArgument >= 0 && iArgument < m_iArgC );

	if( iArgument < 0 || iArgument >= m_iArgC )
		return nullptr;

	return m_ppszArgV[ iArgument ];
}

const char* CCommandLine::operator[]( const int iArgument ) const
{
	return GetArgument( iArgument );
}

int CCommandLine::IndexOf( const char* const pszKey ) const
{
	for( int iArg = 0; iArg < m_iArgC; ++iArg )
	{
		if( stricmp( pszKey, m_ppszArgV[ iArg ] ) == 0 )
		{
			return iArg;
		}
	}

	return INVALID_INDEX;
}

const char* CCommandLine::GetValue( const char* const pszKey ) const
{
	const int iIndex = IndexOf( pszKey );

	if( iIndex == INVALID_INDEX )
		return nullptr;

	if( iIndex + 1 < m_iArgC )
		return m_ppszArgV[ iIndex + 1 ];

	return "";
}

void CCommandLine::Clear()
{
	if( m_ppszArgV )
	{
		for( int iArg = 0; iArg < m_iArgC; ++iArg )
		{
			delete[] m_ppszArgV[ iArg ];
		}

		delete[] m_ppszArgV;
		m_ppszArgV = nullptr;
		m_iArgC = 0;
	}

	if( m_pszCommandLine )
	{
		delete[] m_pszCommandLine;
		m_pszCommandLine = nullptr;
	}
}
