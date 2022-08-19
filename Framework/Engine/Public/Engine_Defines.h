#pragma once

#pragma warning (disable : 4251)


#include <d3d11.h>
#include <DirectXMath.h>
#include <typeinfo>

#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <DirectXCollision.h>

#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>

#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>
#include <wincodec.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace DirectX;
using namespace Assimp;


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <typeinfo>
#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

//#ifdef _DEBUG
//#define USE_IMGUI
//#endif // _DEBUG

#ifndef USE_IMGUI
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG
#endif // !(USE_IMGUI)

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"

#include <tchar.h>

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <io.h>
#include "../../fmod/fmod.h"
#pragma comment(lib, "../../fmod/fmod_vc.lib")

using namespace Engine;
