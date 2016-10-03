#include "CVGUI1Surface.h"

void CVGUI1Surface::setTitle( const char* title )
{
}

bool CVGUI1Surface::setFullscreenMode( int wide, int tall, int bpp )
{
	return false;
}

void CVGUI1Surface::setWindowedMode()
{
}

void CVGUI1Surface::setAsTopMost( bool state )
{
}

void CVGUI1Surface::createPopup( vgui::Panel* embeddedPanel )
{
}

bool CVGUI1Surface::hasFocus()
{
	return false;
}

bool CVGUI1Surface::isWithin( int x, int y )
{
	return false;
}

int CVGUI1Surface::createNewTextureID( void )
{
	return 0;
}

void CVGUI1Surface::GetMousePos( int &x, int &y )
{
}

void CVGUI1Surface::drawSetColor( int r, int g, int b, int a )
{
}

void CVGUI1Surface::drawFilledRect( int x0, int y0, int x1, int y1 )
{
}

void CVGUI1Surface::drawOutlinedRect( int x0, int y0, int x1, int y1 )
{
}

void CVGUI1Surface::drawSetTextFont( vgui::Font* font )
{
}

void CVGUI1Surface::drawSetTextColor( int r, int g, int b, int a )
{
}

void CVGUI1Surface::drawSetTextPos( int x, int y )
{
}

void CVGUI1Surface::drawPrintText( const char* text, int textLen )
{
}

void CVGUI1Surface::drawSetTextureRGBA( int id, const char* rgba, int wide, int tall )
{
}

void CVGUI1Surface::drawSetTexture( int id )
{
}

void CVGUI1Surface::drawTexturedRect( int x0, int y0, int x1, int y1 )
{
}

void CVGUI1Surface::invalidate( vgui::Panel *panel )
{
}

void CVGUI1Surface::enableMouseCapture( bool state )
{
}

void CVGUI1Surface::setCursor( vgui::Cursor* cursor )
{
}

void CVGUI1Surface::swapBuffers()
{
}

void CVGUI1Surface::pushMakeCurrent( vgui::Panel* panel, bool useInsets )
{
}

void CVGUI1Surface::popMakeCurrent( vgui::Panel* panel )
{
}

void CVGUI1Surface::applyChanges()
{
}
