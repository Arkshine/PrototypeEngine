#ifndef ENGINE_CONSOLE_CCOMMANDBUFFER_H
#define ENGINE_CONSOLE_CCOMMANDBUFFER_H

#include <cstdint>

#include "CNetworkBuffer.h"

/**
*	Represents the command buffer. Commands entered in the console, as well as key presses are all processed by this.
*/
class CCommandBuffer final
{
private:
	static const size_t BUFFER_SIZE = 8192;

public:
	CCommandBuffer() = default;

	/**
	*	Initializes the buffer.
	*	@return Whether the buffer initialized successfully.
	*/
	bool Initialize();

	/**
	*	Adds text to the command buffer.
	*	@param pszText Text to add.
	*	@return Whether the text was added successfully.
	*/
	bool AddText( const char* const pszText );

	/**
	*	Inserts text immediately after the current command.
	*	Adds an \n to the text.
	*	@param pszText Text to insert.
	*	@return Whether the text was successfully inserted.
	*/
	bool InsertText( const char* const pszText );

	/**
	*	Executes the commands that are currently in the buffer.
	*	@return Whether execution completed succesfully.
	*/
	bool Execute();

	/**
	*	@return Whether the command buffer was told to wait until next frame to execute any more commands.
	*/
	bool IsWaiting() const { return m_bWait; }

	/**
	*	Sets whether the command buffer should wait until next frame to execute any more commands.
	*/
	void SetWait( const bool bWait )
	{
		m_bWait = bWait;
	}

private:
	CNetworkBuffer m_Buffer;
	uint8_t m_Data[ BUFFER_SIZE ];

	bool m_bWait = false;

private:
	CCommandBuffer( const CCommandBuffer& ) = delete;
	CCommandBuffer& operator=( const CCommandBuffer& ) = delete;
};

#endif //ENGINE_CONSOLE_CCOMMANDBUFFER_H
