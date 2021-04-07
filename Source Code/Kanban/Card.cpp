#include "pch.h"

namespace Kanban {

  Card::Card(Column* col, std::string title, size_t height) noexcept : col_(col), title_(title), height_(height) {}
  Card::~Card(void) noexcept { }

  // serialization
  Card::Card(CArchive* ar)
  {
    *ar >> title_ >> text_ >> height_;
  }

  void Card::Serialize(CArchive* ar) const
  {
    *ar << title_ << text_ << height_;
  }

  void Card::Draw(CDC* pDC, CPoint& p, size_t width) const
  {
    VERIFY(pDC);
    rect_ = CRect{ p.x, p.y, p.x + (int) width, p.y + (int) GetHeight() };
    if (selected_)
    {
      CRect r{ rect_ };
      r.InflateRect(2, 2);
      pDC->Rectangle(r);
    }
    pDC->Rectangle(rect_);
  }

  size_t Card::GetHeight(void) const noexcept { return height_; }

  Column* Card::SetColumn(Column* col) noexcept
  {
    // remove from previous column
    col_->RemoveCard(this);
    col_ = col;
    col_->AddCard(this);
    return col_;
  }

  bool Card::PtInCard(const CPoint& p) const noexcept
  {
    return (rect_.PtInRect(p));
  }

  void Card::Select(bool s) const noexcept
  {
    selected_ = s;
  }

}