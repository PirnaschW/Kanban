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


  Card* Board::GetCard(const CPoint& p) const
  {
    for (const auto& c : column_)
    {
      if (c->PtInColumn(p))
      {
        return c->GetCard(p);
      }
    }
    return nullptr;
  }

  void Board::SetHWND(HWND hView) const noexcept { hView_ = hView; }
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
    if (dragging_)
    {
      //if (!rect_.PtInRect(dragPoint_))
      //  dragging_ = false;
      //else
        selected_->Draw(pDC, dragPoint_, UIStatus::Dragging);
    }
  }

  bool Board::React(unsigned int event, unsigned int nFlags, const CPoint& p)   // react to mouse events
  {
    switch (event)
    {
      case WM_LBUTTONDOWN:
        selected_ = GetCard(p);                       // find clicked card - could be nullptr
        if (selected_)
        {
          dragging_ = true;                           // any mouse move will now drag this card
          dragPoint_ = p;
          CRect r{ dragPoint_.x, dragPoint_.y, dragPoint_.x + (int) selected_->GetWidth(), dragPoint_.y + (int) selected_->GetHeight() };
          r.InflateRect(3, 3);
          ::InvalidateRect(hView_, &r, true);
        }
        return false;                                  // redraw views
      case WM_LBUTTONUP:
        dragging_ = false;
        return true;                                 // redraw views
      case WM_LBUTTONDBLCLK:
        if (!selected_) return false;                 // no redraw needed
        else
        {
          DlgCard dlg(selected_);
          dlg.DoModal();
        }
        return true;                                  // redraw views
      case WM_RBUTTONDOWN:
        return false;                                 // no redraw needed
      case WM_RBUTTONUP:
        assert(false);                                // this is an error - context menu should have been called
        return false;                                 // no redraw needed
      case WM_RBUTTONDBLCLK:
        return false;                                 // no redraw needed
      case WM_MOUSEMOVE:
        if (!dragging_) return false;                 // no redraw needed
        {
          CRect r{ dragPoint_.x, dragPoint_.y, dragPoint_.x + (int) selected_->GetWidth(), dragPoint_.y + (int) selected_->GetHeight() };
          r.InflateRect(3, 3);
          ::InvalidateRect(hView_, &r, true);
        }
        dragPoint_ = p;                               // safe current point
        {
          CRect r{ dragPoint_.x, dragPoint_.y, dragPoint_.x + (int) selected_->GetWidth(), dragPoint_.y + (int) selected_->GetHeight() };
          r.InflateRect(3, 3);
          ::InvalidateRect(hView_, &r, true);
        }
        return false;                                 // do NOT redraw all views every time
      default:
        assert(false);                                // this is an error - unhandled mouse event
        return false;                                 // no redraw needed
    }
    //return true;                              // update all views
  }

}
