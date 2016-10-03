//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include <cstdio>
#include <memory>

#include "VGUI.h"
#include "vgui_loadtga.h"
#include "VGUI_InputStream.h"


// ---------------------------------------------------------------------- //
// Helper class for loading tga files.
// ---------------------------------------------------------------------- //
class MemoryInputStream : public vgui::InputStream
{
public:
				MemoryInputStream()
				{
					m_pData = nullptr;
					m_DataLen = m_ReadPos = 0;
				}
		
	virtual void  seekStart(bool& success)					{m_ReadPos=0; success=true;}
	virtual void  seekRelative(int count,bool& success)		{m_ReadPos+=count; success=true;}
	virtual void  seekEnd(bool& success)					{m_ReadPos=m_DataLen; success=true;}
	virtual int   getAvailable(bool& success)				{success=false; return 0;} // This is what vgui does for files...
	
	virtual uchar readUChar(bool& success)					
	{
		if(m_ReadPos>=0 && m_ReadPos<m_DataLen)
		{
			success=true;
			uchar ret = m_pData[m_ReadPos];
			++m_ReadPos;
			return ret;
		}
		else
		{
			success=false;
			return 0;
		}
	}

	virtual void  readUChar(uchar* buf,int count,bool& success)
	{
		for(int i=0; i < count; i++)
			buf[i] = readUChar(success);
	}

	virtual void  close(bool& success) 
	{
		m_pData = nullptr;
		m_DataLen = m_ReadPos = 0;
	}

	uchar		*m_pData;
	int			m_DataLen;
	int			m_ReadPos;
};

vgui::BitmapTGA* vgui_LoadTGA( char const *pFilename, const bool bInvertAlpha )
{
	//TODO: needs to handle SteamPipe. - Solokiller

	FILE* pFile = fopen( pFilename, "rb" );

	if( !pFile )
		return nullptr;

	fseek( pFile, 0, SEEK_END );

	const auto size = ftell( pFile );

	auto data = std::make_unique<uchar[]>( size );

	fseek( pFile, 0, SEEK_SET );

	const bool bSuccess = fread( data.get(), size, 1, pFile ) == 1;

	fclose( pFile );

	if( !bSuccess )
		return nullptr;

	MemoryInputStream stream;
	
	stream.m_pData = data.get();
	stream.m_ReadPos = 0;
	stream.m_DataLen = size;

	vgui::BitmapTGA *pRet = new vgui::BitmapTGA( &stream, bInvertAlpha );
	
	return pRet;
}
