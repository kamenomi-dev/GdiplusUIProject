#include "./GdiplusUI.h"

Rect GdiplusUI::RectF2Rect(RectF rect) { return Rect((int)rect.X, (int)rect.Y, (int)rect.Width, (int)rect.Height); };

RectF GdiplusUI::Rect2RectF(Rect rect) {
  return RectF((float)rect.X, (float)rect.Y, (float)rect.Width, (float)rect.Height);
};

Size GdiplusUI::SizeF2Size(SizeF size) { return Size((int)size.Width, (int)size.Height); }

SizeF GdiplusUI::Size2SizeF(Size size) { return SizeF((float)size.Width, (float)size.Height); }

Point GdiplusUI::PointF2Point(PointF size) { return Point((int)size.X, (int)size.Y); }

PointF GdiplusUI::Point2PointF(Point size) { return PointF((float)size.X, (float)size.Y); }
