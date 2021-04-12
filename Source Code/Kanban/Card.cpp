#include "pch.h"

namespace Kanban {

  Card::Card(std::wstring title) noexcept : ID_(++lastID_), title_(title == L"" ? L"new Card " + std::to_wstring(ID_) : title_) {}
  Card::Card(CArchive* ar)                            // create from file
  {
    *ar >> ID_;
    if (lastID_ < ID_) lastID_ = ID_;

    *ar >> title_ >> text_;
  }
  void Card::Serialize(CArchive* ar) const            // save to File
  {
    *ar << ID_ << title_ << text_;
  }
  Card::~Card(void) noexcept { }

  void Card::SetText(std::wstring t) noexcept { text_ = std::move(t); bValid_ = false; }

  CRect Card::GetRectTitle(CDC* pDC) const noexcept
  {
    CRect rTitle{ rCard_ };
    rTitle.InflateRect(-1, -1);
    hTitle_ = pDC->DrawText(title_.c_str(), -1, &rTitle, DT_CALCRECT | DT_TOP | DT_CENTER | DT_WORDBREAK);
    return rTitle;
  }

  void Card::Draw(CDC* pDC, const CPoint& p, UIStatus s, size_t width) const
  {
    if (width == 0U) width = GetWidth();             // if no width given, keep previous
    else if (width != GetWidth()) bValid_ = false;   // if width changed, need to recalculate dimensions

    if (!bValid_ || s == UIStatus::Dragging)  // calculate dimensions if needed
    {
      rCard_ = { p.x, p.y, p.x + (int) width, p.y };

      rTitle_ = rCard_;
      rTitle_.InflateRect(-1, -1);
      hTitle_ = pDC->DrawText(title_.c_str(), -1, &rTitle_, DT_CALCRECT | DT_TOP | DT_CENTER | DT_WORDBREAK);
      rTitle_ = rCard_;
      rTitle_.InflateRect(-1, -1);
      rTitle_.bottom += hTitle_ + 2;

      rText_ = rCard_;
      rText_.InflateRect(-1, -1);
      rText_.top += hTitle_ + 1;
      hText_ = pDC->DrawText(text_.c_str(), -1, &rText_, DT_CALCRECT | DT_TOP | DT_LEFT | DT_NOCLIP | DT_WORDBREAK);
      rText_ = rCard_;
      rText_.InflateRect(-1, -1);
      rText_.top += hTitle_ + 3;
      rText_.bottom += hTitle_ + 5 + hText_;

      rCard_.bottom += hTitle_ + 5 + hText_;

      bValid_ = true;
    }

    HPEN hPen{};
    HGDIOBJ hPenOriginal{};
    CBrush b{ RGB(0,0,0) };
    // if dragging, switch to dotted line
    if (s == UIStatus::Dragging)
    {
      hPen = ::CreatePen(PS_DOT, 1, BLACK_PEN);
      hPenOriginal = pDC->SelectObject(hPen);
    }

    if (s == UIStatus::Dragging || s == UIStatus::Selected)  // if selected, double outline
    {
      CRect r{ rCard_ };
      r.InflateRect(2, 2);
      pDC->FrameRect(r, &b);
    }

    // draw the card itself
    pDC->FrameRect(rCard_, &b);
    pDC->FrameRect(rTitle_, &b);
    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->TextOut((rTitle_.left + rTitle_.right) / 2, rTitle_.top, title_.c_str());
    pDC->SetTextAlign(TA_TOP | TA_LEFT | DT_NOCLIP);
    pDC->DrawText(text_.c_str(), text_.size(), &rText_, DT_TOP | DT_LEFT | DT_NOCLIP | DT_WORDBREAK);

    if (s == UIStatus::Dragging)  // switch back to solid line
    {
      pDC->SelectObject(hPenOriginal);
      ::DeleteObject(hPen);  // cleanup
      bValid_ = false;
    }
  }
  
  size_t Card::GetWidth(void) const noexcept { return rCard_.Width(); }
  size_t Card::GetHeight(void) const noexcept { return rCard_.Height(); }
  
  bool Card::PtInCard(const CPoint& p) const noexcept
  {
    return (rCard_.PtInRect(p));
  }

}