#include <cassert>
#include <cstring>
#include <sstream>

#include "Platform.h"

#include "CCommandLine.h"

const char CCommandLine::META_PREFIX[] = "-meta:";

CCommandLine::~CCommandLine()
{
	Clear();
}

bool CCommandLine::Initialize( const size_t uiArgC, char** ppszArgV, const char* const* ppszStripCommands )
{
	Clear();

	m_Arguments.resize( uiArgC );

	for( size_t uiArg = 0; uiArg < uiArgC; ++uiArg )
	{
		m_Arguments[ uiArg ] = ppszArgV[ uiArg ];
	}

	/*	Change all -meta:<command> lines into -<command>
	*	This is a 3 step process: 
	*	find all -meta: commands, 
	*	remove the variants with the same name that don't have the meta prefix, 
	*	then rename the rest. - Solokiller
	*/
	{
		std::vector<std::string> commandsToRemove;

		//Build the list.
		for( const auto& arg : m_Arguments )
		{
			if( strncmp( arg.c_str(), META_PREFIX, META_PREFIX_LENGTH ) == 0 )
			{
				commandsToRemove.emplace_back( arg.substr( META_PREFIX_LENGTH ) );
			}
		}

		//Add additional commands to remove.
		if( ppszStripCommands )
		{
			for( auto ppszStrip = ppszStripCommands; *ppszStrip; ++ppszStrip )
			{
				commandsToRemove.emplace_back( *ppszStrip );
			}
		}

		//Remove the duplicates.
		for( const auto& command : commandsToRemove )
		{
			for( auto it = m_Arguments.begin(); it != m_Arguments.end(); )
			{
				if( *it == command )
				{
					it = m_Arguments.erase( it );

					//Check if there was a value for the command. Values have neither - or + as their first character.
					if( it != m_Arguments.end() )
					{
						const char prefix = it->at( 0 );

						//It's a value, remove it.
						if( prefix != '-' && prefix != '+' )
						{
							it = m_Arguments.erase( it );
						}
					}
				}
				else
					++it;
			}
		}

		//Rename the rest.
		for( auto it = m_Arguments.begin(); it != m_Arguments.end(); )
		{
			if( strncmp( it->c_str(), META_PREFIX, META_PREFIX_LENGTH ) == 0 )
			{
				if( it->length() > META_PREFIX_LENGTH )
				{
					( *it ) = it->substr( META_PREFIX_LENGTH );

					++it;
				}
				else
				{
					//The command was named -meta:, so just remove it.
					it = m_Arguments.erase( it );
				}
			}
			else
			{
				++it;
			}
		}
	}

	m_Arguments.shrink_to_fit();

	std::stringstream stream;

	for( size_t uiArg = 0; uiArg < m_Arguments.size(); ++uiArg )
	{
		const char* pszArg = m_Arguments[ uiArg ].c_str();

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

	m_szCommandLine = stream.str();

	//The last character will be a space unless there were no commandline arguments, so don't copy that. - Solokiller
	if( !m_szCommandLine.empty() )
	{
		m_szCommandLine.resize( m_szCommandLine.size() - 1 );
	}

	m_szCommandLine.shrink_to_fit();

	return true;
}

const char* CCommandLine::GetCommandLineString() const
{
	return m_szCommandLine.c_str();
}

size_t CCommandLine::GetArgumentCount() const
{
	return m_Arguments.size();
}

const char* CCommandLine::GetArgument( const size_t uiArgument ) const
{
	assert( uiArgument < m_Arguments.size() );

	if( uiArgument >= m_Arguments.size() )
		return nullptr;

	return m_Arguments[ uiArgument ].c_str();
}

const char* CCommandLine::operator[]( const size_t uiArgument ) const
{
	return GetArgument( uiArgument );
}

size_t CCommandLine::IndexOf( const char* const pszKey ) const
{
	for( size_t uiArg = 0; uiArg < m_Arguments.size(); ++uiArg )
	{
		if( stricmp( pszKey, m_Arguments[ uiArg ].c_str() ) == 0 )
		{
			return uiArg;
		}
	}

	return INVALID_INDEX;
}

const char* CCommandLine::GetValue( const char* const pszKey ) const
{
	const size_t uiIndex = IndexOf( pszKey );

	if( uiIndex == INVALID_INDEX )
		return nullptr;

	if( uiIndex + 1 < m_Arguments.size() )
		return m_Arguments[ uiIndex + 1 ].c_str();

	return "";
}

void CCommandLine::Clear()
{
	m_szCommandLine.clear();
	m_Arguments.clear();
}
