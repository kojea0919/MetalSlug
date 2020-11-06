#pragma once

#include "GameEngine.h"
#include "resource.h"

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "GameEngine_Debug.lib")
#else
#pragma comment(lib, "GameEngine.lib")
#endif // _DEBUG

#else

#ifdef _DEBUG
#pragma comment(lib, "GameEngine32_Debug.lib")
#else
#pragma comment(lib, "GameEngine32.lib")
#endif // _DEBUG

#endif // _WIN64

