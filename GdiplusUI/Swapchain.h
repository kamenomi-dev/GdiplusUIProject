#pragma once
#ifndef __GDIPLUSUI_SWAPCHAIN_H__
#define __GDIPLUSUI_SWAPCHAIN_H__

#include <Windows.h>
#include <Windowsx.h>
#include <gdiplus.h>

#include <assert.h>
#include <memory>
#include <vector>

namespace GdiplusUI {

class Swapchain : INonCopyable {

  public:
  Swapchain(HWND hWindow);
  ~Swapchain();

  public:
  SIZE GetPresentSize() const;
  void UpdateRect();
  void UpdateRect(Gdiplus::Rect rect);

  std::shared_ptr<Gdiplus::Graphics> BeginPaint();
  void                               EndPaint(std::shared_ptr<Gdiplus::Graphics>&);

  bool Present();
  void InvalidateRect(Gdiplus::Rect = {});

  private:
  HBITMAP CreateMemoryBitmap(int dx, int dy);

  private:
  HWND  m_hWindow;
  SIZE  m_presentSize;
  POINT m_presentPoint;

  HDC     m_memoryDC;
  HBITMAP m_currBitmap;
  HBITMAP m_lastBitmap;

  std::vector<Gdiplus::Rect> m_pushedRectList;
};

} // namespace GdiplusUI

#endif // !__GDIPLUSUI_SWAPCHAIN_H__
