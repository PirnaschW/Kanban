#include "pch.h"

namespace Kanban {

  Column::Column(std::wstring title, size_t width) noexcept : ID_(++lastID_), title_(title == L"" ? L"new Column " + std::to_wstring(ID_) : title_), width_(width)
  {
    card_.reserve(UI::dummycards);
    for (size_t z = 0; z < UI::dummycards; ++z)
    {
      //Card* c = new Card();
      //card_.push_back(c);
      Card* c = card_.emplace_back(new Card());
      std::wstring text = L"Text " + std::to_wstring((ID) *c) + L" ";
      for (size_t i = 0U; i < 4U + rand() % 10U; ++i) text += L"Lorem a b c Ipsum d e f ";
      c->SetText(text + L"Lorem Ipsum");
      c->SetWidth(width_);
    }
  }
  Column::Column(CArchive* ar)
  {
    *ar >> ID_;
    if (lastID_ < ID_) lastID_ = ID_;

    size_t z;
    *ar >> title_ >> width_ >> z;
    card_.reserve(z);
    for (size_t i = 0U; i < z; ++i)
      card_.push_back(new Card(ar));
  }
  void Column::Serialize(CArchive* ar) const
  {
    *ar << ID_;

    *ar << title_ << width_ << card_.size();
    for (const auto& c : card_) c->Serialize(ar);
  }
  Column::~Column(void) noexcept
  {
    for (auto& c : card_) delete c;
  }



  CSize Column::CalcSize(CDC* pDC) const noexcept
  {
    assert(!bValid_);

    CFont* f = pDC->SelectObject(&UI::fontColumnTitle_.font_);
    titleLines_ = Lines::BreakLines(UI::fontColumnTitle_, title_, width_ - 5);  // leave 2 pt space on left, and 3 pts on right
    titleRect_.right = width_;
    titleRect_.bottom = titleLines_.GetHeight(UI::fontColumnTitle_) + 1; // leave 1 pt space at bottom
    bValid_ = true;

    CSize size{ (int) width_, (int) UI::verticalspace + titleRect_.bottom };
    for (const auto& c : card_)
    {
      CSize cSize = c->CalcSize(pDC);
      size.cx = (std::max) (size.cx, cSize.cx);
      size.cy += cSize.cy + UI::verticalspace;
    }
    return size;
  }
  void Column::SetWidth(size_t width) noexcept { width_ = width; }
  size_t Column::GetWidth(void) const noexcept { return width_; }
  size_t Column::GetHeight(void) const noexcept { return titleRect_.Height(); }

  void Column::Draw(CDC* pDC, const CPoint& point) const
  {
    // calculate target rect
//    rCard_ = { p.x, p.y, p.x + (int) GetWidth(), p.y };

// draw column header
    CFont* f = pDC->SelectObject(&UI::fontColumnTitle_.font_);
    pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);
    UI::fontColumnTitle_.DrawMultiText(pDC, point + CSize(titleRect_.Width() / 2, 1), title_, titleLines_);
    pDC->SelectObject(f);

    // draw all cards
    CRect clip;
    pDC->GetClipBox(&clip);
    CPoint p{ point.x,point.y + (int) titleRect_.Height() + (int) UI::verticalspace };
    for (const auto& c : card_)
    {
      if (p.y + (int) c->GetHeight() > clip.top && p.y < clip.bottom)   // only redraw the card if it is invalid            
      {
        c->Draw(pDC, { p.x, p.y + (int) UI::verticalspace }, UIStatus::Normal);
      }
      p.y += c->GetHeight() + UI::verticalspace;
    }
  }


  Card* Column::GetCard(const CPoint& point, CSize& offset) const noexcept
  {
    for (const auto& c : card_) if (c->PtInCard(point,offset)) return c;
    return nullptr;
  }

  bool Column::PtInColumn(const CPoint& p) const noexcept
  {
    return false; // (rCard_.PtInRect(p));
  }

  bool Column::RectInColumn(const CRect& r) const noexcept
  {
    CRect i;
 //   i.IntersectRect(r, rCard_);
    return !i.IsRectEmpty();
  }

}
