#ifndef COMMON_CCOMMANDLINE_H
#define COMMON_CCOMMANDLINE_H

#include "ICommandLine.h"

class CCommandLine final : public ICommandLine
{
public:
	CCommandLine() = default;
	~CCommandLine();

	bool Initialize( const int iArgC, char** ppszArgV, const bool bTakeOwnership ) override;

	const char* GetCommandLineString() const override;

	int GetArgumentCount() const override;

	const char* GetArgument( const int iArgument ) const override;

	const char* operator[]( const int iArgument ) const override;

	int IndexOf( const char* const pszKey ) const override;

	const char* GetValue( const char* const pszKey ) const override;

	void ForgetBuffer() override
	{
		m_ppszArgV = nullptr;
		m_iArgC = 0;
		delete[] m_pszCommandLine;
		m_pszCommandLine = nullptr;
	}

private:
	void Clear();

private:
	char* m_pszCommandLine = nullptr;

	int m_iArgC = 0;

	char** m_ppszArgV = nullptr;
};


#endif //COMMON_CCOMMANDLINE_H