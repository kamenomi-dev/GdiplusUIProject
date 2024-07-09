#include "../GdiplusUI.h"

using namespace GdiplusUI;
using namespace GdiplusUI::Types;
using namespace GdiplusUI::Components;

using std::vector;

ComponentBase::ComponentBase() {
  m_componentRect   = Rect();
  m_componentParent = nullptr;
}

ComponentBase::~ComponentBase() {}

unsigned __int64 ComponentBase::GetComponentID() { return (unsigned __int64)this; }

ComponentBase* ComponentBase::GetComponentRoot() {
  if (m_componentParent != nullptr) {
    return m_componentParent->GetComponentRoot();
  }
  return this;
}

ComponentBase* ComponentBase::GetRootWindow() { return GetComponentRoot(); }

Size ComponentBase::GetComponentSize() const {
  Size size{};
  m_componentRect.GetSize(&size);
  return size;
}

void ComponentBase::SetComponentSize(Gdiplus::Size newSize) {
  m_componentRect.Width  = newSize.Width;
  m_componentRect.Height = newSize.Height;
}

Point ComponentBase::GetComponentPosition() const {
  Point pos{};
  m_componentRect.GetLocation(&pos);
  return pos;
}

void ComponentBase::SetComponentPosition(Gdiplus::Point newPosition) {
  m_componentRect.X = newPosition.X;
  m_componentRect.Y = newPosition.Y;
}

Rect ComponentBase::GetComponentRect() const { return m_componentRect; };

void ComponentBase::SetComponentRect(Rect newRect) { m_componentRect = Rect(newRect); }

ComponentBase* ComponentBase::GetParentComponent() { return m_componentParent; };

ComponentBase* ComponentBase::SetParentComponent(ComponentBase* parent) {
  auto lastParent   = m_componentParent;
  m_componentParent = parent;
  return lastParent;
}

Rect ComponentBase::CalculateComponentRect(Graphics&) { return GetComponentRect(); }

LRESULT ComponentBase::MessageEventProcessor(
    unsigned __int64 ComponentID,
    unsigned __int32 uMsg,
    WPARAM           wParam,
    LPARAM           lParam,
    bool&            NeedProcess
) {

  if (uMsg == WM_PAINT) {
    auto  context   = (RenderContextBlock*)(void*)lParam;
    auto& graphics  = *context->renderContext;
    auto& swapchain = *context->currentSwapChain;

    auto status = graphics.BeginContainer();

    auto rect = GetComponentRect(); // CalculateComponentRect(graphics);
    graphics.SetClip(rect);

    auto position = GetComponentPosition();
    graphics.TranslateTransform((float)position.X, (float)position.Y);

    if (this->Render(graphics)) {
      swapchain.InvalidateRect(rect);
    }

    graphics.EndContainer(status);
  }

  NeedProcess = false;
  return NULL;
}

LRESULT ComponentBase::DefaultMessageEventProcessor(
    unsigned __int64 ComponentID,
    unsigned __int32 uMsg,
    WPARAM           wParam,
    LPARAM           lParam,
    bool&            NeedProcess
) {
  return ((ComponentBase*)this)->MessageEventProcessor(ComponentID, uMsg, wParam, lParam, NeedProcess);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

IParentable::IParentable() { m_childrenList = vector<ChildInformation*>(); }

IParentable::~IParentable() { IParentable::__ReleaseSelf(this); }

std::vector<ChildInformation*>& GdiplusUI::IParentable::GetChildren() { return m_childrenList; }

void IParentable::SetChildren(ComponentBase* parent, ComponentBase* child, unsigned __int64 level) {
  auto info        = new ChildInformation;
  info->nLevel     = level;
  info->pComponent = child;

  m_childrenList.push_back(info);
  child->SetParentComponent((ComponentBase*)parent);
  IParentable::__SortChildren(this);
}

vector<LRESULT>
IParentable::CallChildren(unsigned __int64 ComponentID, unsigned __int32 message, WPARAM wParam, LPARAM lParam) {
  vector<LRESULT> childrenResult{};
  for (auto& child : m_childrenList) {
    childrenResult.push_back(CallChild(child->pComponent, ComponentID, message, wParam, lParam));
  }

  return childrenResult;
};

LRESULT IParentable::CallChild(
    Components::ComponentBase* ptr,
    unsigned __int64           ComponentID,
    unsigned __int32           message,
    WPARAM                     wParam,
    LPARAM                     lParam
) {
  auto needRet = false;
  auto result  = ptr->MessageEventProcessor(ComponentID, message, wParam, lParam, needRet);

  const auto currID = ptr->GetComponentID();
  if (currID == DID_ALL or currID == DID_SELF or currID == ComponentID) {
    if (needRet) return result;
  }

  return NULL;
}

void IParentable::__SortChildren(IParentable* ptr) {
  auto& children = ptr->m_childrenList;
  std::sort(children.begin(), children.end(), [](const ChildInformation* a, const ChildInformation* b) -> bool {
    return a->nLevel < b->nLevel;
  });
}

void IParentable::__ReleaseSelf(IParentable* ptr) {
  bool noop = false;

  for (auto& child : ptr->m_childrenList) {
    ((ComponentBase*)child->pComponent)->MessageEventProcessor(DefaultIDs::DID_ALL, WM_DESTROY, NULL, NULL, noop);
    delete child->pComponent;
  }

  // 这里应该能转， IParentable接口适用于 ComponentBase基类及其衍生类，不可能出现转换导致崩溃的情况。
  // ((ComponentBase*)ptr)->MessageEventProcessor(DefaultIDs::DID_ALL, WM_DESTROY, NULL, NULL, noop);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ITextProtoypes::ITextProtoypes() {
  m_textAlign     = StringAlignmentNear;
  m_textLineAlign = StringAlignmentNear;

  m_font      = new Font(L"Segoe UI", 16.f);
  m_fontColor = Color::White;
}

ITextProtoypes::~ITextProtoypes() {
  if (m_font != nullptr) {
    delete m_font;
    m_font = nullptr;
  }
}

void ITextProtoypes::SetTextAlign(StringAlignment align) { m_textAlign = align; }

StringAlignment ITextProtoypes::GetTextAlign() const { return m_textAlign; }

void ITextProtoypes::SetTextLineAlign(StringAlignment align) { m_textLineAlign = align; }

StringAlignment ITextProtoypes::GetTextLineAlign() const { return m_textLineAlign; }

void ITextProtoypes::SetFont(const Font& font) { m_font = font.Clone(); }

Font* ITextProtoypes::GetFont() { return m_font; }

void ITextProtoypes::SetFontSize(REAL size) {
  FontFamily fontFamily{};
  m_font->GetFamily(&fontFamily);

  delete m_font;
  m_font = new Font(&fontFamily, size);
}

REAL ITextProtoypes::GetFontSize() { return m_font->GetSize(); }

void ITextProtoypes::SetFontColor(const Color color) { m_fontColor = color; }

Color ITextProtoypes::GetFontColor() const { return m_fontColor; }

void ITextProtoypes::DefaultRender(Graphics& graphics) { CommonPrototypes_DefaultRender(graphics); }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ICommonPrototypes::ICommonPrototypes() { m_backgroundColor = Color::Transparent; }

ICommonPrototypes::~ICommonPrototypes() {}

void ICommonPrototypes::SetBackgroundColor(const Color color) { m_backgroundColor = color; }

Color ICommonPrototypes::GetBackgroundColor() const { return m_backgroundColor; }

void ICommonPrototypes::CommonPrototypes_DefaultRender(Graphics& graphics) { graphics.Clear(m_backgroundColor); }
