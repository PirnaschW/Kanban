#include "pch.h"

namespace Kanban {

  Column::Column(std::string title, size_t width) : title_(title), width_(width)
  {
    cards_.emplace_back(std::make_unique<Card>("Card 1"));
    cards_.emplace_back(std::make_unique<Card>("Card 2", 120U));
    cards_.emplace_back(std::make_unique<Card>("Card 3"));
  }


  Column::Column(CArchive* ar)
  {
    size_t z;
    *ar >> title_ >> width_ >> z;
    for (size_t i = 0U; i < z; ++i) cards_.emplace_back(std::make_unique<Card>(ar));
  }

  void Column::Serialize(CArchive* ar) const
  {
    *ar << title_ << width_ << cards_.size();
    for (const auto& card : cards_) card->Serialize(ar);
  }

  void Column::Draw(CDC* pDC, CPoint& p) const
  {
    for (const auto& card : cards_)
    {
      card->Draw(pDC, p, GetWidth());
      p.y += card->GetHeight() + UIDim::verticalspace;
    }

  }

}