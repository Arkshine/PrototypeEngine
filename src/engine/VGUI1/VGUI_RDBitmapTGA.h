#ifndef ENGINE_VGUI1_VGUI_RDBITMAPTGA_H
#define ENGINE_VGUI1_VGUI_RDBITMAPTGA_H

#include <VGUI_BitmapTGA.h>

namespace vgui
{
/**
*	Resolution dependent bitmap. Scales with the resolution, where 640x480 is scale 1.
*/
class RDBitmapTGA : public BitmapTGA
{
public:
	using BitmapTGA::BitmapTGA;

	float GetXScale() const { return m_flXScale; }

	void SetXScale( float flX )
	{
		m_flXScale = flX;
	}

	float GetYScale() const { return m_flYScale; }

	void SetYScale( float flY )
	{
		m_flYScale = flY;
	}

protected:
	void drawFilledRect( int x0, int y0, int x1, int y1 ) override;
	void drawOutlinedRect( int x0, int y0, int x1, int y1 ) override;
	void drawSetTextPos( int x, int y ) override;
	void drawPrintText( int x, int y, const char* str, int strlen ) override;
	void drawPrintChar( int x, int y, char ch ) override;
	void drawTexturedRect( int x0, int y0, int x1, int y1 ) override;

	int ScaleX( int iX ) const;
	int ScaleY( int iY ) const;

private:
	float m_flXScale = 1;
	float m_flYScale = 1;
};
}

#endif //ENGINE_VGUI1_VGUI_RDBITMAPTGA_H
