#ifndef COMMON_FILESYSTEM2_H
#define COMMON_FILESYSTEM2_H

#include <cstddef>
#include <cstdint>

#include "FileSystem.h"

typedef uint32_t FileSystemFindFlags_t;

namespace FileSystemFindFlag
{
enum FileSystemFindFlag : FileSystemFindFlags_t
{
	NONE					= 0,

	/**
	*	Skip identical search paths. 
	*	Different path IDs can have the same search paths, which can result in the same file being returned multiple times.
	*/
	SKIP_IDENTICAL_PATHS	= 1 << 0,
};
}

/**
*	GoldSource2 filesystem interface. Provides extended functionality to the filesystem used by GoldSource.
*/
class IFileSystem2 : public IFileSystem
{
public:

	/**
	*	64 bit version of Seek.
	*	@see Seek
	*/
	virtual void			Seek64( FileHandle_t file, int64_t pos, FileSystemSeek_t seekType ) = 0;

	/**
	*	64 bit version of Tell.
	*	@see Tell
	*/
	virtual uint64_t		Tell64( FileHandle_t file ) = 0;

	/**
	*	64 bit version of Size.
	*	@see Size( FileHandle_t file )
	*/
	virtual uint64_t		Size64( FileHandle_t file ) = 0;

	/**
	*	64 bit version of Size.
	*	@see Size( const char *pFileName )
	*/
	virtual uint64_t		Size64( const char *pFileName ) = 0;

	/**
	*	Extended version of GetFileTime.
	*	@return 64 bit version of GetFileTime's return value.
	*	@see GetFileTime
	*/
	virtual int64_t			GetFileTimeEx( const char *pFileName ) = 0;

	/**
	*	Extended version of FileTimeToString.
	*	@param fileTime 64 bit file time.
	*	@see FileTimeToString
	*/
	virtual void			FileTimeToStringEx( char* pStrip, int maxCharsIncludingTerminator, int64_t fileTime ) = 0;

	/**
	*	Extended version of FindFirst.
	*	@param flags Find flags.
	*	@see FindFirst
	*	@see FileSystemFindFlag::FileSystemFindFlag
	*/
	virtual const char		*FindFirstEx( const char *pWildCard, FileFindHandle_t *pHandle, FileSystemFindFlags_t flags, const char *pathID = nullptr ) = 0;

	/**
	*	Extended version of FullPathToRelativePath.
	*	@param uiSizeInChars Size of the relative path buffer, in characters, including the null terminator.
	*	@see FullPathToRelativePath
	*/
	virtual bool			FullPathToRelativePathEx( const char *pFullpath, char *pRelative, size_t uiSizeInChars ) = 0;
};

/**
*	Interface name.
*/
#define FILESYSTEM2_INTERFACE_VERSION "VFileSystem009_2"

#endif //COMMON_FILESYSTEM2_H
