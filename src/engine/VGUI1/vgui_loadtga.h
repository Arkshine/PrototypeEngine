//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VGUI_LOADTGA_H
#define VGUI_LOADTGA_H
#ifdef _WIN32
#pragma once
#endif


#include "VGUI_BitmapTGA.h"

/**
*	@file
*	Modified to load TGA files without using the engine API. - Solokiller
*/

vgui::BitmapTGA* vgui_LoadTGA( char const *pFilename, const bool bInvertAlpha = true );


#endif // VGUI_LOADTGA_H
