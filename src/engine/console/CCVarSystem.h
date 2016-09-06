#ifndef ENGINE_CONSOLE_CCVARSYSTEM_H
#define ENGINE_CONSOLE_CCVARSYSTEM_H

#include "CCommand.h"

#include "ConCommand_t.h"

namespace cvar
{
enum class Source
{
	CLIENT = 0,
	COMMAND,
};

typedef struct cvar_s cvar_t;

class CCVarSystem final
{
public:
	CCVarSystem();
	~CCVarSystem();

	bool Initialize();

	//Commands

	bool AddCommand( const char* const pszName, xcommand_t pFunction, const uint32_t flags = CmdFlag::NONE );

	void RemoveCommand( const char* const pszName );

	const ConCommand_t* FindCommand( const char* const pszName ) const;

	//Cvars

	bool AddCVar( cvar_t* pCVar );

	void RemoveCVar( const char* const pszName );

	const cvar_t* FindCVar( const char* const pszName ) const;

	cvar_t* FindCVar( const char* const pszName );

private:
	const cvar_t* GetCVarWarn( const char* const pszCVar ) const;

public:
	const char* GetCVarString( const char* const pszCVar ) const;

	float GetCVarFloat( const char* const pszCVar ) const;

	void SetCVarString( const char* const pszCVar, const char* const pszValue );

	void SetCVarFloat( const char* const pszCVar, const float flValue );

	void SetCVarDirect( cvar_t* pCVar, const char* const pszValue );

	//Command execution

	void ExecuteString( const char* const pszString, const Source source );

	const char* GetArgs() const;

	int GetArgC() const;

	const char* GetArgV( const int iArg ) const;

private:
	ConCommand_t* m_pCommands = nullptr;
	cvar_t* m_pCVars = nullptr;

	//The current command.
	CCommand m_Command;

private:
	CCVarSystem( const CCVarSystem& ) = delete;
	CCVarSystem& operator=( const CCVarSystem& ) = delete;
};
}

#endif //ENGINE_CONSOLE_CCVARSYSTEM_H
