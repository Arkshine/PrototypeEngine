#ifndef METALOADER_CMETALOADER_H
#define METALOADER_CMETALOADER_H

#include <experimental/filesystem>

#include "Platform.h"

#include "lib/CLibrary.h"

#include "IMetaLoader.h"

class IFileSystem2;

class IMetaTool;

class CMetaLoader : public IMetaLoader
{
public:

	CMetaLoader() = default;

	const char* GetGameDirectory( char* pszDest, size_t uiSizeInCharacters ) const override;

	SDL_Window* GetEngineWindow() const override;

	bool IsListenServer() const override { return m_bIsListenServer; }

	/**
	*	Gets the game directory directly. The above is safer since it avoids passing a pointer to an address in this library.
	*/
	const char* GetGameDirLocal() const { return m_szGameDir; }

	void SetGameDirectory( const char* pszGameDir );

	void Run( const bool bIsListenServer );

private:
	bool RunLoader();

	void Shutdown();

	bool LoadFileSystem();

	bool SetupFileSystem();

	SDL_Window* FindEngineWindow();

private:
	char m_szGameDir[ MAX_PATH ] = {};

	SDL_Window* m_pEngineWindow = nullptr;

	bool m_bIsListenServer = true;

	std::experimental::filesystem::path m_OldCWD;

	CLibrary m_FileSystemLib;

	IFileSystem2* m_pFileSystem = nullptr;

	CLibrary m_ToolLib;

	IMetaTool* m_pTool = nullptr;
};

extern CMetaLoader g_MetaLoader;

#endif //METALOADER_CMETALOADER_H
