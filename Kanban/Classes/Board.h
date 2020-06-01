#pragma once

namespace Kanban
{

  class Board
  {
  public:
    Board(size_t n);
    Board(std::string title = "new Board");
    ~Board(void);

    Board(CArchive& ar);
    void Serialize(CArchive& ar) const;

  private:
    std::string title_{};

    std::vector<Column*> column_{};
  };

}
