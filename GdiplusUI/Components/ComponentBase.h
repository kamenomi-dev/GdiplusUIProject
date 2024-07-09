#pragma once

#ifndef __GDIPLUSUI_COMPONENTBASE_H__
#define __GDIPLUSUI_COMPONENTBASE_H__

namespace GdiplusUI {

namespace Types {

typedef struct __structRenderContextBlock {
  Graphics*  renderContext;
  Swapchain* currentSwapChain;
} RenderContextBlock, RCB;

} // namespace Types

namespace Components {
typedef enum __enumDefaultIDs { DID_ALL = 0, DID_SELF = 1 } DefaultIDs;

class ComponentBase : INonCopyable {
  public:
  ComponentBase();
  ~ComponentBase();

  public:
  virtual unsigned __int64 GetComponentID();
  ComponentBase*           GetComponentRoot();
  ComponentBase*           GetRootWindow();

  Size           GetComponentSize() const;
  void           SetComponentSize(Size newSize);
  Point          GetComponentPosition() const;
  void           SetComponentPosition(Point newPosition);
  Rect           GetComponentRect() const;
  void           SetComponentRect(Rect newRect);
  ComponentBase* GetParentComponent();
  ComponentBase* SetParentComponent(ComponentBase*);

  virtual Rect CalculateComponentRect(Graphics&);

  LRESULT MessageEventProcessor(
      unsigned __int64 ComponentID,
      unsigned __int32 uMsg,
      WPARAM           wParam,
      LPARAM           lParam,
      bool&            NeedProcess
  );
  LRESULT DefaultMessageEventProcessor(
      unsigned __int64 ComponentID,
      unsigned __int32 uMsg,
      WPARAM           wParam,
      LPARAM           lParam,
      bool&            NeedProcess
  );

  virtual bool Render(Graphics&) { return false; };

  private:
  Rect m_componentRect;

  ComponentBase* m_componentParent;
};

} // namespace Components

namespace Types {

typedef struct __structChildInformation {
  unsigned __int64           nLevel;
  Components::ComponentBase* pComponent;
} ChildInformation;

} // namespace Types

using namespace Types;

struct IParentable {
  public:
  IParentable();
  ~IParentable();

  public:
  std::vector<ChildInformation*>& GetChildren();
  void SetChildren(Components::ComponentBase*, Components::ComponentBase* ptr, unsigned __int64 level = 0);

  std::vector<LRESULT>
          CallChildren(unsigned __int64 ComponentID, unsigned __int32 message, WPARAM wParam, LPARAM lParam); // Todo
  LRESULT CallChild(
      Components::ComponentBase* ptr,
      unsigned __int64           ComponentID,
      unsigned __int32           message,
      WPARAM                     wParam,
      LPARAM                     lParam
  ); // Todo

  public:
  static void __SortChildren(IParentable*);
  static void __ReleaseSelf(IParentable*);

  private:
  std::vector<ChildInformation*> m_childrenList;
};

struct ICommonPrototypes {
  public:
  ICommonPrototypes();
  ~ICommonPrototypes();

  void  SetBackgroundColor(const Color);
  Color GetBackgroundColor() const;

  virtual void DefaultRender(Graphics&) = 0;
  virtual void CommonPrototypes_DefaultRender(Graphics&);

  Color m_backgroundColor;
};

struct ITextProtoypes : public ICommonPrototypes {
  public:
  ITextProtoypes();
  ~ITextProtoypes();

  public:
  void            SetTextAlign(StringAlignment);
  StringAlignment GetTextAlign() const;
  void            SetTextLineAlign(StringAlignment);
  StringAlignment GetTextLineAlign() const;

  void  SetFont(const Font&);
  Font* GetFont();
  void  SetFontSize(REAL);
  REAL  GetFontSize();
  void  SetFontColor(const Color);
  Color GetFontColor() const;

  void DefaultRender(Graphics&);

  private:
  StringAlignment m_textAlign;
  StringAlignment m_textLineAlign;

  Font* m_font;
  Color m_fontColor;
};

} // namespace GdiplusUI

#endif // !__GDIPLUSUI_COMPONENTBASE_H__
