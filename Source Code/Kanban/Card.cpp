#include "pch.h"

namespace Kanban {

  Card::Card(std::wstring title) noexcept : ID_(++lastID_), title_(title == L"" ? L"new Card " + std::to_wstring(ID_) : title_) {}
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

  void Card::SetText(std::wstring t) noexcept { text_ = std::move(t); bValid_ = false; }
  void Card::SetWidth(size_t width) noexcept { width_ = width;  bValid_ = false; }
  size_t Card::GetWidth(void) const noexcept { return titleRect_.Width(); }
  size_t Card::GetHeight(void) const noexcept { return titleRect_.Height() + textRect_.Height(); }


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

    return { (int) width_,textRect_.bottom };
  }

  /*void Card::DrawMultiText(CDC* pDC, const CPoint& point, const std::wstring& text, const Lines& l) const
  {
    size_t toff = 0U;
    for (const auto& t : l.l_)
    {
      pDC->ExtTextOut(point.x + t.offset.cx, point.y + t.offset.cy, 0U, nullptr, text.c_str() + toff, t.nChars, nullptr);
      toff += t.nChars;
    }
  }*/

  void Card::Draw(CDC* pDC, const CPoint& point, UIStatus s) const
  {

    assert(bValid_);

    if (s == UIStatus::Selected)  // if selected, double outline
    {
      CRect r{ 0,0, titleRect_.Width(), titleRect_.Height() + textRect_.Height() };
      r.OffsetRect(point);
      r.InflateRect(2, 2);
      pDC->Rectangle(r);
    }

    // draw the card itself
    CFont* f = pDC->SelectObject(&UI::fontCardTitle_.font_);
    pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);
    UI::fontCardTitle_.DrawMultiText(pDC, point + CSize(titleRect_.Width() / 2, 1), title_, titleLines_);

    pDC->SelectObject(&UI::fontCardText_.font_);
    pDC->SetTextAlign(TA_TOP | TA_LEFT | TA_NOUPDATECP);
    UI::fontCardText_.DrawMultiText(pDC, point + CSize(1, titleRect_.Height() + 1), text_, textLines_);
    pDC->SelectObject(f);

    pDC->FrameRect(titleRect_ + point, &UI::brushFrame_);
    pDC->FrameRect(textRect_ + point, &UI::brushFrame_);
  }

  void Card::DrawDragged(CDC* pDC, const CPoint& point) const
  {
    HGDIOBJ hPenOriginal = pDC->SelectObject(UI::penDragging_);  // change Pen

// double outline and overwrite background
    CRect r{ 0,0, titleRect_.Width(), titleRect_.Height() + textRect_.Height() };
    r.OffsetRect(point);
    r.InflateRect(2, 2);
    pDC->Rectangle(r);

    pDC->SelectObject(hPenOriginal);                   // change Pen back

    // draw the card itself
    Draw(pDC, point, UIStatus::Normal);

  }

  bool Card::PtInCard(const CPoint& point, CSize& offset) const noexcept
  {
    //if (rect_.PtInRect(point))
    //{
    //  offset = CSize{point.x - rect_.left, point.y - rect_.top };
    //  return true;
    //}
    return false;
  }

}