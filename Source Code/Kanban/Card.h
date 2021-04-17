
namespace Kanban
{

  class Card  // a Card
  {
  public:
    Card(std::wstring title = L"") noexcept;                                // create a new Card
    Card(CArchive* ar);                                                     // create from file
    void Serialize(CArchive* ar) const;                                     // save to file
    ~Card(void) noexcept;                                                   
    operator ID(void) const noexcept { return ID_; }                        // cast to ID

    CSize CalcSize(CDC* pDC) const noexcept;
    void SetText(std::wstring t) noexcept;
    //CRect GetRect(void) const noexcept;                                     // screen rectangle of this Card
    void SetWidth(size_t width) noexcept;                                   // set the width of this card
    size_t GetWidth(void) const noexcept;                                   // get the width of this card
    size_t GetHeight(void) const noexcept;                                  // get the height this card needs / wants

    void Draw(CDC* pDC, const CPoint& p, UIStatus s) const;                 // display card
    //void DrawMultiText(CDC* pDC, const CPoint& point, const std::wstring& s, const Lines& l) const;
    void DrawDragged(CDC* pDC, const CPoint& point) const;                  // display card while being dragged
    bool PtInCard(const CPoint& point, CSize& offset) const noexcept;       // check if a point is inside this card

  private:
    ID ID_{};
    static inline ID lastID_{ 0U };
    size_t width_{ 0U };
    std::wstring title_{};
    std::wstring text_{};
    size_t storysize_{ 0 };
    std::wstring owner_{};
    size_t priority_{ 0 };
    time_t planned_start_{ 0LL };
    time_t planned_end_{ 0LL };
    time_t actual_start_{ 0LL };
    time_t actual_end_{ 0LL };
    size_t type_{ 0 };
    size_t color_{ 0 };

  private:
    mutable bool bValid_{ false };              // buffered data is valid
    mutable CRect titleRect_{ 0,0,0,0 };        // Title frame
    mutable Lines titleLines_{};                // Title broken into lines
    mutable CRect textRect_{ 0,0,0,0 };         // Text frame
    mutable Lines textLines_{};                 // Text broken into lines
  };

}
