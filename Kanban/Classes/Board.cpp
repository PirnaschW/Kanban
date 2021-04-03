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

  Doc::Doc(size_t n) { pImpl_ = new Board(n); }
  Doc::Doc(Archive& ar) { pImpl_ = new Board(ar); }
  Doc::~Doc(void) { delete reinterpret_cast<Board*>(pImpl_); }
  void Doc::Serialize(Archive& ar) { reinterpret_cast<Board*>(pImpl_)->Serialize(ar); }
  void Doc::Draw(DC& dc) { reinterpret_cast<Board*>(pImpl_)->Draw(dc); }

}