#include "Classes.h"

namespace MFC
{

  //Archive& Archive::operator << (const char c) { *ar_ << c; return *this; }
  //Archive& Archive::operator >> (char& c)      { *ar_ >> c; return *this; }
  template <typename T> Archive& Archive::operator << (T& t) { *ar_ << t; return *this; }
  template <typename T> Archive& Archive::operator >> (T& t) { *ar_ >> t; return *this; }

}

namespace Kanban {

  Card::Card(std::string title, size_t height) : title_(title), height_(height) {}

  size_t Card::GetHeight(void) const noexcept { return height_; }

  Card::Card(Archive& ar)
  {
//    ar >> title_ >> text_ >> height_;
    ar >> height_;
  }

  void Card::Serialize(Archive& ar) const
  {
//    ar << title_ << text_ << height_;
    ar << height_;
  }

  void Card::Draw(UIContext UIC, size_t width) const
  {
    UIC.dc.Rectangle(UIC.p.x_, UIC.p.y_, UIC.p.x_ + width, UIC.p.y_ + GetHeight());
  }

}