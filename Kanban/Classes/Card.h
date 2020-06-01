#pragma once

namespace Kanban
{

  class Card
  {
  public:
    Card(std::string title = "new Card");

    Card(CArchive& ar);
    void Serialize(CArchive& ar) const;

  private:
    std::string title_{};
    std::string text_{};
    size_t size_{ 0 };
    std::string owner_{};
  };

}
