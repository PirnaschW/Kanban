
namespace Kanban
{

  class Column
  {
  public:
    Column(std::string title = "new Column", size_t width = 190U) noexcept;
    ~Column(void) noexcept;

    Column(CArchive* ar);
    void Serialize(CArchive* ar) const;
    void Draw(CDC* pDC, CPoint& p) const;
    size_t GetWidth() const noexcept { return width_; }
    void AddCard(Card* c) noexcept;
    void RemoveCard(Card* c) noexcept;
    bool PtInColumn(const CPoint& p) const noexcept;
    Card* GetCard(const CPoint& p) const noexcept;

  private:
    std::string title_{};

    mutable CRect rect_{};
    std::list<Card*> cards_{};
    size_t wip_limit_{ 0 };
    std::string exit_rule_{};
    size_t width_{ 190U };
  };

}
