#pragma once

#ifndef __GDIPLUSUI_UIMANAGER_H__
#define __GDIPLUSUI_UIMANAGER_H__

namespace GdiplusUI {

namespace Types {
typedef enum __enumMessage {
  CM_SIZE     = WM_SIZE,
  CM_HITTEST  = WM_NCHITTEST,
  CM_PAINT    = WM_PAINT,
  CM_SETCHILD = WM_APP
} EnumMessage;
} // namespace Types

using namespace Types;

class UIManager {
  public:
  static void Initialize();
  static void UnInitialize();
  ~UIManager();

  private:
  UIManager();

  public:

  static UIManager& GetInstance();

  void                UpdateWindow(HWND hWnd);
  Components::Window* AttachWindow(HWND hWnd, bool bUseAlpha = false, bool bReserveCaption = true);

  LRESULT MessageEventBus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  private:
  static UIManager* m_pInstance;

  std::unordered_map<HWND, Components::Window*> m_instanceWndMap;
};
} // namespace GdiplusUI

#endif // !__GDIPLUSUI_UIMANAGER_H__