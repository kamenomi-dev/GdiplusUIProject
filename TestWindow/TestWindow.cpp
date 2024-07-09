// TestWindow.cpp : 定义应用程序的入口点。
//

#include "TestWindow.h"

#include "framework.h"

static HWND hMain{}, hSub{};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  WNDCLASSEXW wcex{};

  wcex.cbSize = sizeof(WNDCLASSEXW);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.lpszClassName = L"TestWndow";

  RegisterClassExW(&wcex);

  GuInitialize(hInstance);

  HWND hWnd =
      CreateWindowW(L"TestWndow", L"test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  auto hWnd1 =
      CreateWindowW(L"TestWndow", L"玩圆神", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  hMain = hWnd;
  hSub = hWnd1;

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  ShowWindow(hWnd1, nCmdShow);
  UpdateWindow(hWnd1);

  GdiplusUI::UIManager::GetInstance().AttachWindow(hWnd);

  MSG msg{};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  GuUninitialize();

  return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  static auto& uiManager = GdiplusUI::UIManager::GetInstance();

  if (message == WM_CREATE) {
    auto createStruct = (CREATESTRUCTW*)lParam;
    auto wnd = uiManager.AttachWindow(hWnd);

    auto label = new GdiplusUI::Components::Label(
        createStruct->hwndParent == NULL ? L"Owner Window" : L"Child Window");

    label->SetComponentRect({100, 100, 250, 40});
    label->SetTextAlign(StringAlignmentCenter);
    label->SetTextLineAlign(StringAlignmentCenter);
    label->SetBackgroundColor(Color(rand() % 255, rand() % 255, rand() % 255));
    wnd->SetChildren(wnd, label);
  }

  if (message == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }

  return uiManager.MessageEventBus(hWnd, message, wParam, lParam);
}
