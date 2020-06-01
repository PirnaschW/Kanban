#include "pch.h"

namespace Kanban
{
	Board::Board(size_t n) : title_("Kanban Board " + std::to_string(n)) {}
	Board::Board(std::string title) : title_(title) {}
	Board::~Board(void)
	{

	}

	Board::Board(CArchive& ar)
	{
		// TODO: add loading code here

	}

	void Board::Serialize(CArchive& ar) const
	{
		// TODO: add storing code here
	}


}