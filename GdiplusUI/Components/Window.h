#pragma once

#ifndef __GDIPLUSUI_WINDOW_H__
#define __GDIPLUSUI_WINDOW_H__

namespace GdiplusUI {

namespace Types {
typedef struct __structWindowComponentInitializationInformation {
  HWND hWindow;
  bool bUseAlpha;
  bool bReserveCaption;
} InitInformation;
} // namespace Types

using namespace Types;

namespace Components {
class Window : public ComponentBase, public IParentable {
  public:
  Window(InitInformation initInformation);
  ~Window();

  public:
  Swapchain&      __GetSwapChain() const;
  InitInformation GetInitInformation() const;

  ComponentBase* ComponentHitTest(LPARAM lParam);
  LRESULT        BorderHitTest(unsigned __int32 uMsg, WPARAM wParam, LPARAM lParam);
  LRESULT        MessageEventProcessor(
             unsigned __int64 ComponentID,
             unsigned __int32 uMsg,
             WPARAM           wParam,
             LPARAM           lParam,
             bool&            NeedProcess
         );
  bool Render(Graphics&);

  private:
  ComponentBase* _ComponentHitTest(ComponentBase* component, LPARAM lParam);

  protected:
  InitInformation m_initInformation;

  private:
  Size       m_hittestOffset;
  Swapchain* m_renderSwapchain;
};
} // namespace Components
} // namespace GdiplusUI

#endif // !__GDIPLUSUI_WINDOW_H__