
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

    void SetText(std::wstring t) noexcept;
    void Draw(CDC* pDC, const CPoint& p, UIStatus s, size_t width) const;   // display card
    void DrawDragged(CDC* pDC, const CPoint& point) const;                  // display card while being dragged
    CRect GetRect(void) const noexcept;                                     // screen rectangle of this Card
    size_t GetWidth(void) const noexcept;                                   // width of this card
    size_t GetHeight(void) const noexcept;                                  // height this card needs / wants
    bool PtInCard(const CPoint& point, CSize& offset) const noexcept;       // check if a point is inside this card

  private:
    CRect GetRectTitle(CDC* pDC) const noexcept;

  private:
    ID ID_{};
    static inline ID lastID_{ 0U };
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

    mutable bool bValid_{ false };       // buffered data is valid
    mutable CRect rTitle_{ 0,0,0,0 };    // buffered title height
    mutable size_t hTitle_{ 0U };        // buffered title height
    mutable CRect rText_{ 0,0,0,0 };     // buffered title height
    mutable size_t hText_{ 0U };         // buffered text height
    mutable CRect rCard_{ 0,0,0,0 };     // buffered total Card rectangle
  };

}
