#include "Classes.h"

namespace Kanban
{
  Board::Board(size_t n) : title_("Kanban Board " + std::to_string(n))
  {
    columns_.emplace_back(std::make_unique<Column>("Col 1"));
    columns_.emplace_back(std::make_unique<Column>("Col 2", 300U));
    columns_.emplace_back(std::make_unique<Column>("Col 3"));
  }
	
	Board::Board(std::string title) : title_(title) {}
	
	Board::~Board(void)
	{

	}

	Board::Board(Archive& ar)
	{
    size_t z;
    ar >> title_ >> z;
    for (size_t i = 0U; i < z; ++i) columns_.emplace_back(std::make_unique<Column>(ar));
	}

	void Board::Serialize(Archive& ar) const
	{
    ar << title_ << columns_.size();
    for (const auto& column : columns_) column->Serialize(ar);
  }

	void Board::Draw(DC& dc) const
	{
    UIContext UIC{ dc,{UIDim::horizontalspace,UIDim::verticalspace} };
		for (const auto& column : columns_)
		{
			column->Draw(UIC);
			UIC.p.x_ += column->GetWidth() + UIDim::horizontalspace;
		}
	}

}


namespace MFC
{
  using namespace Kanban;

  Board* B(void* p) { return reinterpret_cast<Board*>(p); }

  Doc::Doc(size_t n) { pImpl_ = new Board(n); }
  Doc::Doc(Archive& ar) { pImpl_ = new Board(ar); }
  Doc::~Doc(void) { delete B(pImpl_); }
  void Doc::Serialize(Archive& ar) { B(pImpl_)->Serialize(ar); }
  void Doc::Draw(DC& dc) { B(pImpl_)->Draw(dc); }
  bool Doc::React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) { return true; }      // react to keyboard input (not menu shortcuts, but typing)
  bool Doc::React(unsigned int command) { return true; }                                               // react to button/menu command
  bool Doc::React(unsigned int event, unsigned int nFlags, const Point& p)                             // react to mouse events
  { 
    switch (event)
    {
      case Mouse_LButton_Down:
//        if (_mode.IsSet(Mode::Editing)) DragStart(p);
        break;
      case Mouse_LButton_Up:
        //if (_mode.IsSet(Mode::Dragging)) DragEnd(p);
        //else Select(p);
        break;
      case Mouse_LButton_DblClk:
        {
          //Card* c = B(pImpl_)->GetCard();
          //DlgCard dlg;
          //dlg.DoModal();
        }
        break;
      case Mouse_RButton_Down:   return false;    // that will skip updating all views
      case Mouse_RButton_Up:
        //Unselect();
        break;
      case Mouse_RButton_DblClk: return false;    // that will skip updating all views
      case Mouse_Move:
        //if (_mode.IsSet(Mode::Dragging)) DragTo(p);
        //else return false;                    // that will skip updating all views
        break;
      default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }
  void Doc::React(MFC::CmdUI* pUI) { }                                                                 // react to UI events (allows to set buttons greyed, etc.)
  void Doc::DragStart(const MFC::Point&) {}
  void Doc::DragEnd(const MFC::Point&) {}



}