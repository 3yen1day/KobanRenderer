#pragma once

//外部
#include <memory>
#include <unordered_map>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <string>
#include <algorithm>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//Utillity
#include "Script/Utillity/StringUtil.h"
#include "Script/Utillity/DebugUtil.h"
#include "Script/Utillity/RenderUtil.h"

#include "Script/Include/Macro.h"
#include "Script/Core/GlobalAccess.h"