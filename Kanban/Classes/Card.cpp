#include "Classes.h"

namespace MFC
{

}

namespace Kanban {

  Card::Card(std::string title, size_t height) : title_(title), height_(height) {}

  size_t Card::GetHeight(void) const noexcept { return height_; }

  Card::Card(Archive& ar)
  {
    ar >> title_ >> text_ >> height_;
//    ar >> height_;
  }

  void Card::Serialize(Archive& ar) const
  {
    ar << title_ << text_ << height_;
//    ar << height_;
  }

  void Card::Draw(UIContext UIC, size_t width) const
  {
    UIC.dc.Rectangle(UIC.p.x_, UIC.p.y_, UIC.p.x_ + width, UIC.p.y_ + GetHeight());
  }

}