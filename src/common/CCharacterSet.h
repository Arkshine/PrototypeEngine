#ifndef COMMON_CCHARACTERSET_H
#define COMMON_CCHARACTERSET_H

#include <cassert>
#include <climits>

/**
*	Represents a set of ASCII characters. Used to quickly check if a character falls in a group of non-sequential characters.
*/
class CCharacterSet final
{
public:
	static const size_t MAX_CHARS = 1 << CHAR_BIT;

public:
	CCharacterSet() = default;

	CCharacterSet( const CCharacterSet& other ) = default;
	CCharacterSet& operator=( const CCharacterSet& other ) = default;

	CCharacterSet( const char* pszCharacters );

	inline bool InSet( const char cCharacter ) const
	{
		return m_szChars[ cCharacter ];
	}

private:
	bool m_szChars[ MAX_CHARS ] = {};
};

inline CCharacterSet::CCharacterSet( const char* pszCharacters )
{
	assert( pszCharacters );

	while( *pszCharacters )
	{
		m_szChars[ *pszCharacters ] = true;

		++pszCharacters;
	}
}

#endif //COMMON_CCHARACTERSET_H
