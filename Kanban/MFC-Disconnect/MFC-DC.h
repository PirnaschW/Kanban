
class CDC;
class CGdiObject;

namespace MFC
{
  class GdiObjectRAII
  {
  public:
    GdiObjectRAII(CDC* cdc, CGdiObject* gdi);
    ~GdiObjectRAII(void);

  private:
    CDC* cdc_;
    CGdiObject* gdi_; // const, but the SelectObject interface doesn't support this
  };

  class DC
  {
  public:
    constexpr DC(CDC* cdc) noexcept : cdc_{ cdc } {};
    constexpr operator CDC* (void) const noexcept { return cdc_; }
    [[nodiscard]] const GdiObjectRAII SelectStockObject(int nIndex);
    [[nodiscard]] const GdiObjectRAII SelectPen(int nPenStyle, int nWidth, unsigned long crColor);
    [[nodiscard]] const GdiObjectRAII SelectPen(int nPenStyle, int nWidth, unsigned char r, unsigned char g, unsigned char b);
    void Rectangle(const Rect& r) const;
    void Rectangle(int l, int t, int r, int b) const;
    bool Text(int x, int y, std::string s) const;

  private:
    CDC* const cdc_{ nullptr };
  };

}
