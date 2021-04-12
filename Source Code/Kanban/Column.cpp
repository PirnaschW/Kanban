#include "pch.h"

namespace Kanban {

  Column::Column(std::wstring title, size_t width) noexcept : ID_(++lastID_), title_(title == L"" ? L"new Column " + std::to_wstring(ID_) : title_), width_(width)
  {
    card_.reserve(UIDim::dummycards);
    for (size_t z = 0; z < UIDim::dummycards; ++z)
    {
      //Card* c = new Card();
      //card_.push_back(c);
      Card* c = card_.emplace_back(new Card());
      std::wstring text = L"Text " + std::to_wstring((ID)*c) + L" ";
      for (size_t i = 0U; i < 10; ++i) text += L"Lorem a b c Ipsum d e f ";
      c->SetText(text + L"Lorem Ipsum");
    }
  }
  Column::Column(CArchive* ar)
  {
    *ar >> ID_;
    if (lastID_ < ID_) lastID_ = ID_;

    size_t z;
    *ar >> title_ >> width_ >> z;
    card_.reserve(z);
    for (size_t i = 0U; i < z; ++i)
      card_.push_back(new Card(ar));
  }
  void Column::Serialize(CArchive* ar) const
  {
    *ar << ID_;

    *ar << title_ << width_ << card_.size();
    for (const auto& c : card_) c->Serialize(ar);
  }
  Column::~Column(void) noexcept
  {
    for (auto& c : card_) delete c;
  }

  void Column::Draw(CDC* pDC, const CPoint& p) const
  {
    // calculate target rect
    rCard_ = { p.x, p.y, p.x + (int) GetWidth(), p.y };
    // draw column header
    rCard_.bottom += pDC->DrawText(title_.c_str(), -1, &rCard_, DT_TOP | DT_CENTER) + 1;

    CRect r{ rCard_ };
    r.InflateRect(3, 3);
    pDC->Rectangle(r);
    r.InflateRect(-2, -2);
    pDC->Rectangle(r);
    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->TextOut((rCard_.left + rCard_.right) / 2, rCard_.top + 1, title_.c_str());

    // draw all cards
    CRect clip;
    pDC->GetClipBox(&clip);
    for (const auto& c : card_)
    {
      if (rCard_.bottom + (int) c->GetHeight() > clip.top && rCard_.bottom < clip.bottom)   // only redraw the card if it is invalid
      {
        c->Draw(pDC, { rCard_.left, rCard_.bottom + (int) UIDim::verticalspace }, UIStatus::Normal, GetWidth());
      }
      rCard_.bottom += c->GetHeight() + UIDim::verticalspace;
    }
  }

  void Column::SetWidth(size_t width) noexcept { width_ = width; }
  size_t Column::GetWidth(void) const noexcept { return width_; }
  size_t Column::GetHeight(void) const noexcept { return rCard_.Height(); }


  Card* Column::GetCard(const CPoint& point, CSize& offset) const noexcept
  {
    for (const auto& c : card_) if (c->PtInCard(point,offset)) return c;
    return nullptr;
  }

  bool Column::PtInColumn(const CPoint& p) const noexcept
  {
    return (rCard_.PtInRect(p));
  }

  bool Column::RectInColumn(const CRect& r) const noexcept
  {
    CRect i;
    i.IntersectRect(r, rCard_);
    return !i.IsRectEmpty();
  }

}
