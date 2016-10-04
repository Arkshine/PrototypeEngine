//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VGUI_BASEFONTPLAT_H
#define VGUI_BASEFONTPLAT_H

#include <VGUI.h>

namespace vgui
{
class SurfacePlat;

/**
*	Platform font interface.
*/
class VGUIAPI BaseFontPlat
{
public:
	virtual ~BaseFontPlat() = 0;

	virtual bool equals( const char* name, int tall, int wide, float rotation, int weight, bool italic, bool underline ) = 0;

	virtual void getCharRGBA( int ch, int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, uchar* rgba ) = 0;
	virtual void getCharABCwide( int ch, int& a, int& b, int& c ) = 0;
	virtual int  getTall() = 0;
	//TODO: might be linux only
	virtual int getWide() = 0;

	virtual void drawSetTextFont( SurfacePlat* plat ) = 0;
};

inline BaseFontPlat::~BaseFontPlat()
{
}
}

#endif //VGUI_BASEFONTPLAT_H
