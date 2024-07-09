#pragma once

#ifndef __GDIPLUSUI_LABEL_H__
#define __GDIPLUSUI_LABEL_H__

namespace GdiplusUI {

namespace Components {

class Label : public ComponentBase, public ITextProtoypes {
  public:
  Label(std::wstring context);
  ~Label();

  public:

  void         SetContext(std::wstring context);
  std::wstring GetContext();

  bool Render(Graphics&);

  private:
  std::wstring m_context;
};

} // namespace Components
} // namespace GdiplusUI

#endif // !__GDIPLUSUI_LABEL_H__