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

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//Library
#include "Script/Library/StringLib.h"
#include "Script/Library/DebugLib.h"
#include "Script/Include/DefaultInclude.h"