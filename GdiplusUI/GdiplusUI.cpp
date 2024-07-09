#include "./GdiplusUI.h"

static ULONG_PTR pGdipToken = NULL;

void GuInitialize(_In_ HINSTANCE hInstance) {
  assert(pGdipToken == NULL && "GuInitialize function should not be called more times. ");

  GdiplusStartupInput startupInput{};
  GdiplusStartup(&pGdipToken, &startupInput, nullptr);

  GdiplusUI::UIManager::Initialize();
}

void GuUninitialize() {
  GdiplusShutdown(pGdipToken);
  pGdipToken = NULL;

  GdiplusUI::UIManager::UnInitialize();
}