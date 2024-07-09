#include "./GdiplusUI.h"

using namespace GdiplusUI;
using namespace GdiplusUI::Components;

UIManager* UIManager::m_pInstance = nullptr;

void UIManager::Initialize() {
  assert(m_pInstance == NULL && "You should not init this class twice! only single instance! ");
  m_pInstance = new UIManager();
};

void UIManager::UnInitialize() {
  assert(m_pInstance != NULL);
  delete m_pInstance;
  m_pInstance = nullptr;
}

UIManager::UIManager() {
  m_instanceWndMap = std::unordered_map<HWND, Components::Window*>();
  return;
}

UIManager::~UIManager() {
  for (auto& item : m_instanceWndMap) {
    delete item.second;
    item.second = nullptr;
  }
}

UIManager& UIManager::GetInstance() { return *UIManager::m_pInstance; }

void UIManager::UpdateWindow(HWND hWnd) {
  bool noop{};
  m_instanceWndMap[hWnd]->MessageEventProcessor(DID_ALL, WM_SIZE, NULL, NULL, noop);
}

Window* UIManager::AttachWindow(HWND hWnd, bool useAlpha, bool bReserveCaption) {
  InitInformation info{};
  info.hWindow   = hWnd;
  info.bUseAlpha = useAlpha;
  info.bReserveCaption = bReserveCaption;

  // m_instanceWndMap.insert({hWnd, pWindow});
  return (m_instanceWndMap[hWnd] = new Window(info));
}

LRESULT UIManager::MessageEventBus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

  auto window = m_instanceWndMap[hWnd];

  if (window != nullptr) {
    bool needReturn{};
    auto result = window->MessageEventProcessor(DID_ALL, uMsg, wParam, lParam, needReturn);
    if (needReturn) {
      return result;
    }
  }

  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
