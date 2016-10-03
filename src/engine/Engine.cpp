#include "VGUI1/CVGUI1App.h"

#include "Engine.h"

CCommandBuffer g_CommandBuffer;

cvar::CCVarSystem g_CVar;

CEngine g_Engine;

//Constructor self registers itself for retrieval with vgui::App::getInstance.
static CVGUI1App g_VGUI1App;

CVGUI1Surface* g_pVGUI1Surface = nullptr;