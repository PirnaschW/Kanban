#include "MFC-Win.h"

namespace MFC
{

  //Archive& Archive::operator << (const char c) { *ar_ << c; return *this; }
  //Archive& Archive::operator >> (char& c)      { *ar_ >> c; return *this; }
  template <typename T> Archive& Archive::operator << (T& t) { *ar_ << t; return *this; }
  template <typename T> Archive& Archive::operator >> (T& t) { *ar_ >> t; return *this; }

}
