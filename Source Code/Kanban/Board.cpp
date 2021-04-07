#include "pch.h"

namespace Kanban
{
  Board::Board(size_t n) noexcept : title_("Kanban Board " + std::to_string(n))
  {
    for (size_t z = 0; z < 100; ++z) AddColumn(new Column("Col " + z, 120U + rand() % 120));
  }
 
  Board::Board(std::string title) noexcept : title_(title) {}
  
  Board::~Board(void) noexcept { for (auto& c : columns_) delete c;  }

  Board::Board(CArchive* ar)
  {
    size_t z;
    *ar >> title_ >> z;
    for (size_t i = 0U; i < z; ++i) AddColumn(new Column(ar));
  }

  void Board::Serialize(CArchive* ar) const
  {
    *ar << title_ << columns_.size();
    for (const auto& column : columns_) column->Serialize(ar);
  }

  void Board::Draw(CDC* pDC) const
  {
    VERIFY(pDC);
    CPoint p{UIDim::horizontalspace,UIDim::verticalspace};
    for (const auto& column : columns_)
    {
      p.y = UIDim::verticalspace; // reset for each column
      VERIFY(column);
      column->Draw(pDC, p);
      p.x += column->GetWidth() + UIDim::horizontalspace;
    }
  }

  bool Board::React(unsigned int event, unsigned int nFlags, const CPoint& p)   // react to mouse events
  { 
    Card* c{ nullptr };
    switch (event)
    {
      case WM_LBUTTONDOWN:
        c = GetCard(p);
        if (!c) return false;
        if (selected_) selected_->Select(false);
        c->Select(true);
        selected_ = c;
        //        if (_mode.IsSet(Mode::Editing)) DragStart(p);
        break;
      case WM_LBUTTONUP:
        //if (_mode.IsSet(Mode::Dragging)) DragEnd(p);
        //else Select(p);
        break;
      case WM_LBUTTONDBLCLK:
        c = GetCard(p);
        if (!c) return false;
        {
          DlgCard dlg(*c);
          dlg.DoModal();
        }
        break;
      case WM_RBUTTONDOWN:   return false;    // that will skip updating all views
      case WM_RBUTTONUP:
        //Unselect();
        break;
      case WM_RBUTTONDBLCLK: return false;    // that will skip updating all views
      case WM_MOVE:
        //if (_mode.IsSet(Mode::Dragging)) DragTo(p);
        //else return false;                    // that will skip updating all views
        break;
      default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }


  Card* Board::GetCard(const CPoint& p) const
  {
    const Column* col = GetColumn(p);
    return col ? col->GetCard(p) : nullptr;
  }

  Column* Board::GetColumn(const CPoint& p) const
  {
    for (auto& c : columns_) if (c->PtInColumn(p)) return c;
    return nullptr;
  }


  void Board::AddColumn(Column* c)
  {
    columns_.push_back(c);
  }

  void Board::RemoveColumn(Column* c)
  {
    columns_.remove(c);
  }

}


//namespace MFC
//{
//  using namespace Kanban;
//
//  Board* B(void* p) { return reinterpret_cast<Board*>(p); }
//
//  Doc::Doc(size_t n) { pImpl_ = new Board(n); }
//  Doc::Doc(Archive& ar) { pImpl_ = new Board(ar); }
//  Doc::~Doc(void) { delete B(pImpl_); }
//  void Doc::Serialize(Archive& ar) { B(pImpl_)->Serialize(ar); }
//  void Doc::Draw(DC& dc) { B(pImpl_)->Draw(dc); }
//  bool Doc::React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) { return true; }      // react to keyboard input (not menu shortcuts, but typing)
//  bool Doc::React(unsigned int command) { return true; }                                               // react to button/menu command
//  bool Doc::React(unsigned int event, unsigned int nFlags, const Point& p)                             // react to mouse events
//  { 
//    switch (event)
//    {
//      case Mouse_LButton_Down:
////        if (_mode.IsSet(Mode::Editing)) DragStart(p);
//        break;
//      case Mouse_LButton_Up:
//        //if (_mode.IsSet(Mode::Dragging)) DragEnd(p);
//        //else Select(p);
//        break;
//      case Mouse_LButton_DblClk:
//        {
//          //Card* c = B(pImpl_)->GetCard();
//          //DlgCard dlg;
//          //dlg.DoModal();
//        }
//        break;
//      case Mouse_RButton_Down:   return false;    // that will skip updating all views
//      case Mouse_RButton_Up:
//        //Unselect();
//        break;
//      case Mouse_RButton_DblClk: return false;    // that will skip updating all views
//      case Mouse_Move:
//        //if (_mode.IsSet(Mode::Dragging)) DragTo(p);
//        //else return false;                    // that will skip updating all views
//        break;
//      default: return false;                  // that will skip updating all views
//    }
//    return true;                              // update all views
//  }
//  void Doc::React(MFC::CmdUI* pUI) { }                                                                 // react to UI events (allows to set buttons greyed, etc.)
//  void Doc::DragStart(const MFC::Point&) {}
//  void Doc::DragEnd(const MFC::Point&) {}
//
//
//
//}