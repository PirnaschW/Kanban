
namespace Kanban
{
  class Column;

  class Card
  {
  public:
    Card(Column* col, std::string title = "new Card", size_t height = UIDim::default_cardheight) noexcept;
    ~Card(void) noexcept;

    Card(CArchive* ar);                                  // create from file
    void Serialize(CArchive* ar) const;                  // save to file
    void Draw(CDC* pDC, CPoint& p, size_t width) const;  // display at point p
 
    size_t GetHeight(void) const noexcept;
    Column* SetColumn(Column* col) noexcept;
    bool PtInCard(const CPoint& p) const noexcept;
    void Select(bool s) const noexcept;

  private:
    // organizational info
    mutable Column* col_;               // column the card is in
    mutable bool selected_{ false };
    mutable CRect rect_{};

    // card data
    std::string title_{};
    std::string text_{};
    size_t height_{};
    size_t size_{ 0 };
    std::string owner_{};
    size_t priority_{ 0 };
    time_t planned_start_{ 0LL };
    time_t planned_end_{ 0LL };
    time_t actual_start_{ 0LL };
    time_t actual_end_{ 0LL };
    size_t type_{ 0 };
    size_t color_{ 0 };
  };

}
