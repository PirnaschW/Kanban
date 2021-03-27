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
    for (const auto& column : columns_) column->Serialize(ar);
	}

	void Board::Serialize(Archive& ar) const
	{
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