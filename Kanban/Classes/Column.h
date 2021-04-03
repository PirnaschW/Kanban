
namespace Kanban
{

  class Column
  {
  public:
    Column(std::string title = "new Column", size_t width = 190U);

    Column(Archive& ar);
    void Serialize(Archive& ar) const;
    void Draw(UIContext UIC) const;
    size_t GetWidth() const noexcept { return width_; }

  private:
    std::string title_{};

    std::vector<std::unique_ptr<Card>> cards_{};
    size_t wip_limit_{ 0 };
    std::string exit_rule_{};
    size_t width_{ 190U };
  };

}
