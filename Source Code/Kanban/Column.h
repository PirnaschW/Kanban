
namespace Kanban
{

  class Column
  {
  public:
    Column(std::string title = "new Column", size_t width = 190U);

    Column(CArchive* ar);
    void Serialize(CArchive* ar) const;
    void Draw(CDC* pDC, CPoint& p) const;
    size_t GetWidth() const noexcept { return width_; }

  private:
    std::string title_{};

    std::vector<std::unique_ptr<Card>> cards_{};
    size_t wip_limit_{ 0 };
    std::string exit_rule_{};
    size_t width_{ 190U };
  };

}
