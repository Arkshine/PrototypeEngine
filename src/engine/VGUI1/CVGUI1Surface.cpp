#include <SDL2/SDL.h>

#include "Engine.h"

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

int CVGUI1Surface::createNewTextureID()
{
	GLuint texture;

	glGenTextures( 1, &texture );

	return texture;
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
	glBindTexture( GL_TEXTURE_2D, id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, wide, tall, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void CVGUI1Surface::drawSetTexture( int id )
{
	m_CurrentTexture = id;

	glBindTexture( GL_TEXTURE_2D, id );
}

void CVGUI1Surface::drawTexturedRect( int x0, int y0, int x1, int y1 )
{
	if( m_CurrentTexture == 0 )
		return;

	glBegin( GL_TRIANGLE_STRIP );

		glTexCoord2f( 0, 0 );
		glVertex2i( x0, y0 );

		glTexCoord2f( 1, 0 );
		glVertex2i( x1, y0 );

		glTexCoord2f( 0, 1 );
		glVertex2i( x0, y1 );

		glTexCoord2f( 1, 1 );
		glVertex2i( x1, y1 );

	glEnd();
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
	glFinish();

	SDL_GL_SwapWindow( g_Engine.GetWindow() );
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
