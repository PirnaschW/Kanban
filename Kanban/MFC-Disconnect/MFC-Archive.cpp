#include "MFC-Win.h"

namespace MFC
{
  //template <> Archive& Archive::operator << (const int& t) { *ar_ << t; return *this; }
  //template <> Archive& Archive::operator >> (int& t) { *ar_ >> t; return *this; }

  //Archive& Archive::operator << (const char c) { *ar_ << c; return *this; }
  //Archive& Archive::operator >> (char& c)      { *ar_ >> c; return *this; }
  template <typename T> Archive& Archive::operator << (const T& t) { *ar_ << t; return *this; }
  template <typename T> Archive& Archive::operator >> (T& t) { *ar_ >> t; return *this; }

  // generate needed instances explicitly (avoids exporting the template to the world)
  template Archive& Archive::operator << <int> (const int& t);
  template Archive& Archive::operator >> <int> (int& t);
  template Archive& Archive::operator << <size_t> (const size_t& t);
  template Archive& Archive::operator >> <size_t> (size_t& t);
  // overwrite instancs for some types with specializations
  Archive& Archive::operator << (const std::string& t) { *ar_ << t.size(); for (const auto& s : t) *ar_ << s;; return *this; }
  Archive& Archive::operator >> (std::string& t) { size_t l; *ar_ >> l; t.resize(l); for (auto &s : t) *ar_ >> s; return *this; }

  void Archive::dummy() { int i = 0; *ar_ << i; }

}
