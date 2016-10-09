#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <utility>

#include "CNetworkBuffer.h"

static unsigned int BitWriteMasks[ 32 ][ 33 ];
static unsigned int ExtraMasks[ 32 ];

void CNetworkBuffer::InitMasks()
{
	unsigned int startbit, endbit;
	unsigned int maskBit, nBitsLeft;

	for( startbit = 0; startbit < 32; startbit++ )
	{
		for( nBitsLeft = 0; nBitsLeft < 33; nBitsLeft++ )
		{
			endbit = startbit + nBitsLeft;

			BitWriteMasks[ startbit ][ nBitsLeft ] = ( 1 << startbit ) - 1;
			if( endbit < 32 ) BitWriteMasks[ startbit ][ nBitsLeft ] |= ~( ( 1 << endbit ) - 1 );
		}
	}

	for( maskBit = 0; maskBit < 32; maskBit++ )
		ExtraMasks[ maskBit ] = ( 1 << maskBit ) - 1;
}

CNetworkBuffer::CNetworkBuffer()
{
	ResetToEmpty();
}

CNetworkBuffer::CNetworkBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit )
	: m_pszDebugName( pszDebugName )
	, m_fOverflowed( false )
	, m_pData( pData )
	, m_uiMaxBits( ByteBit( uiNumBytes ) )
	, m_uiCurrentBit( uiCurrentBit )
{
}

void CNetworkBuffer::ResetToEmpty()
{
	SetBuffer( nullptr, nullptr, 0, 0 );
}

void CNetworkBuffer::ResetToStart()
{
	m_fOverflowed = false;
	m_uiCurrentBit = 0;
}

void CNetworkBuffer::SetBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit )
{
	m_pszDebugName	= pszDebugName;
	m_fOverflowed	= false;
	m_pData			= pData;
	m_uiMaxBits		= ByteBit( uiNumBytes );
	m_uiCurrentBit	= uiCurrentBit;
}

bool CNetworkBuffer::CheckOverflow( const size_t uiBits ) const
{
	return m_uiCurrentBit + uiBits > m_uiMaxBits;
}

bool CNetworkBuffer::Overflow( const size_t uiBits )
{
	if( CheckOverflow( uiBits ) )
	{
#ifdef _DEBUG
		printf( "Overflowed buffer \"%s\": Max size %u bits, Current %u, Attempted to write %u bits\n", 
				GetDebugName(), m_uiMaxBits, m_uiCurrentBit, uiBits );
#endif

		m_fOverflowed = true;
		m_uiCurrentBit = m_uiMaxBits;	//Prevent any more reads or writes
	}

	return m_fOverflowed;
}

void CNetworkBuffer::ClearOverflow()
{
	if( m_fOverflowed )
		m_fOverflowed = false;
}

void CNetworkBuffer::WriteOneBit( const int iValue )
{
	if( Overflow( 1 ) )
		return;

	if( iValue )
	{
		m_pData[ m_uiCurrentBit >> 3 ] |= ( 1 << ( m_uiCurrentBit & 7 ) );
	}
	else
	{
		m_pData[ m_uiCurrentBit >> 3 ] &= ~( 1 << ( m_uiCurrentBit & 7 ) );
	}

	++m_uiCurrentBit;
}

void CNetworkBuffer::WriteUnsignedBitLong( unsigned int iValue, const size_t uiNumBits )
{
	assert( uiNumBits <= 32 );

	if( Overflow( uiNumBits ) )
		return;

	size_t uiBitsLeft = uiNumBits;
	size_t uiCurBit = m_uiCurrentBit;
	unsigned int iDWord = uiCurBit >> 5;	// Mask in a dword.
	unsigned int iCurBitMasked;
	size_t uiBitsWritten;

	assert( ( iDWord * 4 + sizeof( long ) ) <= ( unsigned int ) GetMaxBytes() );

	iCurBitMasked = uiCurBit & 31;
	( ( unsigned int* ) m_pData )[ iDWord ] &= BitWriteMasks[ iCurBitMasked ][ uiBitsLeft ];
	( ( unsigned int* ) m_pData )[ iDWord ] |= iValue << iCurBitMasked;

	// did it span a dword?
	uiBitsWritten = 32 - iCurBitMasked;

	if( uiBitsWritten < uiBitsLeft )
	{
		uiBitsLeft -= uiBitsWritten;
		uiCurBit += uiBitsWritten;
		iValue >>= uiBitsWritten;

		iCurBitMasked = uiCurBit & 31;
		( ( unsigned int* ) m_pData )[ iDWord + 1 ] &= BitWriteMasks[ iCurBitMasked ][ uiBitsLeft ];
		( ( unsigned int* ) m_pData )[ iDWord + 1 ] |= iValue << iCurBitMasked;
	}

	m_uiCurrentBit += uiNumBits;
}

void CNetworkBuffer::WriteSignedBitLong( int iValue, const size_t uiNumBits )
{
	// do we have a valid # of bits to encode with?
	assert( uiNumBits >= 1 );

	// NOTE: it does this wierdness here so it's bit-compatible with regular integer data in the buffer.
	if( iValue < 0 )
	{
		WriteUnsignedBitLong( ( unsigned int ) ( 0x80000000 + iValue ), uiNumBits - 1 );
		WriteOneBit( 1 );
	}
	else
	{
		WriteUnsignedBitLong( ( unsigned int ) iValue, uiNumBits - 1 );
		WriteOneBit( 0 );
	}
}

bool CNetworkBuffer::WriteBits( const void* pData, const size_t uiNumBits )
{
	auto pOut = ( const uint8_t* ) pData;
	size_t uiBitsLeft = uiNumBits;

	// get output dword-aligned.
	while( ( ( unsigned int ) pOut & 3 ) != 0 && uiBitsLeft >= 8 )
	{
		WriteUnsignedBitLong( *pOut, 8 );

		uiBitsLeft -= 8;
		++pOut;
	}

	// read dwords.
	while( uiBitsLeft >= 32 )
	{
		WriteUnsignedBitLong( *( ( const unsigned int* ) pOut ), 32 );

		pOut += sizeof( unsigned int );
		uiBitsLeft -= 32;
	}

	// read the remaining bytes.
	while( uiBitsLeft >= 8 )
	{
		WriteUnsignedBitLong( *pOut, 8 );

		uiBitsLeft -= 8;
		++pOut;
	}

	// Read the remaining bits.
	if( uiBitsLeft )
	{
		WriteUnsignedBitLong( *pOut, uiBitsLeft );
	}

	return !m_fOverflowed;
}

void CNetworkBuffer::WriteBitFloat( const float flValue )
{
	int iVal = *( ( int * ) &flValue );

	WriteSignedBitLong( iVal, 32 );
}

void CNetworkBuffer::WriteChar( const int iValue )
{
	WriteSignedBitLong( iValue, sizeof( char ) << 3 );
}

void CNetworkBuffer::WriteByte( const int iValue )
{
	WriteUnsignedBitLong( ( unsigned int ) iValue, sizeof( uint8_t ) << 3 );
}

void CNetworkBuffer::WriteShort( const int iValue )
{
	WriteSignedBitLong( iValue, sizeof( short ) << 3 );
}

void CNetworkBuffer::WriteWord( const int iValue )
{
	WriteUnsignedBitLong( iValue, sizeof( unsigned short ) << 3 );
}

void CNetworkBuffer::WriteLong( const int iValue )
{
	WriteSignedBitLong( iValue, sizeof( int ) << 3 );
}

void CNetworkBuffer::WriteFloat( const float flValue )
{
	WriteBits( &flValue, sizeof( float ) << 3 );
}

bool CNetworkBuffer::WriteBytes( const void* pData, const size_t uiNumBytes )
{
	return WriteBits( pData, uiNumBytes << 3 );
}

bool CNetworkBuffer::WriteString( const char* pszString )
{
	if( pszString )
	{
		do
		{
			WriteChar( *pszString );
		}
		while( *pszString++ );
	}
	else
		WriteChar( 0 );

	return !m_fOverflowed;
}

bool CNetworkBuffer::PadToByte()
{
	if( ( m_uiCurrentBit % 8 ) != 0 )
	{
		m_uiCurrentBit += m_uiCurrentBit % 8;

		return !HasOverflowed();
	}

	return true;
}

bool CNetworkBuffer::ExternalBitsWritten( const size_t uiBits )
{
	if( Overflow( uiBits ) )
		return false;

	m_uiCurrentBit += uiBits;

	return true;
}

bool CNetworkBuffer::ExternalBytesWritten( const size_t uiBytes )
{
	return ExternalBitsWritten( ByteBit( uiBytes ) );
}

bool CNetworkBuffer::BackUpBits( const size_t uiBits )
{
	m_uiCurrentBit -= std::min( m_uiCurrentBit, uiBits );

	return true;
}

bool CNetworkBuffer::BackUpBytes( const size_t uiBytes )
{
	return BackUpBits( ByteBit( uiBytes ) );
}

void CNetworkBuffer::EraseBytes( size_t uiStart, size_t uiCount )
{
	if( uiStart >= GetBytesInBuffer() )
		return;

	if( uiStart + uiCount >= GetBytesInBuffer() )
	{
		uiCount = GetBytesInBuffer() - uiStart;
	}

	memmove( m_pData + uiStart, m_pData + uiStart + uiCount, GetBytesInBuffer() - ( uiStart + uiCount ) );

	m_uiCurrentBit -= ByteBit( uiStart + uiCount );
}

int CNetworkBuffer::ReadOneBit()
{
	if( Overflow( 1 ) )
		return false;

	const int iValue = ( m_pData[ m_uiCurrentBit >> 3 ] & ( 1 << ( m_uiCurrentBit & 7 ) ) );

	++m_uiCurrentBit;

	return !!iValue;
}

unsigned int CNetworkBuffer::ReadUnsignedBitLong( const size_t uiNumBits )
{
	int	idword1;
	unsigned int dword1, ret;

	if( uiNumBits == 8 )
	{
		int leftBits = GetBitsLeft();

		if( leftBits >= 0 && leftBits < 8 )
			return 0;	// end of message
	}

	if( Overflow( uiNumBits ) )
	{
		return 0;
	}

	assert( uiNumBits > 0 && uiNumBits <= 32 );

	// Read the current dword.
	idword1 = m_uiCurrentBit >> 5;
	dword1 = ( ( unsigned int* ) m_pData )[ idword1 ];
	dword1 >>= ( m_uiCurrentBit & 31 );	// get the bits we're interested in.

	m_uiCurrentBit += uiNumBits;
	ret = dword1;

	// Does it span this dword?
	if( ( m_uiCurrentBit - 1 ) >> 5 == idword1 )
	{
		if( uiNumBits != 32 )
			ret &= ExtraMasks[ uiNumBits ];
	}
	else
	{
		size_t uiExtraBits = m_uiCurrentBit & 31;
		unsigned int dword2 = ( ( unsigned int* ) m_pData )[ idword1 + 1 ] & ExtraMasks[ uiExtraBits ];

		// no need to mask since we hit the end of the dword.
		// shift the second dword's part into the high bits.
		ret |= ( dword2 << ( uiNumBits - uiExtraBits ) );
	}

	return ret;
}

int CNetworkBuffer::ReadSignedBitLong( const size_t uiNumBits )
{
	int r = ReadUnsignedBitLong( uiNumBits - 1 );

	// NOTE: it does this wierdness here so it's bit-compatible with regular integer data in the buffer.
	const int sign = ReadOneBit();
	if( sign ) r = -( ( 1 << ( uiNumBits - 1 ) ) - r );

	return r;
}

float CNetworkBuffer::ReadBitFloat()
{
	long	val;
	int	bit, byte;

	assert( sizeof( float ) == sizeof( long ) );
	assert( sizeof( float ) == 4 );

	if( Overflow( 32 ) )
		return 0.0f;

	bit = m_uiCurrentBit & 0x7;
	byte = m_uiCurrentBit >> 3;

	val = m_pData[ byte ] >> bit;
	val |= ( ( int ) m_pData[ byte + 1 ] ) << ( 8 - bit );
	val |= ( ( int ) m_pData[ byte + 2 ] ) << ( 16 - bit );
	val |= ( ( int ) m_pData[ byte + 3 ] ) << ( 24 - bit );

	if( bit != 0 )
		val |= ( ( int ) m_pData[ byte + 4 ] ) << ( 32 - bit );
	m_uiCurrentBit += 32;

	return *( ( float * ) &val );
}

bool CNetworkBuffer::ReadBits( void* pBuffer, const size_t uiNumBits )
{
	uint8_t *pOut = ( uint8_t* ) pBuffer;
	size_t uiBitsLeft = uiNumBits;

	// get output dword-aligned.
	while( ( ( unsigned int ) pOut & 3 ) != 0 && uiBitsLeft >= 8 )
	{
		*pOut = ( uint8_t ) ReadUnsignedBitLong( 8 );
		++pOut;
		uiBitsLeft -= 8;
	}

	// read dwords.
	while( uiBitsLeft >= 32 )
	{
		*( ( unsigned int* ) pOut ) = ReadUnsignedBitLong( 32 );
		pOut += sizeof( unsigned int );
		uiBitsLeft -= 32;
	}

	// read the remaining bytes.
	while( uiBitsLeft >= 8 )
	{
		*pOut = ReadUnsignedBitLong( 8 );
		++pOut;
		uiBitsLeft -= 8;
	}

	// read the remaining bits.
	if( uiBitsLeft )
	{
		*pOut = ReadUnsignedBitLong( uiBitsLeft );
	}

	return !m_fOverflowed;
}

char CNetworkBuffer::ReadChar()
{
	return ReadSignedBitLong( sizeof( char ) << 3 );
}

uint8_t CNetworkBuffer::ReadByte()
{
	return ReadUnsignedBitLong( sizeof( uint8_t ) << 3 );
}

short CNetworkBuffer::ReadShort()
{
	return ReadSignedBitLong( sizeof( short ) << 3 );
}

unsigned short CNetworkBuffer::ReadWord()
{
	return ReadUnsignedBitLong( sizeof( unsigned short ) << 3 );
}

int CNetworkBuffer::ReadLong()
{
	return ReadSignedBitLong( sizeof( int ) << 3 );
}

float CNetworkBuffer::ReadFloat()
{
	float	ret;
	assert( sizeof( ret ) == 4 );

	ReadBits( &ret, 32 );

	return ret;
}

bool CNetworkBuffer::ReadBytes( void* pBuffer, const size_t uiNumBytes )
{
	return ReadBits( pBuffer, uiNumBytes << 3 );
}

bool CNetworkBuffer::ReadString( char* pszBuffer, const size_t uiBufferSize, const bool fIsLine )
{
	assert( pszBuffer );
	assert( uiBufferSize > 0 );

	size_t uiLength = 0;

	int c;

	do
	{
		// use BF_ReadByte so -1 is out of bounds
		c = ReadByte();

		if( c == 0 )
			break;
		else if( fIsLine && c == '\n' )
			break;

		// translate all fmt spec to avoid crash bugs
		// NOTE: but game strings leave unchanged. see pfnWriteString for details
		if( c == '%' ) c = '.';

		pszBuffer[ uiLength ] = c;
		++uiLength;
	}
	while( uiLength < uiBufferSize - 1 );

	pszBuffer[ uiLength ] = 0; // terminator

	return ( *pszBuffer ) != '\0';
}

bool CNetworkBuffer::ReadAndDiscardBits( const size_t uiNumBits )
{
	if( Overflow( uiNumBits ) )
		return false;

	m_uiCurrentBit += uiNumBits;

	return !m_fOverflowed;
}

bool CNetworkBuffer::ReadAndDiscardBytes( const size_t uiNumBytes )
{
	return ReadAndDiscardBits( ByteBit( uiNumBytes ) );
}