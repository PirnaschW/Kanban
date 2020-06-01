#include "pch.h"

namespace Kanban {

  Column::Column(std::string title) : title_(title) {}


	Column::Column(CArchive& ar)
	{
		// TODO: add loading code here

	}

	void Column::Serialize(CArchive& ar) const
	{
		// TODO: add storing code here
	}



}