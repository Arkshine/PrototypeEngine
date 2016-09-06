#include <cstring>

#include "Logging.h"

#include "CCommandBuffer.h"

bool CCommandBuffer::Initialize()
{
	m_Buffer.SetBuffer( "CommandBuffer", m_Data, sizeof( m_Data ) );

	return true;
}

bool CCommandBuffer::AddText( const char* const pszText )
{
	const size_t uiLength = strlen( pszText );

	if( m_Buffer.GetBytesInBuffer() + 1 >= m_Buffer.GetMaxBytes() )
	{
		Msg( "CCommandBuffer::AddText: Overflow\n" );
		return false;
	}

	m_Buffer.WriteBytes( pszText, uiLength );

	return true;
}

bool CCommandBuffer::InsertText( const char* const pszText )
{
	//Copy off any commands still remaining in the exec buffer.
	const size_t uiTempLen = m_Buffer.GetBytesInBuffer();

	char* pszTemp;

	if( uiTempLen )
	{
		pszTemp = new char[ uiTempLen ];

		memcpy( pszTemp, m_Buffer.GetData(), uiTempLen );

		m_Buffer.ResetToStart();
	}
	else
	{
		pszTemp = nullptr; //Shut up compiler.
	}

	//Add the entire text of the file.
	AddText( pszText );

	//Add the copied off data.
	if( uiTempLen )
	{
		m_Buffer.WriteBytes( pszTemp, uiTempLen );
		delete[] pszTemp;
	}

	return true;
}

bool CCommandBuffer::Execute()
{
	char szLine[ 1024 ];

	const char* pszText;

	size_t uiIndex;

	bool bQuotes;

	while( m_Buffer.GetBytesInBuffer() )
	{
		pszText = reinterpret_cast<const char*>( m_Buffer.GetData() );

		bQuotes = false;

		for( uiIndex = 0; uiIndex < m_Buffer.GetBytesInBuffer(); ++uiIndex )
		{
			if( pszText[ uiIndex ] == '\"' )
			{
				bQuotes = !bQuotes;
			}

			//Don't break if inside a quoted string.
			if( !bQuotes && pszText[ uiIndex ] == ';' )
				break;

			if( pszText[ uiIndex ] == '\n' )
				break;
		}

		//TODO: bounds checking. - Solokiller
		memcpy( szLine, pszText, uiIndex );

		szLine[ uiIndex ] = '\0';

		//Delete the text from the command buffer and move remaining commands down.
		//This is necessary because commands (exec, alias) can insert data at the 
		//beginning of the text buffer.

		if( uiIndex == m_Buffer.GetBytesInBuffer() )
			m_Buffer.ResetToStart();
		else
		{
			++uiIndex;
			m_Buffer.EraseBytes( 0, uiIndex );
		}

		//Execute the command line.
		//TODO
		//m_pCVar->ExecuteString( szLine, cvar::Source::Command );

		if( m_bWait )
		{
			//Skip out while text still remains in buffer, leaving it
			//for next frame.
			m_bWait = false;
			break;
		}
	}

	return true;
}
