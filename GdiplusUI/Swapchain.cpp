#include "./GdiplusUI.h"

using namespace GdiplusUI;
using namespace GdiplusUI::Types;
using namespace GdiplusUI::Components;

using std::make_shared;
using std::shared_ptr;

Swapchain::Swapchain(HWND hWindow) {
  m_hWindow = hWindow;

  m_memoryDC = CreateCompatibleDC(NULL);
  if (m_memoryDC == NULL) {
    DebugBreak();
  }

  RECT rectWindow{};
  GetWindowRect(hWindow, &rectWindow);
  m_currBitmap = CreateMemoryBitmap(rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top);
  m_lastBitmap = SelectBitmap(m_memoryDC, m_currBitmap);

  if (m_currBitmap == NULL or m_lastBitmap == NULL) {
    DebugBreak();
  }
}

Swapchain::~Swapchain() {
  SelectBitmap(m_memoryDC, m_lastBitmap);
  DeleteBitmap(m_currBitmap);
  DeleteDC(m_memoryDC);

  m_currBitmap = m_lastBitmap = NULL;
  m_memoryDC                  = NULL;
}

SIZE GdiplusUI::Swapchain::GetPresentSize() const { return m_presentSize; }

void Swapchain::UpdateRect() {
  SelectBitmap(m_memoryDC, m_lastBitmap);
  DeleteBitmap(m_currBitmap);

  RECT rectWindow{};
  GetClientRect(m_hWindow, &rectWindow);
  m_presentSize.cx = rectWindow.right - rectWindow.left;
  m_presentSize.cy = rectWindow.bottom - rectWindow.top;
  m_currBitmap     = CreateMemoryBitmap(m_presentSize.cx, m_presentSize.cy);

  m_presentPoint.x = m_presentPoint.y = 0;

  SelectBitmap(m_memoryDC, m_currBitmap);
}

void Swapchain::UpdateRect(Rect rect) {
  SelectBitmap(m_memoryDC, m_lastBitmap);
  DeleteBitmap(m_currBitmap);

  m_currBitmap = CreateMemoryBitmap(rect.Width, rect.Height);

  m_presentSize.cx = rect.Width;
  m_presentSize.cy = rect.Height;

  m_presentPoint.x = rect.X;
  m_presentPoint.y = rect.Y;

  SelectBitmap(m_memoryDC, m_currBitmap);
}

shared_ptr<Graphics> Swapchain::BeginPaint() {
  /*
  Region presentRegion{};

  for (auto& rect : m_pushedRectList) {
    presentRegion.Union(rect);
  }

  auto ptrGraphics = new Graphics(m_memoryDC);

  if (not presentRegion.IsEmpty(ptrGraphics)) {
    ptrGraphics->SetClip(&presentRegion);
  }
  */

  auto ptrGraphics = new Graphics(m_memoryDC);

  return shared_ptr<Graphics>(ptrGraphics);
}

void Swapchain::EndPaint(shared_ptr<Graphics>& graphics) {
  graphics.reset();

  // As you didn't use ptr correctly, it will take place to out of memeory.
  assert(graphics.use_count() == NULL);
}

bool Swapchain::Present() {
  HDC wndDC = GetDC(m_hWindow);
  assert(wndDC != NULL);

  BOOL bResult =
      BitBlt(wndDC, m_presentPoint.x, m_presentPoint.y, m_presentSize.cx, m_presentSize.cy, m_memoryDC, 0, 0, SRCCOPY);

#ifdef _DEBUG
  DWORD lastError = GetLastError();
#endif

  assert(bResult);

  ReleaseDC(m_hWindow, wndDC);

  return bResult;
}

void Swapchain::InvalidateRect(Rect rect) {
  /* m_pushedRectList.push_back(rect); */
  ::InvalidateRect(m_hWindow, NULL, true);
}

HBITMAP Swapchain::CreateMemoryBitmap(int dx, int dy) {
  void*      ppvBits{};
  BITMAPINFO bitmapInfo{};
  bitmapInfo.bmiHeader.biBitCount    = 32;
  bitmapInfo.bmiHeader.biCompression = 0;
  bitmapInfo.bmiHeader.biPlanes      = 1;
  bitmapInfo.bmiHeader.biSize        = 40;
  bitmapInfo.bmiHeader.biWidth       = dx;
  bitmapInfo.bmiHeader.biHeight      = dy;

  return CreateDIBSection(m_memoryDC, &bitmapInfo, DIB_RGB_COLORS, &ppvBits, NULL, 0);
}
