
namespace Kanban
{

  class Board
  {
  public:
    Board(size_t n);
    Board(std::string title = "new Board");
    ~Board(void);

    Board(Archive& ar);
    void Serialize(Archive& ar) const;
    void Draw(DC& dc) const;

  private:
    std::string title_{};

    std::vector<std::unique_ptr<Column>> columns_{};
    std::vector<std::string> owners_{};
    // filter_
    double scale_{ 1.0 };

  };

}
