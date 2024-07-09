#pragma once

#ifndef __GDIPLUSUI_H__
#define __GDIPLUSUI_H__

#include <Windows.h>
#include <gdiplus.h>

#include <algorithm>
#include <assert.h>
#include <string>
#include <unordered_map>

#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

extern void GuInitialize(_In_ HINSTANCE hInstance);
extern void GuUninitialize();

#include "./Common.h"
#include "./Swapchain.h"

#include "./Components/ComponentBase.h"
#include "./Components/Label.h"
#include "./Components/Window.h"

#include "./UIManager.h"

#endif // !__GDIPLUSUI_H__
