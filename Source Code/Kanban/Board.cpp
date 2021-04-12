#include "pch.h"

namespace Kanban
{
  Board::Board(size_t n) noexcept : title_(L"Kanban Board " + std::to_wstring(n))
  {
    column_.reserve(UIDim::dummycolumns);
    for (size_t z = 0; z < UIDim::dummycolumns; ++z)
      column_.push_back(new Column(L"", 120U + rand() % 120));
  }
  Board::Board(CArchive* ar)
  {
    size_t z;
    *ar >> title_ >> z;
    column_.reserve(z);

    for (size_t i = 0U; i < z; ++i) column_.push_back(new Column(ar));   // read complete column from archive
  }
  void Board::Serialize(CArchive* ar) const
  {
    *ar << title_ << column_.size();
    for (const auto* c : column_) c->Serialize(ar);
  }
  Board::~Board(void) noexcept
  {
    for (auto& c : column_) delete c;
  }


  Card* Board::GetCard(const CPoint& point, CSize& offset) const
  {
    for (const auto& c : column_)
    {
      if (c->PtInColumn(point))
      {
        return c->GetCard(point,offset);
      }
    }
    return nullptr;
  }

  void Board::Draw(CDC* pDC) const
  {
    CRect clip;
    pDC->GetClipBox(&clip);

    CPoint p{ UIDim::horizontalspace, UIDim::verticalspace};
    for (const auto& c : column_)
    {
      if (p.x + (int) c->GetWidth() > clip.left && p.x < clip.right)   // only redraw the column if it is invalid            
        c->Draw(pDC, p);
      p.x += c->GetWidth() + UIDim::horizontalspace;
    }
  }

}
