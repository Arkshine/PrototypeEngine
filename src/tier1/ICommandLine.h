#ifndef COMMON_ICOMMANDLINE_H
#define COMMON_ICOMMANDLINE_H

#include <cstddef>

/**
*	Represents the command line.
*	Never store a pointer returned by this.
*/
class ICommandLine
{
public:
	static const size_t INVALID_INDEX = -1;

public:
	virtual ~ICommandLine() = 0;

	/**
	*	Initializes the command line to the given arguments.
	*	"-meta:" commands are renamed and commands with the original name are removed.
	*	@param uiArgC Argument count.
	*	@param ppszArgV Argument vector.
	*	@param ppszStripCommands Null terminated list of commands to remove from the command line.
	*	@return Whether initialization succeeded.
	*/
	virtual bool Initialize( const size_t uiArgC, char** ppszArgV, const char* const* ppszStripCommands = nullptr ) = 0;

	/**
	*	@return The complete command line.
	*/
	virtual const char* GetCommandLineString() const = 0;

	/**
	*	@return The number of arguments. Identical to argc.
	*/
	virtual size_t GetArgumentCount() const = 0;

	/**
	*	Gets an argument by index.
	*	@param uiArgument Index of the argument.
	*	@return If the index is valid, returns the argument. Otherwise, returns null.
	*/
	virtual const char* GetArgument( const size_t uiArgument ) const = 0;

	/**
	*	@copydoc GetArgument( const int uiArgument ) const
	*/
	virtual const char* operator[]( const size_t uiArgument ) const = 0;

	/**
	*	Gets the index of a key.
	*	@param pszKey Key to search for.
	*	@return If the key was found, returns the index. Otherwise, returns INVALID_INDEX.
	*/
	virtual size_t IndexOf( const char* const pszKey ) const = 0;

	/**
	*	Gets the value for a key.
	*	@param pszKey Key to search for.
	*	@return If the key was found, returns the value. Otherwise, returns null. If the key is the last argument, returns an empty string.
	*/
	virtual const char* GetValue( const char* const pszKey ) const = 0;
};

inline ICommandLine::~ICommandLine()
{
}

#endif //COMMON_ICOMMANDLINE_H