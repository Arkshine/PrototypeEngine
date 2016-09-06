#ifndef CNETWORKBUFFER_H
#define CNETWORKBUFFER_H

#include <cstdint>

/**
*	@defgroup Networking Networking systems
*
*	@{
*/

/**
*	Pad a number so it lies on an N byte boundary.
*	So PadNumber( 0, 4 ) is 0 and PadNumber( 1, 4 ) is 4
*/
template<typename T>
inline T PadNumber( const T iNum, const T iBoundary )
{
	return ( ( iNum + ( iBoundary - 1 ) ) / iBoundary ) * iBoundary;
}

/**
*	Converts bits into bytes.
*/
template<typename T>
inline T BitByte( const T iBits )
{
	return PadNumber( iBits, static_cast<T>( 8 ) ) >> 3;
}

/**
*	Converts bytes into bits.
*/
template<typename T>
inline T ByteBit( const T iBytes )
{
	return iBytes << 3;
}

/**
*	A network buffer that points to a buffer containing arbitrary data
*/
class CNetworkBuffer final
{
public:
	/**
	*	Constructs an empty buffer.
	*/
	CNetworkBuffer();

	/**
	*	Constructs a buffer with the given debug name, data buffer and start offset.
	*	@param pszDebugName Name to use in debug output.
	*	@param pData Pointer to a buffer that this buffer will write to. Must be valid for at least as long as this object.
	*	@param uiNumBytes Total number of bytes that can be written into the buffer.
	*	@param uiCurrentBit Starting offset for read and write operations.
	*/
	CNetworkBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit = 0 );

	/**
	*	Initializes internal mask data for read and write operations.
	*/
	static void InitMasks();

	/**
	*	@return This buffer's debug name.
	*/
	const char* GetDebugName() const { return m_pszDebugName; }

	/**
	*	@return Whether this buffer has overflowed as a result of read or write operations.
	*/
	bool HasOverflowed() const { return m_fOverflowed; }

	/**
	*	@return Pointer to the internal buffer.
	*/
	const uint8_t* GetData() const { return m_pData; }

	/**
	*	@copydoc GetData() const
	*/
	uint8_t* GetData() { return m_pData; }

	/**
	*	@return Pointer to the next read or write operation location. Aligned to the nearest byte.
	*/
	const uint8_t* GetCurrentData() const { return m_pData + GetBytesInBuffer(); }

	/**
	*	@copydoc GetCurrentData() const
	*/
	uint8_t* GetCurrentData() { return m_pData + GetBytesInBuffer(); }

	/**
	*	@return Maximum number of bits that can be written to this buffer.
	*/
	size_t GetMaxBits() const { return m_uiMaxBits; }

	/**
	*	@return Maximum number of bytes that can be written to this buffer.
	*/
	size_t GetMaxBytes() const { return m_uiMaxBits >> 3; }

	/**
	*	@return Number of bits that have been written to this buffer, or the current read position.
	*/
	size_t GetBitsInBuffer() const { return m_uiCurrentBit; }

	/**
	*	@return Number of bytes that have been written to this buffer, or the current read position. Rounded up to the nearest byte.
	*/
	size_t GetBytesInBuffer() const { return BitByte( m_uiCurrentBit ); }

	/**
	*	@return Number of bytes that have been written to this buffer, or the current read position. Unlike GetBytesInBuffer, this is rounded down, not up.
	*/
	size_t GetUnpaddedBytesInBuffer() const { return m_uiCurrentBit >> 3; }

	/**
	*	@return Number of bits left in the buffer that can be written to, or read from.
	*/
	size_t GetBitsLeft() const { return m_uiMaxBits - m_uiCurrentBit; }

	/**
	*	@return Number of bytes left in the buffer that can be written to, or read from.
	*/
	size_t GetBytesLeft() const { return BitByte( GetBitsLeft() ); }

	/**
	*	This buffer is reset to an empty state.
	*/
	void ResetToEmpty();

	/*
	*	This buffer's read/write pointer is reset to the start.
	*	Resets the overflow flag.
	*/
	void ResetToStart();

	/**
	*	Sets the buffer's debug name, buffer and start offset.
	*	@param pszDebugName Name to use in debug output.
	*	@param pData Pointer to a buffer that this buffer will write to. Must be valid for at least as long as this object.
	*	@param uiNumBytes Total number of bytes that can be written into the buffer.
	*	@param uiCurrentBit Starting offset for read and write operations.
	*/
	void SetBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit = 0 );

	/*
	*	@return whether the buffer would be overflowed if the given number of bits were to be written.
	*/
	bool CheckOverflow( const size_t uiBits ) const;

	/*
	*	If overflow occurs after writing uiBits bits, sets overflow flag.
	*	@return Whether the bufferhas overflowed or not.
	*/
	bool Overflow( const size_t uiBits );

	/**
	*	Clears the overflow flag if it was set.
	*/
	void ClearOverflow();

	/*
	*	Write methods
	*/

	/**
	*	Writes a single bit to the buffer.
	*/
	void WriteOneBit( const int iValue );

	/**
	*	Writes an unsigned integer to the buffer.
	*	@param iValue Value to write.
	*	@param uiNumBits Number of bits to write.
	*/
	void WriteUnsignedBitLong( unsigned int iValue, const size_t uiNumBits );

	/**
	*	Writes a singed integer to the buffer.
	*	@param iValue Value to write.
	*	@param uiNumBits Number of bits to write. This includes the sign bit.
	*/
	void WriteSignedBitLong( int iValue, const size_t uiNumBits );

	/**
	*	Writes either a signed or unsigned integer to the buffer.
	*	@param iValue Value to write.
	*	@param uiNumBits Number of bits to write. In case this is a signed integer, this includes the sign bit.
	*	@param fSigned Whether this is a signed or unsigned integer.
	*/
	inline void WriteBitLong( const unsigned int iValue, const size_t uiNumBits, const bool fSigned )
	{
		if( fSigned )
			WriteSignedBitLong( ( int ) iValue, uiNumBits );
		else
			WriteUnsignedBitLong( iValue, uiNumBits );
	}

	/**
	*	Writes a float value to the buffer. The value is written as a signed integer.
	*	@param flValue Value to write.
	*/
	void WriteBitFloat( const float flValue );

	/**
	*	Writes a number of bits to the buffer.
	*	@param pData Data to write.
	*	@param iNumBits Number of bits to write.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool WriteBits( const void* pData, const size_t iNumBits );

	/**
	*	Writes a single character to the buffer.
	*	@param iValue Value to write.
	*/
	void WriteChar( const int iValue );

	/**
	*	Writes a single unsigned byte to the buffer.
	*	@param iValue Value to write.
	*/
	void WriteByte( const int iValue );

	/**
	*	Writes a single signed short to the buffer.
	*	@param iValue Value to write.
	*/
	void WriteShort( const int iValue );

	/**
	*	Writes a single unsigned short to the buffer.
	*	@param iValue Value to write.
	*/
	void WriteWord( const int iValue );

	/**
	*	Writes a single signed long to the buffer.
	*	@param iValue Value to write.
	*/
	void WriteLong( const int iValue );

	/**
	*	Writes a single float to the buffer. The value is written as a bit buffer.
	*	@param flValue Value to write.
	*/
	void WriteFloat( const float flValue );

	/**
	*	Writes a number of bytes to the buffer.
	*	@param pData Data to write.
	*	@param uiNumBytes Number of bytes to write.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool WriteBytes( const void* pData, const size_t uiNumBytes );

	/**
	*	Writes a string to the buffer.
	*	@param pszString String to write.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool WriteString( const char* pszString );

	/**
	*	Pads the buffer to the nearest byte.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool PadToByte();

	/**
	*	Advances the internal offset pointer to account for bits that were written through external means.
	*	@param uiBits Number of bits to advance by.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ExternalBitsWritten( const size_t uiBits );

	/**
	*	Advances the internal offset pointer to account for bytes that were written through external means.
	*	@param uiBytes Number of bytes to advance by.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ExternalBytesWritten( const size_t uiBytes );

	/**
	*	Backs up the internal offset pointer to account for bits that were invalidated through external means.
	*	@param uiBits Number of bits to back up.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool BackUpBits( const size_t uiBits );

	/**
	*	Backs up the internal offset pointer to account for bytes that were invalidated through external means.
	*	@param uiBytes Number of bytes to back up.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool BackUpBytes( const size_t uiBytes );

	/**
	*	Erases a chunk of bytes from the buffer.
	*	@param uiStart Starting byte.
	*	@param uiCount Number of bytes to erase.
	*/
	void EraseBytes( size_t uiStart, size_t uiCount );

	/*
	*	Read methods
	*/

	/**
	*	Reads a single bit.
	*	@return Zero if a 0 bit was read, non-zero if a 1 bit was read.
	*/
	int ReadOneBit();

	/**
	*	Reads an unsigned integer.
	*	@param uiNumBits Number of bits to read.
	*	@return Value that was read.
	*/
	unsigned int ReadUnsignedBitLong( const size_t uiNumBits );

	/**
	*	Reads an signed integer.
	*	@param uiNumBits Number of bits to read. This includes the sign bit.
	*	@return Value that was read.
	*/
	int ReadSignedBitLong( const size_t uiNumBits );

	/**
	*	Reads either a signed or unsigned integer.
	*	@param uiNumBits Number of bits to read.
	*	@param fSigned Whether the value is signed or unsigned.
	*	@return Value that was read.
	*/
	unsigned int ReadBitLong( const size_t uiNumBits, const bool fSigned )
	{
		if( fSigned )
			return ( unsigned int ) ReadSignedBitLong( uiNumBits );
		else
			return ReadUnsignedBitLong( uiNumBits );
	}

	/**
	*	Reads a float that was written as a signed integer.
	*	@return Value that was read.
	*/
	float ReadBitFloat();

	/**
	*	Reads a number of bits from the buffer.
	*	@param pBuffer Destination buffer to write the bits to.
	*	@param uiNumBits Number of bits to read.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ReadBits( void* pBuffer, const size_t uiNumBits );

	/**
	*	Reads a single character from the buffer.
	*	@return Character that was read.
	*/
	char ReadChar();

	/**
	*	Reads a single unsigned byte from the buffer.
	*	@return Value that was read.
	*/
	uint8_t ReadByte();

	/**
	*	Reads a single signed short from the buffer.
	*	@return Value that was read.
	*/
	short ReadShort();

	/**
	*	Reads a single unsigned short from the buffer.
	*	@return Value that was read.
	*/
	unsigned short ReadWord();

	/**
	*	Reads a single signed long from the buffer.
	*	@return Value that was read.
	*/
	int ReadLong();

	/**
	*	Reads a single float from the buffer. The float must have been written as a bit buffer.
	*	@return Value that was read.
	*/
	float ReadFloat();

	/**
	*	Reads a number of bytes from the buffer.
	*	@param pBuffer Buffer to write the bytes to.
	*	@param uiNumBytes Number of bytes to read.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ReadBytes( void* pBuffer, const size_t uiNumBytes );

	/**
	*	Reads a string from the buffer.
	*	@param pszBuffer Buffer to write the string to.
	*	@param uiBufferSize Size of the buffer.
	*	@param fIsLine Whether the entire string should be read, or everything up to the first newline or null terminator.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ReadString( char* pszBuffer, const size_t uiBufferSize, const bool fIsLine = false );

	/**
	*	Reads and discards a number of bits.
	*	@param uiNumBits Number of bits to read.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ReadAndDiscardBits( const size_t uiNumBits );

	/**
	*	Reads and discards a number of bytes.
	*	@param uiNumBytes Number of bytes to read.
	*	@return Whether the buffer overflowed after performing this operation.
	*/
	bool ReadAndDiscardBytes( const size_t uiNumBytes );

private:
	const char* m_pszDebugName;	//Debug name. Points to a static const string
	bool m_fOverflowed;			//Whether a read or write operation overflowed
	uint8_t* m_pData;			//Pointer to destination buffer
	size_t m_uiMaxBits;			//Size of the destination buffer, in bits
	size_t m_uiCurrentBit;		//Next bit we're reading from or writing to

private:
	CNetworkBuffer( const CNetworkBuffer& ) = delete;
	CNetworkBuffer& operator=( const CNetworkBuffer& ) = delete;
};

/** @} */

#endif //CNETWORKBUFFER_H