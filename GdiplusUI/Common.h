#pragma once
#ifndef __GDIPLUSUI_COMMON_H_
#define __GDIPLUSUI_COMMON_H_

namespace GdiplusUI {

struct INonCopyable {
  public:
  INonCopyable()  = default;
  ~INonCopyable() = default;

  private:
  INonCopyable(const INonCopyable&)                  = delete;
  const INonCopyable& operator=(const INonCopyable&) = delete;
};

Rect  RectF2Rect(RectF rect);
RectF Rect2RectF(Rect rect);

Size  SizeF2Size(SizeF size);
SizeF Size2SizeF(Size size);

Point  PointF2Point(PointF size);
PointF Point2PointF(Point size);

} // namespace GdiplusUI

#endif // !__GDIPLUSUI_COMMON_H_