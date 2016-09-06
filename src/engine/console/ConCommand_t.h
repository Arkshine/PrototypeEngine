#ifndef ENGINE_CONSOLE_CONCOMMAND_T_H
#define ENGINE_CONSOLE_CONCOMMAND_T_H

#include <cstdint>

namespace cvar
{
using xcommand_t = void ( * )();

namespace CmdFlag
{
/**
*	Command flags
*/
enum CmdFlag
{
	NONE	= 0,
	/**
	*	Command registered by client.
	*/
	CLIENT	= 1 << 0,

	/**
	*	Command registered by server.
	*/
	SERVER	= 1 << 1,
};
};

struct ConCommand_t
{
	ConCommand_t* pNext;
	const char* pszName;
	xcommand_t pFunction;

	uint32_t flags;

	inline const char* GetName() const { return pszName; }

	inline bool IsClient() const { return ( flags & CmdFlag::CLIENT ) != 0; }

	inline bool IsServer() const { return ( flags & CmdFlag::SERVER ) != 0; }
};
}

#endif //ENGINE_CONSOLE_CONCOMMAND_T_H
