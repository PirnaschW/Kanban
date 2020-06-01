#include "pch.h"

namespace Kanban {

	Card::Card(std::string title) : title_(title) {}

	Card::Card(CArchive& ar)
	{		
		// TODO: add loading code here

	}

	void Card::Serialize(CArchive& ar) const
	{
			// TODO: add storing code here
	}

}