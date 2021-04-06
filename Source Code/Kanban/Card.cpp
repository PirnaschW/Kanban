#include "pch.h"

namespace MFC
{

}

namespace Kanban {

  Card::Card(std::string title, size_t height) : title_(title), height_(height) {}

  size_t Card::GetHeight(void) const noexcept { return height_; }

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
    pDC->Rectangle(p.x, p.y, p.x + width, p.y + GetHeight());
  }

}