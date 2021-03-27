
class CBitmap;

namespace MFC
{

  class Bitmap
  {
  public:
    Bitmap(void) = delete;
    Bitmap(unsigned int ID) noexcept;
    ~Bitmap() noexcept;
    void Draw(DC* pDC, const Rect& r) const noexcept;

  private:
    void Load(DC* pDC, const Rect& r) const noexcept;

    const unsigned int ID_{};
    mutable CBitmap* bmP_{nullptr};  // Piece bitmap - mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()
    mutable CBitmap* bmM_{nullptr};  // Mask  bitmap 
  };

}
