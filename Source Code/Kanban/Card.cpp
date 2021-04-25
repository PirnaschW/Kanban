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
  size_t Card::GetWidth(void) const noexcept { return GetSize().cx; }
  size_t Card::GetHeight(void) const noexcept { return GetSize().cy; }
  CSize Card::GetSize(void) const noexcept { return { titleRect_.Width(), titleRect_.Height() + textRect_.Height()}; }

  CSize Card::CalcSize(CDC* pDC) const noexcept
  {
    assert(!bValid_);

    CFont* f = pDC->SelectObject(&UI::fontCardTitle_.font_);
    titleLines_ = Lines::BreakLines(UI::fontCardTitle_, title_, width_ - 5);  // leave 2 pt space on left, and 3 pts on right
    titleRect_.right = width_;
    titleRect_.bottom = titleLines_.GetHeight(UI::fontCardTitle_) + 1; // leave 1 pt space at bottom

    pDC->SelectObject(&UI::fontCardText_.font_);
    textLines_ = Lines::BreakLines(UI::fontCardText_, text_, width_ - 5);    // leave 2 pt space on left, and 3 pts on right
    textRect_ = titleRect_ + CSize(0, titleRect_.bottom - 1);
    textRect_.bottom = textRect_.top + textLines_.GetHeight(UI::fontCardText_) + 1; // leave 1 pt space at bottom
    pDC->SelectObject(f);

    bValid_ = true;

    return GetSize();
  }

  void Card::Draw(CDC* pDC, const CPoint& point, bool saveLoc) const noexcept
  {
    if (!bValid_) CalcSize(pDC);

    // draw the card itself

    // drop shadows
    CBrush* b = pDC->SelectObject(&UI::brushShadow_);
    CPen* p = pDC->SelectObject(&UI::penShadow_);
    CRect shadowB{ textRect_.left + point.x + (int) UI::shadowoffset, textRect_.bottom + point.y,
                  textRect_.right + point.x + (int) UI::shadowoffset,  textRect_.bottom + point.y + (int) UI::shadowoffset };
    pDC->Rectangle(shadowB);
    CRect shadowR{ titleRect_.right + point.x, titleRect_.top + point.y + (int) UI::shadowoffset,
                   titleRect_.right + point.x + (int) UI::shadowoffset,  textRect_.bottom + point.y + (int) UI::shadowoffset };
    pDC->Rectangle(shadowR);

    // main card
    pDC->SelectObject(&UI::brushCard_);
    pDC->SelectObject(&UI::penCard_);
    pDC->RoundRect(titleRect_ + point, CPoint(UI::roundcorners, UI::roundcorners));
    pDC->RoundRect(textRect_ + point, CPoint(UI::roundcorners, UI::roundcorners));
    pDC->SelectObject(p);
    pDC->SelectObject(b);

    pDC->SetBkColor(UI::cardColor_);
    CFont* f = pDC->SelectObject(&UI::fontCardTitle_.font_);
    pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);
    UI::fontCardTitle_.DrawMultiText(pDC, point + CSize(titleRect_.Width() / 2, 1), title_, titleLines_);

    pDC->SelectObject(&UI::fontCardText_.font_);
    pDC->SetTextAlign(TA_TOP | TA_LEFT | TA_NOUPDATECP);
    UI::fontCardText_.DrawMultiText(pDC, point + CSize(1, titleRect_.Height() + 1), text_, textLines_);
    pDC->SelectObject(f);

    // frame afterwards only; otherwise hanging blanks would damage the frame
    //pDC->FrameRect(titleRect_ + point, &UI::brushFrame_);
    //pDC->FrameRect(textRect_ + point, &UI::brushFrame_);

    if (saveLoc) point_ = point; // buffer Card loction (absolute screen coordinates); this will be used to find the card when clicked
  }

  bool Card::PtInCard(const CPoint& point, CSize& offset) const noexcept
  {
    offset = point - point_;
    return offset.cx >= 0 && offset.cx < GetSize().cx && offset.cy >= 0 && offset.cy < GetSize().cy;
  }

}