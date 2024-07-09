#include "../GdiplusUI.h"

using namespace GdiplusUI;
using namespace GdiplusUI::Types;
using namespace GdiplusUI::Components;

using std::wstring;

Label::Label(wstring context) { m_context = wstring(context); }

Label::~Label() { m_context = L""; }

void Label::SetContext(wstring context) {
  m_context = wstring(context);
  ((Window*)GetRootWindow())->__GetSwapChain().InvalidateRect();
}

wstring Label::GetContext() { return m_context; }

bool Label::Render(Graphics& graphics) {
  DefaultRender(graphics);

  SolidBrush brush(GetFontColor());
  StringFormat format{};
  format.SetAlignment(GetTextAlign());
  format.SetLineAlignment(GetTextLineAlign());

  const auto componentSize = Size2SizeF(GetComponentSize());

  graphics.DrawString(
      m_context.c_str(),
      (int)m_context.length(),
      GetFont(),
      RectF(0.f, 0.f, componentSize.Width, componentSize.Height),
      &format,
      &brush
  );
  return true;
}
