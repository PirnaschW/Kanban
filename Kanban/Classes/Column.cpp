#include "Classes.h"

namespace Kanban {

  Column::Column(std::string title, size_t width) : title_(title), width_(width)
  {
    cards_.emplace_back(std::make_unique<Card>("Card 1"));
    cards_.emplace_back(std::make_unique<Card>("Card 2", 120U));
    cards_.emplace_back(std::make_unique<Card>("Card 3"));
  }


  Column::Column(Archive& ar)
  {
    for (const auto& card : cards_) card->Serialize(ar);
  }

  void Column::Serialize(Archive& ar) const
  {
    for (const auto& card : cards_) card->Serialize(ar);
  }

  void Column::Draw(UIContext UIC) const
  {
    for (const auto& card : cards_)
    {
      card->Draw(UIC, GetWidth());
      UIC.p.y_ += card->GetHeight() + UIDim::verticalspace;
    }

  }

}