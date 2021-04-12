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

  void Card::Draw(CDC* pDC, const CPoint& point, UIStatus s, size_t width) const
  {
    if (width != GetWidth() || point!=rCard_.TopLeft()) bValid_ = false;   // if width changed, need to recalculate dimensions

    if (!bValid_ || s == UIStatus::Dragging)  // calculate dimensions if needed
    {
      rCard_ = { point.x, point.y, point.x + (int) width, point.y };

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

    if (s == UIStatus::Selected)  // if selected, double outline
    {
      CRect r{ rCard_ };
      r.InflateRect(2, 2);
      pDC->Rectangle(r);
    }

    // draw the card itself
    CBrush b{ RGB(0,0,0) };
    pDC->FrameRect(rCard_, &b);
    pDC->FrameRect(rTitle_, &b);
    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->TextOut((rTitle_.left + rTitle_.right) / 2, rTitle_.top, title_.c_str());
    pDC->SetTextAlign(TA_TOP | TA_LEFT | DT_NOCLIP);
    pDC->DrawText(text_.c_str(), text_.size(), &rText_, DT_TOP | DT_LEFT | DT_NOCLIP | DT_WORDBREAK);
  }
  
  void Card::DrawDragged(CDC* pDC, const CPoint& p) const
  {
    CSize offset = CPoint{ rCard_.left, rCard_.top } - p;
    CRect rCard = rCard_ - offset;
    CRect rTitle = rTitle_ - offset;
    CRect rText = rText_ - offset;

    HPEN hPen{};
    HGDIOBJ hPenOriginal{};
//    hPen = ::CreatePen(PS_DOT, 1, BLACK_PEN);
    hPen = ::CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    hPenOriginal = pDC->SelectObject(hPen);

// double outline
    CRect r{ rCard };
    r.InflateRect(2, 2);
    pDC->Rectangle(r);

    // draw the card itself
    CBrush b{ RGB(0,0,0) };
    pDC->FrameRect(rCard, &b);
    pDC->FrameRect(rTitle, &b);
    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->TextOut((rTitle.left + rTitle.right) / 2, rTitle.top, title_.c_str());
    pDC->SetTextAlign(TA_TOP | TA_LEFT | DT_NOCLIP);
    pDC->DrawText(text_.c_str(), text_.size(), &rText, DT_TOP | DT_LEFT | DT_NOCLIP | DT_WORDBREAK);

    pDC->SelectObject(hPenOriginal);
    ::DeleteObject(hPen);  // cleanup
  }

  CRect Card::GetRect(void) const noexcept { return rCard_; }
  size_t Card::GetWidth(void) const noexcept { return rCard_.Width(); }
  size_t Card::GetHeight(void) const noexcept { return rCard_.Height(); }
  
  bool Card::PtInCard(const CPoint& point, CSize& offset) const noexcept
  {
    if (rCard_.PtInRect(point))
    {
      offset = CSize{point.x - rCard_.left, point.y - rCard_.top };
      return true;
    }
    return false;
  }

}