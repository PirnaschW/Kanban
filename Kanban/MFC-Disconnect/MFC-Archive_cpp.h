
namespace MFC
{
    template <typename T> Archive& operator << (const T& t) { *ar_ << t; return *this; }
    template <typename T> Archive& operator >> (T& t) { *ar_ >> t; return *this; }
  template <typename T> Archive& Archive::operator << (T& t) { *ar_ << t; return *this; }
  template <typename T> Archive& Archive::operator >> (T& t) { *ar_ >> t; return *this; }
}
