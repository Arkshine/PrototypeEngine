#ifndef COMMON_CFILE_H
#define COMMON_CFILE_H

/**
*	RAII wrapper around IFileSystem files.
*/
class CFile final
{
public:
	/**
	*	Default constructor. Represents the invalid file.
	*/
	CFile() = default;

	/**
	*	Takes ownership of the given file handle.
	*	@param hFile File handle.
	*/
	CFile( FileHandle_t hFile );

	/**
	*	Opens the given file with the given options, using the given path ID to find the file.
	*	@param pszFilename Name of the file to open.
	*	@param pszOptions Options.
	*	@param pszPathID Optional. ID of the search path to use to find the file. Pass null for all paths.
	*	@see IFileSystem::Open
	*/
	CFile( const char* const pszFilename, const char* const pszOptions, const char* const pszPathID = nullptr );

	CFile( CFile&& other ) = default;
	CFile& operator=( CFile&& other );

	/**
	*	Destructor. Closes the file if it is open.
	*/
	~CFile();

	/**
	*	@return The file handle.
	*/
	FileHandle_t GetFileHandle() { return m_hFile; }

	/**
	*	@return Whether the file's state is good for read/write operations.
	*/
	operator bool() const;

	/**
	*	@return Whether the file is open.
	*/
	bool IsOpen() const;

	/**
	*	Closes the file if it is open.
	*/
	void Close();

	/**
	*	@see IFileSystem::Seek
	*/
	void Seek( int pos, FileSystemSeek_t seekType );

	/**
	*	@see IFileSystem::Tell
	*/
	unsigned int Tell() const;

	/**
	*	@see IFileSystem::Size( FileHandle_t file )
	*/
	unsigned int Size() const;

	/**
	*	@see IFileSystem::IsOk
	*/
	bool IsOk() const;

	/**
	*	@see IFileSystem::Flush
	*/
	void Flush();

	/**
	*	@see IFileSystem::EndOfFile
	*/
	bool EndOfFile() const;

	/**
	*	@see IFileSystem::Read
	*/
	int Read( void* pOutput, int size );

	/**
	*	@see IFileSystem::Write
	*/
	int Write( void const* pInput, int size );

	/**
	*	@see IFileSystem::ReadLine
	*/
	char* ReadLine( char* pOutput, int maxChars );

private:
	FileHandle_t m_hFile = FILESYSTEM_INVALID_HANDLE;

private:
	CFile( const CFile& ) = delete;
	CFile& operator=( const CFile& ) = delete;
};

inline CFile::CFile( FileHandle_t hFile )
	: m_hFile( hFile )
{
}

inline CFile::CFile( const char* const pszFilename, const char* const pszOptions, const char* const pszPathID )
{
	m_hFile = g_pFileSystem->Open( pszFilename, pszOptions, pszPathID );
}

inline CFile& CFile::operator=( CFile&& other )
{
	if( this != &other )
	{
		Close();

		m_hFile = other.m_hFile;
		other.m_hFile = FILESYSTEM_INVALID_HANDLE;
	}

	return *this;
}

inline CFile::~CFile()
{
	Close();
}

inline CFile::operator bool() const
{
	return IsOk();
}

inline bool CFile::IsOpen() const
{
	return m_hFile != FILESYSTEM_INVALID_HANDLE;
}

inline void CFile::Close()
{
	if( IsOpen() )
	{
		g_pFileSystem->Close( m_hFile );
		m_hFile = FILESYSTEM_INVALID_HANDLE;
	}
}

inline void CFile::Seek( int pos, FileSystemSeek_t seekType )
{
	ASSERT( IsOpen() );

	g_pFileSystem->Seek( m_hFile, pos, seekType );
}

inline unsigned int CFile::Tell() const
{
	ASSERT( IsOpen() );

	return g_pFileSystem->Tell( m_hFile );
}

inline unsigned int CFile::Size() const
{
	ASSERT( IsOpen() );

	return g_pFileSystem->Size( m_hFile );
}

inline bool CFile::IsOk() const
{
	ASSERT( IsOpen() );

	return g_pFileSystem->IsOk( m_hFile );
}

inline void CFile::Flush()
{
	ASSERT( IsOpen() );

	return g_pFileSystem->Flush( m_hFile );
}

inline bool CFile::EndOfFile() const
{
	ASSERT( IsOpen() );

	return g_pFileSystem->EndOfFile( m_hFile );
}

inline int CFile::Read( void* pOutput, int size )
{
	ASSERT( IsOpen() );

	return g_pFileSystem->Read( pOutput, size, m_hFile );
}

inline int CFile::Write( void const* pInput, int size )
{
	ASSERT( IsOpen() );

	return g_pFileSystem->Write( pInput, size, m_hFile );
}

inline char* CFile::ReadLine( char* pOutput, int maxChars )
{
	ASSERT( IsOpen() );

	return g_pFileSystem->ReadLine( pOutput, maxChars, m_hFile );
}

#endif //COMMON_CFILE_H
