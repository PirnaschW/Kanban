
namespace Kanban
{

  enum UIStatus  // how to draw an object
  {
    Normal = 0x00,
    Selected = 0x01,
  };

  using ID = size_t;   // object IDs, to be able to map and serialize relationships


  // multiline text breakup helpers
  struct Font;
  class Line
  {
  public:
    CSize offset{ 0,0 };    // start the line at this offset
    size_t nChars{ 0U };    // number of chars to draw into this line
  };

  class Lines
  {
  public:
    size_t GetHeight(const Font& f) const noexcept;
    static Lines BreakLines(const Font& f, const std::wstring& s, size_t width);

  public:
    std::vector<Line> l_{};
  };

  struct Font
  {
  public:
    void GetCharWidths(CDC* pDC);
    void DrawMultiText(CDC* pDC, const CPoint& point, const std::wstring& s, const Lines& l) const;
  public:
    CFont font_;
    std::array<size_t, 128> cx_;
    size_t cy_;
  };

  struct UI
  {
  public:
    constexpr static size_t dummycolumns{ 10U };                                         // # of columns to create while testing
    constexpr static size_t dummycards{ 8U };                                            // # of cards per column to create while testing
  public:
    constexpr static size_t horizontalspace{ 20U };                                      // horizontal space between columns
    constexpr static size_t verticalspace{ 12U };                                        // vertical space between cards
    constexpr static size_t roundcorners{ 4U };                                          // corner roudning radius
    constexpr static size_t shadowoffset{ 3U };                                          // size of drop shadow
    constexpr static size_t placeholderWidth{ 4U };                                      // width of the placeholder frame
    constexpr static size_t default_columnwidth{ 200U };                                 // default width of new columns
    constexpr static size_t default_columnheight{ 20U };                                 // height of column header
    constexpr static size_t default_cardwidth{ default_columnwidth - horizontalspace };  // default width of new cards
    constexpr static size_t default_cardheight{ 80U };                                   // default height of new cards

  public:  // GDI related helper functions
    static COLORREF Darken(COLORREF c, int amount) noexcept;    // calculate a color that is 'amount' darker than the given one
    static COLORREF Lighten(COLORREF c, int amount) noexcept;   // calculate a color that is 'amount' lighter than the given one

  public:  // reusable GDI objects
    static const unsigned char backgroundGray_;   // general window background
    static const unsigned char shadowGray_;       // drop shadows
    static const unsigned char cardGray_;         // card surface
    static const unsigned char columnGray_;       // column header
    static const COLORREF backgroundColor_;
    static const COLORREF shadowColor_;
    static const COLORREF cardColor_;
    static const COLORREF columnColor_;
    static CPen penBackground_;
    static CPen penShadow_;
    static CPen penCard_;
    static const CPen penDragging_;
    static const CPen penSelected_;
    static CBrush brushBackground_;
    static CBrush brushShadow_;
    static CBrush brushCard_;
    static CBrush brushFrame_;
    static CBrush brushSelected_;
    static Font fontColumnTitle_;
    static Font fontCardTitle_;
    static Font fontCardText_;
  };

}
