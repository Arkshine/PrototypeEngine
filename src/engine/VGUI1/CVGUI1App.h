#ifndef ENGINE_VGUI1_CVGUI1APP_H
#define ENGINE_VGUI1_CVGUI1APP_H

#include <VGUI_App.h>

/**
*	The VGUI1 app class.
*/
class CVGUI1App : public vgui::App
{
public:
	void main( int argc, char* argv[] ) override;

protected:
	void platTick() override;
};

#endif //ENGINE_VGUI1_CVGUI1APP_H
