#include "pch.h"

namespace Kanban
{

  Board::Board(size_t n) noexcept : title_(L"Kanban Board " + std::to_wstring(n))
  {
    //column_.reserve(UI::dummycolumns);
    for (size_t z = 0; z < UI::dummycolumns; ++z)
      column_.push_back(new Column(L"", 120U + rand() % 120));
  }
  Board::Board(CArchive* ar)
  {
    size_t z;
    *ar >> title_ >> z;
    //column_.reserve(z);

    for (size_t i = 0U; i < z; ++i) column_.emplace_back(new Column(ar));   // read complete column from archive
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


  CSize Board::CalcSize(CDC* pDC) const noexcept
  {
    if (UI::fontCardTitle_.cy_ == 0U)
    {
      UI::fontColumnTitle_.font_.CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontColumnTitle_.GetCharWidths(pDC);
      UI::fontCardTitle_.font_.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontCardTitle_.GetCharWidths(pDC);
      UI::fontCardText_.font_.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontCardText_.GetCharWidths(pDC);
    }

    CSize size{ UI::horizontalspace,0 };
    for (const auto& c : column_)
    {
      CSize cSize = c->CalcSize(pDC);
      size.cx += cSize.cx + UI::horizontalspace;
      size.cy = (std::max)(size.cy,cSize.cy);
    }
    size.cy += UI::verticalspace;
    return size;
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

  void Board::Draw(CDC* pDC, const CRect& clip) const
  {
    CPoint p{ UI::horizontalspace, UI::verticalspace};
    for (const auto& c : column_)
    {
      c->Draw(pDC, clip, p);
      p.x += c->GetWidth() + UI::horizontalspace;
    }
  }

}
