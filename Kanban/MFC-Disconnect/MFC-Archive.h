
class CArchive;

namespace MFC
{

  class Archive
  {
  public:
    constexpr Archive(CArchive* ar) noexcept : ar_(ar) {}

    //Archive& operator << (const char c);
    //Archive& operator >> (char& c);
    template <typename T> Archive& operator << (const T& t);
    template <typename T> Archive& operator >> (T& t);
    Archive& operator << (const std::string& t);
    Archive& operator >> (std::string& t);


    void dummy();
  private:
    CArchive* ar_;  // just store it so we can use it, we do NOT own it
  };

}
