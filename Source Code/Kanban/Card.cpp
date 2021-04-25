#include "pch.h"

namespace Kanban {

  Card::Card(std::wstring title) noexcept : ID_(++lastID_), title_(title == L"" ? L"New Card " + std::to_wstring(ID_) : title_) {}
  Card::Card(CArchive* ar)                            // create from file
  {
    *ar >> ID_;
    if (lastID_ < ID_) lastID_ = ID_;

    *ar >> title_ >> text_;
  }
  void Card::Serialize(CArchive* ar) const            // save to File
  {
    *ar << ID_ << title_ << text_;
  }
  Card::~Card(void) noexcept { }

  bool Card::Edit(void)
  {
    DlgCard dlg(this);
    if (dlg.DoModal() == IDOK)
    {
      SetText(L"new shorter text!");
      bValid_ = false;
      return true;
    }
    return false;
  }
  void Card::SetText(std::wstring t) noexcept { text_ = std::move(t); bValid_ = false; }
  void Card::SetWidth(size_t width) noexcept { width_ = width;  bValid_ = false; }
  void Card::SetColor(COLORREF color) noexcept { color_ = color; }
  size_t Card::GetWidth(void) const noexcept { return width_; }
  size_t Card::GetHeight(void) const noexcept { return rect_.Height(); }
  CSize Card::GetSize(void) const noexcept { return { (int) GetWidth(), (int) GetHeight() }; }

  CSize Card::CalcSize(CDC* pDC) const noexcept
  {
    assert(!bValid_);

    // Title:
    CFont* f = pDC->SelectObject(&UI::fontCardTitle_.font_);
    titleRect_.top = 1;              // leave 1 pts space on top
    titleRect_.left = 4;             // leave 4 pts space on left
    titleRect_.right = width_ - 5;   // leave 5 pts space on right
    titleLines_ = Lines::BreakLines(UI::fontCardTitle_, title_, titleRect_.right - titleRect_.left);  
    titleRect_.bottom = titleRect_.top + titleLines_.GetHeight(UI::fontCardTitle_) + 2; // leave 2 pts space at bottom

    // Text:
    pDC->SelectObject(&UI::fontCardText_.font_);
    textRect_.top = titleRect_.bottom + 1;  // leave 1 pts space on top
    textRect_.left = 4;                     // leave 4 pts space on left
    textRect_.right = width_ - 5;           // leave 5 pts space on right
    textLines_ = Lines::BreakLines(UI::fontCardText_, text_, textRect_.right - textRect_.left);
    textRect_ = titleRect_ + CSize(0, titleRect_.bottom - 1);
    textRect_.bottom = textRect_.top + textLines_.GetHeight(UI::fontCardText_) + 2; // leave 2 pt space at bottom
    pDC->SelectObject(f);

    rect_.right = width_;
    rect_.bottom = textRect_.bottom;

    bValid_ = true;

    return GetSize();
  }

  void Card::Draw(CDC* pDC, const CPoint& point, bool saveLoc) const noexcept
  {
    if (!bValid_) CalcSize(pDC);

    // draw the card itself

    // drop shadows - they are going 'outside' of rect
    CBrush* b = pDC->SelectObject(&UI::brushShadow_);
    CPen* p = pDC->SelectObject(&UI::penShadow_);
    CRect shadowB{ rect_.left + point.x + (int) UI::shadowoffset, rect_.bottom + point.y,
                   rect_.right + point.x + (int) UI::shadowoffset,  rect_.bottom + point.y + (int) UI::shadowoffset };
    pDC->Rectangle(shadowB);
    CRect shadowR{ rect_.right + point.x, rect_.top + point.y + (int) UI::shadowoffset,
                   rect_.right + point.x + (int) UI::shadowoffset,  rect_.bottom + point.y + (int) UI::shadowoffset };
    pDC->Rectangle(shadowR);

    // main card
    CBrush b0(color_);
    pDC->SelectObject(&b0);
    pDC->SelectObject(&UI::penCard_);
    pDC->RoundRect(rect_ + point, CPoint(UI::roundcorners, UI::roundcorners));
    pDC->SelectObject(p);
    pDC->SelectObject(b);

    COLORREF bk = pDC->SetBkColor(color_);
    CFont* f = pDC->SelectObject(&UI::fontCardTitle_.font_);
    pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);
    UI::fontCardTitle_.DrawMultiText(pDC, point + CSize((titleRect_.left + titleRect_.right) / 2, titleRect_.top), title_, titleLines_);

    pDC->SelectObject(&UI::fontCardText_.font_);
    pDC->SetTextAlign(TA_TOP | TA_LEFT | TA_NOUPDATECP);
    UI::fontCardText_.DrawMultiText(pDC, point + CSize(textRect_.left, textRect_.top), text_, textLines_);
    pDC->SelectObject(f);
    pDC->SetBkColor(bk);

    if (saveLoc) point_ = point; // buffer Card loction (absolute screen coordinates); this will be used to find the card when clicked
  }

  bool Card::PtInCard(const CPoint& point, CSize& offset) const noexcept
  {
    offset = point - point_;
    return offset.cx >= 0 && offset.cx < (int) width_ && offset.cy >= 0 && offset.cy < rect_.Height();
  }

  bool Card::CardInRect(const CRect& clip) const noexcept
  {
    return
      point_.y + rect_.bottom + (int) UI::shadowoffset >= clip.top && point_.y + rect_.top < clip.bottom &&
      point_.x + rect_.right + (int) UI::shadowoffset >= clip.left && point_.x + rect_.left < clip.right;
  }

}