
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
    constexpr static size_t horizontalspace{ 10U };                                      // horizontal space between columns
    constexpr static size_t verticalspace{ 10U };                                        // vertical space between cards
    constexpr static size_t default_columnwidth{ 200U };                                 // default width of new columns
    constexpr static size_t default_columnheight{ 20U };                                 // height of column header
    constexpr static size_t default_cardwidth{ default_columnwidth - horizontalspace };  // default width of new cards
    constexpr static size_t default_cardheight{ 80U };                                   // defaultheight of new cards

  public:  // reusable GDI objects
    static const inline CPen penDragging_{ PS_SOLID, 2, RGB(255, 0, 0) };
    static const inline CPen penSelected_{ PS_SOLID, 2, RGB(0, 255, 0) };
    static inline CBrush brushFrame_{ RGB(0,0,0) };
    static inline CBrush brushSelected_{ RGB(0,0,255) };
    static Font fontColumnTitle_;
    static Font fontCardTitle_;
    static Font fontCardText_;
  };

}
