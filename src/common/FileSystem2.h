#ifndef COMMON_FILESYSTEM2_H
#define COMMON_FILESYSTEM2_H

#include <cstddef>

#include "FileSystem.h"

/**
*	GoldSource2 filesystem interface. Provides extended functionality to the filesystem used by GoldSource.
*/
class IFileSystem2 : public IFileSystem
{
public:

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
