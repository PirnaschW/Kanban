
namespace Kanban
{

  class Card
  {
  public:
    Card(std::string title = "new Card", size_t height = UIDim::default_cardheight);

    Card(CArchive* ar);
    void Serialize(CArchive* ar) const;
    void Draw(CDC* pDC, CPoint& p, size_t width) const;
    size_t GetHeight(void) const noexcept;

  private:
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
