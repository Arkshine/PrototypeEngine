#ifndef COMMON_ICOMMANDLINE_H
#define COMMON_ICOMMANDLINE_H

/**
*	Represents the command line.
*	Never store a pointer returned by this.
*/
class ICommandLine
{
public:
	static const int INVALID_INDEX = -1;

public:
	virtual ~ICommandLine() = 0;

	/**
	*	Initializes the command line to the given arguments.
	*	@param iArgC Argument count.
	*	@param ppszArgV Argument vector.
	*	@param bTakeOwnership Whether to take ownership of the argument vector.
	*	@return Whether initialization succeeded.
	*/
	virtual bool Initialize( const int iArgC, char** ppszArgV, const bool bTakeOwnership ) = 0;

	/**
	*	@return The complete command line.
	*/
	virtual const char* GetCommandLineString() const = 0;

	/**
	*	@return The number of arguments. Identical to argc.
	*/
	virtual int GetArgumentCount() const = 0;

	/**
	*	Gets an argument by index.
	*	@param iArgument Index of the argument.
	*	@return If the index is valid, returns the argument. Otherwise, returns null.
	*/
	virtual const char* GetArgument( const int iArgument ) const = 0;

	/**
	*	@copydoc GetArgument( const int iArgument ) const
	*/
	virtual const char* operator[]( const int iArgument ) const = 0;

	/**
	*	Gets the index of a key.
	*	@param pszKey Key to search for.
	*	@return If the key was found, returns the index. Otherwise, returns INVALID_INDEX.
	*/
	virtual int IndexOf( const char* const pszKey ) const = 0;

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