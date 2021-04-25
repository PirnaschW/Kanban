
namespace Kanban
{

  class Card  // a Card
  {
  public:
    Card(std::wstring title = L"") noexcept;          // create a new Card
    Card(CArchive* ar);                               // create from file
    void Serialize(CArchive* ar) const;               // save to file
    ~Card(void) noexcept;                             // destructor
    operator ID(void) const noexcept { return ID_; }  // cast to ID

// modifying the Card
    bool Edit(void);                                  // edit the Card data (in a dialog)
    void SetText(std::wstring t) noexcept;            // set the text of the Card
    void SetWidth(size_t width) noexcept;             // set the width of this card

// sizing and drawing
    CSize CalcSize(CDC* pDC) const noexcept;          // calculate Card size and text line breaks
    size_t GetWidth(void) const noexcept;             // get the width of this card
    size_t GetHeight(void) const noexcept;            // get the height this card needs / wants
    CSize GetSize(void) const noexcept;               // overall size of this Card

    void Draw(CDC* pDC, const CPoint& p, bool saveLoc = true) const noexcept; // display card
    bool PtInCard(const CPoint& point, CSize& offset) const noexcept;         // check if a point is inside this card
    //void Invalidate(CDC* pDC) const noexcept;         // invalidate Card rectangle

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
    mutable CPoint point_{ 0,0 };               // Card location (absolute screen coordinates)
    mutable bool bValid_{ false };              // further buffered data is valid
    mutable CRect titleRect_{ 0,0,0,0 };        // Title frame (Card-relative coordinates)
    mutable Lines titleLines_{};                // Title broken into lines
    mutable CRect textRect_{ 0,0,0,0 };         // Text frame (Card-relative coordinates)
    mutable Lines textLines_{};                 // Text broken into lines
  };

}
