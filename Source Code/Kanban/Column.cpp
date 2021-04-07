#include "pch.h"

namespace Kanban {

  Column::Column(std::string title, size_t width) noexcept : title_(title), width_(width)
  {
    for (size_t z = 0; z < 100; ++z) AddCard(new Card(this, "Card " + z, 20U + rand() % 60));
  }
  Column::~Column(void) noexcept { for (auto& c : cards_) delete c; }

  Column::Column(CArchive* ar)
  {
    size_t z;
    *ar >> title_ >> width_ >> z;
    for (size_t i = 0U; i < z; ++i) cards_.emplace_back(new Card(ar));
  }

  void Column::Serialize(CArchive* ar) const
  {
    *ar << title_ << width_ << cards_.size();
    for (const auto& card : cards_) card->Serialize(ar);
  }

  void Column::Draw(CDC* pDC, CPoint& p) const
  {
    rect_ = CRect{ p.x, p.y, p.x + (int) width_, p.y };
    for (const auto& card : cards_)
    {
      pDC->Rectangle(rect_);
      card->Draw(pDC, p, GetWidth());
      p.y += card->GetHeight() + UIDim::verticalspace;
    }
    rect_.bottom = p.y;
  }

  void Column::AddCard(Card* c) noexcept
  {
    cards_.push_back(c);
  }

  void Column::RemoveCard(Card* c) noexcept
  {
    cards_.remove(c);
  }

  Card* Column::GetCard(const CPoint& p) const noexcept
  {
    for (const auto& c : cards_) if (c->PtInCard(p)) return c;
    return nullptr;
  }

  bool Column::PtInColumn(const CPoint& p) const noexcept
  {
    return (rect_.PtInRect(p));
  }

}