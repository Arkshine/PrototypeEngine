#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>

#include "Logging.h"
#include "Platform.h"

#include "ConCommand_t.h"
#include "cvardef.h"

#include "Engine.h"

#include "CCVarSystem.h"

namespace
{
static void Cmd_Echo_f()
{
	for( int iArg = 1; iArg < g_CVar.GetArgC(); ++iArg )
	{
		Msg( "%s ", g_CVar.GetArgV( iArg ) );
	}

	Msg( "\n" );
}

static void Cmd_Wait_f()
{
	g_CommandBuffer.SetWait( true );
}
}

namespace cvar
{
CCVarSystem::CCVarSystem()
{
}

CCVarSystem::~CCVarSystem()
{
}

bool CCVarSystem::Initialize()
{
	AddCommand( "echo", &::Cmd_Echo_f );
	AddCommand( "wait", &::Cmd_Wait_f );

	//TODO: add Cmd_Init functions. - Solokiller

	return true;
}

bool CCVarSystem::AddCommand( const char* const pszName, xcommand_t pFunction, const uint32_t flags )
{
	assert( pszName );
	assert( pFunction );

	if( strpbrk( pszName, " \t\"\';" ) )
	{
		Warning( "CCVarSystem::AddCommand: Attempted to add command with invalid characters in its name \"%s\"!\n", pszName );
		return false;
	}

	if( FindCVar( pszName ) )
	{
		Warning( "CCVarSystem::AddCommand: \"%s\" already defined as a cvar\n", pszName );
		return false;
	}

	if( FindCommand( pszName ) )
	{
		Warning( "CCVarSystem::AddCommand: Command \"%s\" already defined\n", pszName );
		return false;
	}

	ConCommand_t* pCommand = new ConCommand_t;

	pCommand->pszName = pszName;
	pCommand->pFunction = pFunction;
	pCommand->flags = flags;

	pCommand->pNext = m_pCommands;

	m_pCommands = pCommand;

	return true;
}

void CCVarSystem::RemoveCommand( const char* const pszName )
{
	assert( pszName );

	if( !( *pszName ) )
	{
		return;
	}

	ConCommand_t* pPrev = nullptr;
	ConCommand_t* pCommand = m_pCommands;

	while( pCommand )
	{
		if( strcmp( pszName, pCommand->GetName() ) == 0 )
		{
			if( pPrev )
				pPrev->pNext = pCommand->pNext;
			else
				m_pCommands = pCommand->pNext;

			delete pCommand;
			return;
		}

		pPrev = pCommand;
		pCommand = pCommand->pNext;
	}
}

const ConCommand_t* CCVarSystem::FindCommand( const char* const pszName ) const
{
	assert( pszName );

	if( !( *pszName ) )
		return nullptr;

	ConCommand_t* pCommand = m_pCommands;

	while( pCommand )
	{
		if( strcmp( pszName, pCommand->GetName() ) == 0 )
			return pCommand;

		pCommand = pCommand->pNext;
	}

	return nullptr;
}

bool CCVarSystem::AddCVar( cvar_t* pCVar )
{
	assert( pCVar );

	if( FindCVar( pCVar->pszName ) )
	{
		Warning( "CCVarSystem::AddCVar: Can't register variable \"%s\", already defined\n", pCVar->pszName );
		return false;
	}

	if( FindCommand( pCVar->pszName ) )
	{
		Warning( "CCVarSystem::AddCVar: \"%s\" is a command\n", pCVar->pszName );
		return false;
	}

	//Copy the value off, because it will be delete[]'d later on.
	const char* pszString = pCVar->string;
	pCVar->string = new char[ strlen( pszString ) + 1 ];
	strcpy( pCVar->string, pszString );
	pCVar->value = static_cast<float>( atof( pCVar->string ) );

	pCVar->next = m_pCVars;
	m_pCVars = pCVar;

	return true;
}

void CCVarSystem::RemoveCVar( const char* const pszName )
{
	cvar_t* pPrev = nullptr;
	cvar_t* pCVar = m_pCVars;

	while( pCVar )
	{
		if( strcmp( pszName, pCVar->pszName ) == 0 )
		{
			if( pPrev )
				pPrev->next = pCVar->next;
			else
				m_pCVars = pCVar->next;

			delete[] pCVar->string;
			return;
		}

		pPrev = pCVar;
		pCVar = pCVar->next;
	}
}

const cvar_t* CCVarSystem::FindCVar( const char* const pszName ) const
{
	cvar_t* pCVar = m_pCVars;

	while( pCVar )
	{
		if( strcmp( pszName, pCVar->pszName ) == 0 )
			return pCVar;

		pCVar = pCVar->next;
	}

	return nullptr;
}

cvar_t* CCVarSystem::FindCVar( const char* const pszName )
{
	return const_cast<cvar_t*>( const_cast<const CCVarSystem* const>( this )->FindCVar( pszName ) );
}

const cvar_t* CCVarSystem::GetCVarWarn( const char* const pszCVar ) const
{
	assert( pszCVar );

	auto pCVar = FindCVar( pszCVar );

	if( !pCVar )
	{
		Warning( "Unknown CVar \"%s\", ignoring\n", pszCVar );
		return nullptr;
	}

	return pCVar;
}

const char* CCVarSystem::GetCVarString( const char* const pszCVar ) const
{
	const cvar_t* pCVar = GetCVarWarn( pszCVar );

	if( !pCVar )
		return "";

	return pCVar->string;
}

float CCVarSystem::GetCVarFloat( const char* const pszCVar ) const
{
	const cvar_t* pCVar = GetCVarWarn( pszCVar );

	if( !pCVar )
		return 0;

	return pCVar->value;
}

void CCVarSystem::SetCVarString( const char* const pszCVar, const char* const pszValue )
{
	cvar_t* pCVar = const_cast<cvar_t*>( GetCVarWarn( pszCVar ) );

	if( !pCVar )
		return;

	SetCVarDirect( pCVar, pszValue );
}

void CCVarSystem::SetCVarFloat( const char* const pszCVar, const float flValue )
{
	cvar_t* pCVar = const_cast<cvar_t*>( GetCVarWarn( pszCVar ) );

	if( !pCVar )
		return;

	char szBuffer[ 512 ];

	snprintf( szBuffer, sizeof( szBuffer ), "%f", flValue );

	SetCVarDirect( pCVar, szBuffer );
}

void CCVarSystem::SetCVarDirect( cvar_t* pCVar, const char* const pszValue )
{
	assert( pCVar );
	assert( pszValue );

	if( !pCVar )
		return;

	delete[] pCVar->string;

	pCVar->string = new char[ strlen( pszValue ) + 1 ];
	strcpy( pCVar->string, pszValue );
	pCVar->value = static_cast<float>( atof( pCVar->string ) );

	Msg( "\"%s\" changed to \"%s\"\n", pCVar->pszName, pCVar->string );
}

void CCVarSystem::ExecuteString( const char* const pszString, const Source source )
{
	m_Command.Initialize( pszString );

	//Execute the command line.
	if( m_Command.ArgC() == 0 )
	{
		//No tokens.
		return;
	}

	if( const ConCommand_t* pCommand = FindCommand( m_Command.Arg( 0 ) ) )
	{
		pCommand->pFunction();
		return;
	}

	//TODO: alias - Solokiller

	if( cvar_t* pCVar = FindCVar( m_Command.Arg( 0 ) ) )
	{
		if( m_Command.ArgC() == 1 )
		{
			Msg( "\"%s\" is \"%s\"\n", pCVar->pszName, pCVar->string );
			return;
		}

		SetCVarDirect( pCVar, m_Command.Arg( 1 ) );

		return;
	}

	Msg( "Unknown command \"%s\"\n", m_Command.Arg( 0 ) );
}

const char* CCVarSystem::GetArgs() const
{
	return m_Command.GetArgumentsString();
}

int CCVarSystem::GetArgC() const
{
	return m_Command.ArgC();
}

const char* CCVarSystem::GetArgV( const int iArg ) const
{
	return m_Command.Arg( iArg );
}
}