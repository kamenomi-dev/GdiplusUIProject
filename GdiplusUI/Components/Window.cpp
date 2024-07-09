#include "../GdiplusUI.h"

using namespace GdiplusUI;
using namespace GdiplusUI::Types;
using namespace GdiplusUI::Components;

Window::Window(InitInformation initInformation) {
  m_initInformation = InitInformation(initInformation);
  m_renderSwapchain = new Swapchain(initInformation.hWindow);

  if (initInformation.bReserveCaption) {
    RECT frameRect{}, clientRect{};
    GetWindowRect(initInformation.hWindow, &frameRect);
    GetClientRect(initInformation.hWindow, &clientRect);

    m_hittestOffset        = Size();
    m_hittestOffset.Width  = 1;
    m_hittestOffset.Height = frameRect.bottom - frameRect.top - (clientRect.bottom - clientRect.top);
    cos(1);
  }
}

Window::~Window() {
  if (m_renderSwapchain != nullptr) {
    delete m_renderSwapchain;
    m_renderSwapchain = nullptr;
  }
}

Swapchain& Window::__GetSwapChain() const { return *m_renderSwapchain; }

InitInformation Window::GetInitInformation() const { return m_initInformation; }

ComponentBase* Window::ComponentHitTest(LPARAM lParam) { return _ComponentHitTest(this, lParam); }

LRESULT Window::BorderHitTest(unsigned __int32 uMsg, WPARAM wParam, LPARAM lParam) { return HTCLIENT; }

LRESULT Window::MessageEventProcessor(
    unsigned __int64 ComponentID,
    unsigned __int32 uMsg,
    WPARAM           wParam,
    LPARAM           lParam,
    bool&            NeedProcess
) {

  NeedProcess = false;

  // - - - - - - - - - - WM_SIZE - - - - - - - - - -

  if (uMsg == WM_SIZE) {
    m_renderSwapchain->UpdateRect();
    auto windowRect = m_renderSwapchain->GetPresentSize();

    SetComponentSize({windowRect.cx, windowRect.cy});
  }

  // - - - - - - - - - - WM_NCHITTEST - - - - - - - - - -

  const bool isReserveCaption = GetInitInformation().bReserveCaption;
  if (uMsg == WM_NCHITTEST) {

    POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    ScreenToClient(m_initInformation.hWindow, &point);

    lParam = MAKELPARAM(point.x, point.y);

    auto result = BorderHitTest(uMsg, wParam, lParam);
    if (result != HTCLIENT) {
      NeedProcess = true;
      return result;
    }

    auto hovered = ComponentHitTest(lParam);
    auto label   = GetChildren().begin();

    if (label != GetChildren().end())
      ((Label*)((*label._Ptr)->pComponent))
          ->SetContext(L"Current hovered component: " + std::to_wstring((long long)hovered));
    cos(1);
  }


  // - - - - - - - - - - WM_PAINT - - - - - - - - - -

  if (uMsg == WM_PAINT) {
    // Begin all component render function of a window.

    auto graphics = m_renderSwapchain->BeginPaint();

    RenderContextBlock context{};
    context.renderContext    = graphics.get();
    context.currentSwapChain = m_renderSwapchain;

    lParam = (LPARAM)&context;

    DefaultMessageEventProcessor(DID_ALL, CM_PAINT, NULL, lParam, NeedProcess);
    CallChildren(DID_ALL, CM_PAINT, NULL, lParam);

    m_renderSwapchain->EndPaint(graphics);
    m_renderSwapchain->Present();

    // End
    return NULL;
  }

  return DefaultMessageEventProcessor(ComponentID, uMsg, wParam, lParam, NeedProcess);
}

bool Window::Render(Graphics& graphics) {
  graphics.Clear(Color(32, 32, 32));
  return true;
}

ComponentBase* Window::_ComponentHitTest(ComponentBase* component, LPARAM lParam) {
  if (!component->GetComponentRect().Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) {
    return nullptr;
  }

  auto children = dynamic_cast<IParentable*>(component);
  if (children == nullptr) {
    return component;
  }

  for (const auto& child : children->GetChildren()) {
    auto ptrComponent = _ComponentHitTest(child->pComponent, lParam);
    if (ptrComponent) {
      return ptrComponent;
    }
  }

  return component;
}
