#pragma once

namespace Kanban
{

  class Column
  {
  public:
    Column(std::string title = "new Column");

    Column(CArchive& ar);
    void Serialize(CArchive& ar) const;

  private:
    std::string title_{};

    std::vector<Card*> card_{};
  };

}
