#include "Engine.h"

#include "VGUI_RDBitmapTGA.h"

namespace vgui
{
void RDBitmapTGA::drawFilledRect( int x0, int y0, int x1, int y1 )
{
	BitmapTGA::drawFilledRect( 
		ScaleX( x0 ),
		ScaleY( y0 ),
		ScaleX( x1 ),
		ScaleY( y1 ) );
}

void RDBitmapTGA::drawOutlinedRect( int x0, int y0, int x1, int y1 )
{
	BitmapTGA::drawOutlinedRect(
		ScaleX( x0 ),
		ScaleY( y0 ),
		ScaleX( x1 ),
		ScaleY( y1 ) );
}

void RDBitmapTGA::drawSetTextPos( int x, int y )
{
	BitmapTGA::drawSetTextPos(
		ScaleX( x ),
		ScaleY( y ) );
}

void RDBitmapTGA::drawPrintText( int x, int y, const char* str, int strlen )
{
	BitmapTGA::drawPrintText(
		ScaleX( x ),
		ScaleY( y ),
		str,
		strlen );
}

void RDBitmapTGA::drawPrintChar( int x, int y, char ch )
{
	BitmapTGA::drawPrintChar(
		ScaleX( x ),
		ScaleY( y ),
		ch );
}

void RDBitmapTGA::drawTexturedRect( int x0, int y0, int x1, int y1 )
{
	BitmapTGA::drawTexturedRect( 
		ScaleX( x0 ),
		ScaleY( y0 ),
		ScaleX( x1 ),
		ScaleY( y1 ) );
}

int RDBitmapTGA::ScaleX( int iX ) const
{
	return static_cast<int>( ceil( iX * g_Video.GetXScale() * m_flXScale ) );
}

int RDBitmapTGA::ScaleY( int iY ) const
{
	return static_cast<int>( ceil( iY * g_Video.GetYScale() * m_flYScale ) );
}
}
