#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_KOREA, LEVEL_JAPAN, LEVEL_END };
enum ITEM { ITEM_COIN, ITEM_END };

extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;

namespace Client {}

using namespace Client;
