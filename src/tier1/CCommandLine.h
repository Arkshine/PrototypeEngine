#ifndef COMMON_CCOMMANDLINE_H
#define COMMON_CCOMMANDLINE_H

#include <memory>
#include <string>
#include <vector>

#include "ICommandLine.h"

class CCommandLine final : public ICommandLine
{
private:
	typedef std::vector<std::string> Arguments_t;

	static const char META_PREFIX[];

	static const size_t META_PREFIX_LENGTH = 6;

public:
	CCommandLine() = default;
	~CCommandLine();

	bool Initialize( const size_t uiArgC, char** ppszArgV, const char* const* ppszStripCommands = nullptr ) override;

	const char* GetCommandLineString() const override;

	size_t GetArgumentCount() const override;

	const char* GetArgument( const size_t uiArgument ) const override;

	const char* operator[]( const size_t uiArgument ) const override;

	size_t IndexOf( const char* const pszKey ) const override;

	const char* GetValue( const char* const pszKey ) const override;

private:
	void Clear();

private:
	std::string m_szCommandLine;

	Arguments_t m_Arguments;
};


#endif //COMMON_CCOMMANDLINE_H