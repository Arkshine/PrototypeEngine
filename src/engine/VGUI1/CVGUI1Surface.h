#ifndef ENGINE_VGUI1_CVGUI1SURFACE_H
#define ENGINE_VGUI1_CVGUI1SURFACE_H

#include <GL/glew.h>

#include <VGUI_SurfaceBase.h>

class CVGUI1Surface : public vgui::SurfaceBase
{
public:
	using SurfaceBase::SurfaceBase;

	void setTitle( const char* title ) override;
	bool setFullscreenMode( int wide, int tall, int bpp ) override;
	void setWindowedMode() override;
	void setAsTopMost( bool state ) override;
	void createPopup( vgui::Panel* embeddedPanel ) override;
	bool hasFocus() override;
	bool isWithin( int x, int y ) override;
	int  createNewTextureID( void ) override;
	void GetMousePos( int &x, int &y ) override;

	void drawSetColor( int r, int g, int b, int a ) override;
	void drawFilledRect( int x0, int y0, int x1, int y1 ) override;
	void drawOutlinedRect( int x0, int y0, int x1, int y1 ) override;
	void drawSetTextFont( vgui::Font* font ) override;
	void drawSetTextColor( int r, int g, int b, int a ) override;
	void drawSetTextPos( int x, int y ) override;
	void drawPrintText( const char* text, int textLen ) override;
	void drawSetTextureRGBA( int id, const char* rgba, int wide, int tall ) override;
	void drawSetTexture( int id ) override;
	void drawTexturedRect( int x0, int y0, int x1, int y1 ) override;
	void invalidate( vgui::Panel *panel ) override;
	void enableMouseCapture( bool state ) override;
	void setCursor( vgui::Cursor* cursor ) override;
	void swapBuffers() override;
	void pushMakeCurrent( vgui::Panel* panel, bool useInsets ) override;
	void popMakeCurrent( vgui::Panel* panel ) override;
	void applyChanges() override;

private:
	GLuint m_CurrentTexture = 0;
};

#endif //ENGINE_VGUI1_CVGUI1SURFACE_H
