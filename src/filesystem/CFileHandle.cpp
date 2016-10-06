#include <cassert>

#include "CFileSystem.h"

#include "CFileHandle.h"

CFileHandle::CFileHandle( CFileSystem& fileSystem, const char* pszFileName, const char* pszMode, const bool bIsPackFile )
{
	assert( pszFileName );
	assert( pszMode );

	m_pFile = fopen( pszFileName, pszMode );

	if( m_pFile )
	{
		m_szFileName = pszFileName;

		struct stat buffer{};

		if( stat( pszFileName, &buffer ) == -1 )
		{
			fileSystem.Warning( FILESYSTEM_WARNING_CRITICAL, "CFileHandle::CFileHandle: Couldn't stat file \"%s\" that was opened with mode \"%s\"!\n", pszFileName, pszMode );
		}

		m_uiLength = buffer.st_size;

		if( bIsPackFile )
			SetFlags( FileHandleFlag::IS_PACK_FILE );
	}
}

CFileHandle::CFileHandle( CFileSystem& fileSystem, const char* pszFileName, FILE* pFile, uint64_t uiStartOffset, uint64_t uiLength )
{
	assert( pszFileName );
	assert( pFile );

	m_pFile = pFile;

	if( m_pFile )
	{
		m_szFileName = pszFileName;

		m_uiStartOffset = uiStartOffset;
		m_uiLength = uiLength;

		m_Flags |= FileHandleFlag::IS_PACK_ENTRY;
	}
	else
	{
		fileSystem.Warning( FILESYSTEM_WARNING_CRITICAL, "CFileHandle::CFileHandle: Null FILE* for pack file entry \"%s\"!\n", pszFileName );
	}
}

void CFileHandle::Close()
{
	if( IsOpen() )
	{
		if( !IsPackEntry() )
		{
			fclose( m_pFile );
		}

		m_pFile = nullptr;
		m_szFileName.clear();

		m_uiStartOffset = m_uiLength = 0;

		m_Flags = FileHandleFlag::NONE;
	}
}