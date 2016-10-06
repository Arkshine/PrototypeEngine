#ifndef FILESYSTEM_CFILESYSTEM_H
#define FILESYSTEM_CFILESYSTEM_H

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "Platform.h"

#include "FileSystem.h"

#undef SearchPath

class CFileSystem : public IFileSystem
{
private:
	typedef uint32_t SearchPathFlags_t;

	struct SearchPathFlag
	{
		enum : SearchPathFlags_t
		{
			NONE		= 0,
			READ_ONLY	= 1 << 0,
		};
	};

	struct SearchPath
	{
		char szPath[ MAX_PATH ];

		const char* pszPathID;

		SearchPathFlags_t flags;
	};

	typedef uint32_t FindFileFlags_t;

	struct FindFileFlag
	{
		enum : FindFileFlags_t
		{
			NONE			= 0,
			END_OF_DATA		= 1 << 0,
			VALID			= 1 << 1,
		};
	};

	struct FindFileData
	{
		std::experimental::filesystem::recursive_directory_iterator iterator;
		std::experimental::filesystem::directory_entry entry;
		std::string szFileName;
		std::regex filter;

		char szPathID[ MAX_PATH ];

		const SearchPath* pCurrentPath = nullptr;

		FindFileFlags_t flags = FindFileFlag::VALID;
	};

	typedef std::vector<SearchPath> SearchPaths_t;
	typedef std::vector<FILE*> OpenedFiles_t;
	typedef std::vector<std::unique_ptr<FindFileData>> FindFiles_t;

public:
	CFileSystem() = default;

	void			Mount() override;

	void			Unmount() override;

	void			RemoveAllSearchPaths() override;

	void			AddSearchPath( const char *pPath, const char *pathID ) override;

	bool			RemoveSearchPath( const char *pPath ) override;

	void			RemoveFile( const char *pRelativePath, const char *pathID ) override;

	void			CreateDirHierarchy( const char *path, const char *pathID ) override;

	// File I/O and info
	bool			FileExists( const char *pFileName ) override;

	bool			IsDirectory( const char *pFileName ) override;

	FileHandle_t	Open( const char *pFileName, const char *pOptions, const char *pathID = nullptr ) override;

	void			Close( FileHandle_t file ) override;

	void			Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) override;

	unsigned int	Tell( FileHandle_t file ) override;

	unsigned int	Size( FileHandle_t file ) override;

	unsigned int	Size( const char *pFileName ) override;

	long			GetFileTime( const char *pFileName ) override;

	void			FileTimeToString( char* pStrip, int maxCharsIncludingTerminator, long fileTime ) override;

	bool			IsOk( FileHandle_t file ) override;

	void			Flush( FileHandle_t file ) override;

	bool			EndOfFile( FileHandle_t file ) override;

	int				Read( void* pOutput, int size, FileHandle_t file ) override;

	int				Write( void const* pInput, int size, FileHandle_t file ) override;

	char			*ReadLine( char *pOutput, int maxChars, FileHandle_t file ) override;

	int				FPrintf( FileHandle_t file, char *pFormat, ... ) override;

	// direct filesystem buffer access
	void			*GetReadBuffer( FileHandle_t file, int *outBufferSize, bool failIfNotInCache ) override;

	void            ReleaseReadBuffer( FileHandle_t file, void *readBuffer ) override;

	// FindFirst/FindNext
	const char		*FindFirst( const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID = nullptr ) override;

	const char		*FindNext( FileFindHandle_t handle ) override;

	bool			FindIsDirectory( FileFindHandle_t handle ) override;

	void			FindClose( FileFindHandle_t handle ) override;

	void			GetLocalCopy( const char *pFileName ) override;

	const char		*GetLocalPath( const char *pFileName, char *pLocalPath, int localPathBufferSize ) override;

	char			*ParseFile( char* pFileBytes, char* pToken, bool* pWasQuoted ) override;

	bool			FullPathToRelativePath( const char *pFullpath, char *pRelative ) override;

	bool			GetCurrentDirectory( char* pDirectory, int maxlen ) override;

	void			PrintOpenedFiles( void ) override;

	void			SetWarningFunc( FileSystemWarningFunc pfnWarning ) override;

	void			SetWarningLevel( FileWarningLevel_t level ) override;

	void			LogLevelLoadStarted( const char *name ) override;

	void			LogLevelLoadFinished( const char *name ) override;

	int				HintResourceNeed( const char *hintlist, int forgetEverything ) override;

	int				PauseResourcePreloading( void ) override;

	int				ResumeResourcePreloading( void ) override;

	int				SetVBuf( FileHandle_t stream, char *buffer, int mode, long size ) override;

	void			GetInterfaceVersion( char *p, int maxlen ) override;

	bool			IsFileImmediatelyAvailable( const char *pFileName ) override;

	WaitForResourcesHandle_t WaitForResources( const char *resourcelist ) override;

	bool			GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */, bool *complete /* out */ ) override;

	void			CancelWaitForResources( WaitForResourcesHandle_t handle ) override;

	bool			IsAppReadyForOfflinePlay( int appID ) override;

	bool			AddPackFile( const char *fullpath, const char *pathID ) override;

	FileHandle_t	OpenFromCacheForRead( const char *pFileName, const char *pOptions, const char *pathID = nullptr ) override;

	void			AddSearchPathNoWrite( const char *pPath, const char *pathID ) override;

private:
	void Warning( FileWarningLevel_t level, const char* pszFormat, ... );

	SearchPaths_t::const_iterator FindSearchPath( const char* pszPath, const bool bCheckPathID = false, const char* pszPathID = nullptr ) const;

	bool AddSearchPath( const char *pPath, const char *pathID, const bool bReadOnly );

	bool IsValidFileHandle( FileHandle_t handle ) const;

private:
	SearchPaths_t m_SearchPaths;
	OpenedFiles_t m_OpenedFiles;
	FindFiles_t m_FindFiles;

	FileSystemWarningFunc m_WarningFunc = nullptr;

	FileWarningLevel_t m_WarningLevel = FileWarningLevel_t::FILESYSTEM_WARNING_REPORTUNCLOSED;

private:
	CFileSystem( const CFileSystem& ) = delete;
	CFileSystem& operator=( const CFileSystem& ) = delete;
};

#endif //FILESYSTEM_CFILESYSTEM_H
