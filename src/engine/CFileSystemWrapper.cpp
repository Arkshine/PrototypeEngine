#include <cstdarg>

#include "Engine.h"
#include "FileSystem2.h"

#include "CFileSystemWrapper.h"

void CFileSystemWrapper::Mount()
{
	g_pFileSystem->Mount();
}

void CFileSystemWrapper::Unmount()
{
	g_pFileSystem->Unmount();
}

void CFileSystemWrapper::RemoveAllSearchPaths()
{
	g_pFileSystem->RemoveAllSearchPaths();
}

void CFileSystemWrapper::AddSearchPath( const char *pPath, const char *pathID )
{
	g_pFileSystem->AddSearchPath( pPath, pathID );
}

bool CFileSystemWrapper::RemoveSearchPath( const char *pPath )
{
	return g_pFileSystem->RemoveSearchPath( pPath );
}

void CFileSystemWrapper::RemoveFile( const char *pRelativePath, const char *pathID )
{
	g_pFileSystem->RemoveFile( pRelativePath, pathID );
}

void CFileSystemWrapper::CreateDirHierarchy( const char *path, const char *pathID )
{
	g_pFileSystem->CreateDirHierarchy( path, pathID );
}

bool CFileSystemWrapper::FileExists( const char *pFileName )
{
	return g_pFileSystem->FileExists( pFileName );
}

bool CFileSystemWrapper::IsDirectory( const char *pFileName )
{
	return g_pFileSystem->IsDirectory( pFileName );
}

FileHandle_t CFileSystemWrapper::Open( const char *pFileName, const char *pOptions, const char *pathID )
{
	return g_pFileSystem->Open( pFileName, pOptions, pathID );
}

void CFileSystemWrapper::Close( FileHandle_t file )
{
	g_pFileSystem->Close( file );
}

void CFileSystemWrapper::Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType )
{
	g_pFileSystem->Seek( file, pos, seekType );
}

unsigned int CFileSystemWrapper::Tell( FileHandle_t file )
{
	return g_pFileSystem->Tell( file );
}

unsigned int CFileSystemWrapper::Size( FileHandle_t file )
{
	return g_pFileSystem->Size( file );
}

unsigned int CFileSystemWrapper::Size( const char *pFileName )
{
	return g_pFileSystem->Size( pFileName );
}

long CFileSystemWrapper::GetFileTime( const char *pFileName )
{
	return g_pFileSystem->GetFileTime( pFileName );
}

void CFileSystemWrapper::FileTimeToString( char* pStrip, int maxCharsIncludingTerminator, long fileTime )
{
	g_pFileSystem->FileTimeToString( pStrip, maxCharsIncludingTerminator, fileTime );
}

bool CFileSystemWrapper::IsOk( FileHandle_t file )
{
	return g_pFileSystem->IsOk( file );
}

void CFileSystemWrapper::Flush( FileHandle_t file )
{
	g_pFileSystem->Flush( file );
}

bool CFileSystemWrapper::EndOfFile( FileHandle_t file )
{
	return g_pFileSystem->EndOfFile( file );
}

int CFileSystemWrapper::Read( void* pOutput, int size, FileHandle_t file )
{
	return g_pFileSystem->Read( pOutput, size, file );
}

int CFileSystemWrapper::Write( void const* pInput, int size, FileHandle_t file )
{
	return g_pFileSystem->Write( pInput, size, file );
}

char *CFileSystemWrapper::ReadLine( char *pOutput, int maxChars, FileHandle_t file )
{
	return g_pFileSystem->ReadLine( pOutput, maxChars, file );
}

int CFileSystemWrapper::FPrintf( FileHandle_t file, const char *pFormat, ... )
{
	va_list list;

	va_start( list, pFormat );

	const int iResult = g_pFileSystem->VFPrintf( file, "%s", list );

	va_end( list );

	return iResult;
}

void *CFileSystemWrapper::GetReadBuffer( FileHandle_t file, int *outBufferSize, bool failIfNotInCache )
{
	return g_pFileSystem->GetReadBuffer( file, outBufferSize, failIfNotInCache );
}

void CFileSystemWrapper::ReleaseReadBuffer( FileHandle_t file, void *readBuffer )
{
	g_pFileSystem->ReleaseReadBuffer( file, readBuffer );
}

const char *CFileSystemWrapper::FindFirst( const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID )
{
	return g_pFileSystem->FindFirst( pWildCard, pHandle, pathID );
}

const char *CFileSystemWrapper::FindNext( FileFindHandle_t handle )
{
	return g_pFileSystem->FindNext( handle );
}

bool CFileSystemWrapper::FindIsDirectory( FileFindHandle_t handle )
{
	return g_pFileSystem->FindIsDirectory( handle );
}

void CFileSystemWrapper::FindClose( FileFindHandle_t handle )
{
	g_pFileSystem->FindClose( handle );
}

void CFileSystemWrapper::GetLocalCopy( const char *pFileName )
{
	g_pFileSystem->GetLocalCopy( pFileName );
}

const char *CFileSystemWrapper::GetLocalPath( const char *pFileName, char *pLocalPath, int localPathBufferSize )
{
	return g_pFileSystem->GetLocalPath( pFileName, pLocalPath, localPathBufferSize );
}

char *CFileSystemWrapper::ParseFile( char* pFileBytes, char* pToken, bool* pWasQuoted )
{
	return g_pFileSystem->ParseFile( pFileBytes, pToken, pWasQuoted );
}

bool CFileSystemWrapper::FullPathToRelativePath( const char *pFullpath, char *pRelative )
{
	return g_pFileSystem->FullPathToRelativePath( pFullpath, pRelative );
}

bool CFileSystemWrapper::GetCurrentDirectory( char* pDirectory, int maxlen )
{
	return g_pFileSystem->GetCurrentDirectory( pDirectory, maxlen );
}

void CFileSystemWrapper::PrintOpenedFiles( void )
{
	g_pFileSystem->PrintOpenedFiles();
}

void CFileSystemWrapper::SetWarningFunc( FileSystemWarningFunc pfnWarning )
{
	g_pFileSystem->SetWarningFunc( pfnWarning );
}

void CFileSystemWrapper::SetWarningLevel( FileWarningLevel_t level )
{
	g_pFileSystem->SetWarningLevel( level );
}

void CFileSystemWrapper::LogLevelLoadStarted( const char *name )
{
	g_pFileSystem->LogLevelLoadStarted( name );
}

void CFileSystemWrapper::LogLevelLoadFinished( const char *name )
{
	g_pFileSystem->LogLevelLoadFinished( name );
}

int CFileSystemWrapper::HintResourceNeed( const char *hintlist, int forgetEverything )
{
	return g_pFileSystem->HintResourceNeed( hintlist, forgetEverything );
}

int CFileSystemWrapper::PauseResourcePreloading( void )
{
	return g_pFileSystem->PauseResourcePreloading();
}

int CFileSystemWrapper::ResumeResourcePreloading( void )
{
	return g_pFileSystem->ResumeResourcePreloading();
}

int CFileSystemWrapper::SetVBuf( FileHandle_t stream, char *buffer, int mode, long size )
{
	return g_pFileSystem->SetVBuf( stream, buffer, mode, size );
}

void CFileSystemWrapper::GetInterfaceVersion( char *p, int maxlen )
{
	g_pFileSystem->GetInterfaceVersion( p, maxlen );
}

bool CFileSystemWrapper::IsFileImmediatelyAvailable( const char *pFileName )
{
	return g_pFileSystem->IsFileImmediatelyAvailable( pFileName );
}

WaitForResourcesHandle_t CFileSystemWrapper::WaitForResources( const char *resourcelist )
{
	return g_pFileSystem->WaitForResources( resourcelist );
}

bool CFileSystemWrapper::GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */, bool *complete /* out */ )
{
	return g_pFileSystem->GetWaitForResourcesProgress( handle, progress, complete );
}

void CFileSystemWrapper::CancelWaitForResources( WaitForResourcesHandle_t handle )
{
	g_pFileSystem->CancelWaitForResources( handle );
}

bool CFileSystemWrapper::IsAppReadyForOfflinePlay( int appID )
{
	return g_pFileSystem->IsAppReadyForOfflinePlay( appID );
}

bool CFileSystemWrapper::AddPackFile( const char *fullpath, const char *pathID )
{
	return g_pFileSystem->AddPackFile( fullpath, pathID );
}

FileHandle_t CFileSystemWrapper::OpenFromCacheForRead( const char *pFileName, const char *pOptions, const char *pathID )
{
	return g_pFileSystem->OpenFromCacheForRead( pFileName, pOptions, pathID );
}

void CFileSystemWrapper::AddSearchPathNoWrite( const char *pPath, const char *pathID )
{
	g_pFileSystem->AddSearchPathNoWrite( pPath, pathID );
}