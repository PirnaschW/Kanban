#include "pch.h"

namespace Kanban {

  Column::Column(std::wstring title, size_t width) noexcept : ID_(++lastID_), title_(title == L"" ? L"New Column " + std::to_wstring(ID_) : title_), width_(width)
  {
  }
  Column::Column(CArchive* ar)
  {
    *ar >> ID_;
    if (lastID_ < ID_) lastID_ = ID_;

    *ar >> title_ >> width_;
  }
  void Column::Serialize(CArchive* ar) const
  {
    *ar << ID_;
    *ar << title_ << width_;
  }
  Column::~Column(void) noexcept {}


  CSize Column::CalcSize(CDC* pDC) const noexcept
  {
    assert(!bValid_);

    CFont* f = pDC->SelectObject(&UI::fontColumnTitle_.font_);
    titleLines_ = Lines::BreakLines(UI::fontColumnTitle_, title_, width_ - 5);  // leave 2 pt space on left, and 3 pts on right
    titleRect_.right = width_;
    titleRect_.bottom = titleLines_.GetHeight(UI::fontColumnTitle_) + 1; // leave 1 pt space at bottom
    bValid_ = true;

    size_ = { (int) width_, (int) UI::verticalspace + titleRect_.bottom + (int) UI::shadowoffset };
    //for (const auto& c : card_)
    //{
    //  CSize cSize = c->CalcSize(pDC);
    //  size_.cx = (std::max) (size_.cx, cSize.cx);
    //  size_.cy += cSize.cy + UI::verticalspace;
    //}
    return size_;
  }
  void Column::SetWidth(size_t width) noexcept { width_ = width; bValid_ = false; }
  size_t Column::GetWidth(void) const noexcept { return width_; }
  size_t Column::GetHeight(void) const noexcept { return titleRect_.Height(); }
  CSize Column::GetSize(void) const noexcept { return size_; }

  void Column::Draw(CDC* pDC, const CRect& clip, const CPoint& point, bool saveLoc) const
  {
    assert(bValid_);  // must have been be calculated before, otherwise programming error

    CPoint p{ point };
    if (p.y + (int) GetHeight() > clip.top && p.y < clip.bottom)   // only draw the column header if it is inside clip rectangle
    {
      pDC->SetBkColor(UI::columnColor_);
      CFont* f = pDC->SelectObject(&UI::fontColumnTitle_.font_);
      pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);
      UI::fontColumnTitle_.DrawMultiText(pDC, point + CSize(titleRect_.Width() / 2, 1), title_, titleLines_);
      pDC->SelectObject(f);
    }
    if (saveLoc) point_ = point; // buffer Column loction (absolute screen coordinates); this will be used to find the column when clicked
  }

  bool Column::PtInColumn(const CPoint& point) const noexcept
  {
    CSize offset = point - point_;
    return offset.cx >= 0 && offset.cx < GetSize().cx && offset.cy >= 0 && offset.cy < GetSize().cy;
  }

  bool Column::RectInColumn(const CRect& r) const noexcept
  {
    CRect i{ point_, point_ + size_ };
    i &= r;
    return !i.IsRectEmpty();
  }

}
