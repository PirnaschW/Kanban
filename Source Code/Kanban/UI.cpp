
#include "pch.h"

namespace Kanban
{

  const unsigned char UI::backgroundGray_{ 255 };   // general window background
  const unsigned char UI::shadowGray_{ 96 };        // drop shadows
  const unsigned char UI::cardGray_{ 224 };         // card surface
  const unsigned char UI::columnGray_{ 192 };       // column header
  const COLORREF UI::backgroundColor_{ RGB(backgroundGray_,backgroundGray_,backgroundGray_) };
  const COLORREF UI::shadowColor_{ RGB(shadowGray_, shadowGray_, shadowGray_) };
  const COLORREF UI::cardColor_{ RGB(backgroundGray_, cardGray_, backgroundGray_) };
  const COLORREF UI::columnColor_{ RGB(columnGray_, columnGray_, columnGray_) };
  CPen UI::penBackground_{ PS_SOLID, 1, backgroundColor_ };
  CPen UI::penShadow_{ PS_SOLID, 1, shadowColor_ };
  CPen UI::penCard_{ PS_SOLID, 1, cardColor_ };
  const CPen UI::penDragging_{ PS_SOLID, 1, RGB(255, 0, 0) };
  const CPen UI::penSelected_{ PS_SOLID, 2, RGB(0, 255, 0) };
  CBrush UI::brushBackground_{ backgroundColor_ };
  CBrush UI::brushShadow_{ shadowColor_ };
  CBrush UI::brushCard_{ cardColor_ };
  CBrush UI::brushFrame_{ RGB(0,0,0) };
  CBrush UI::brushSelected_{ RGB(0,0,255) };


  Font UI::fontColumnTitle_{};
  Font UI::fontCardTitle_{};
  Font UI::fontCardText_{};

  size_t Lines::GetHeight(const Font& f) const noexcept { return l_.size() * f.cy_; }

  void Font::GetCharWidths(CDC* pDC)
  {
    //int cx[256];
    //auto b = pDC->GetOutputCharWidth(0,255,cx);

    CFont* f = pDC->SelectObject(&font_);
    for (wchar_t c = 0; c < cx_.size(); ++c)
    {
      const auto& cs = pDC->GetTextExtent(&c, 1);
      cx_[c] = cs.cx;
      cy_ = (std::max)((LONG) cy_,cs.cy);
    }
    pDC->SelectObject(f);
  }

  void Font::DrawMultiText(CDC* pDC, const CPoint& point, const std::wstring& text, const Lines& l) const
  {
    size_t toff = 0U;
    for (const auto& t : l.l_)
    {
      pDC->ExtTextOut(point.x + t.offset.cx, point.y + t.offset.cy, 0U, nullptr, text.c_str() + toff, t.nChars, nullptr);
      toff += t.nChars;
    }
  }


  Lines Lines::BreakLines(const Font& f, const std::wstring& s, size_t width)
  {

    Lines l{};
    l.l_.reserve(s.size() * 8U / width);
    Line tl{};
    size_t w = 0U;
    size_t lastblank = 0U;
    size_t lastblankwidth = 0U;
    for (auto& c : s)
    {
      if (w + f.cx_[c] > width)  // does not fit in current line
      {
        if (c == ' ')               // a blank is perfect - break after it
        {
          ++tl.nChars;
          l.l_.push_back(tl);
          tl.nChars = 0U;
          tl.offset.cy += f.cy_;
          w = 0U;
          lastblank = 0U;
        }
        else if (lastblank == 0U)   // single word is too long - cut it
        {
          l.l_.push_back(tl);
          tl.nChars = 1U;
          tl.offset.cy += f.cy_;
          w = f.cx_[c];
        }
        else if (tl.nChars == 0)    // must fit at least one char in each line
        {
          ++tl.nChars;
          l.l_.push_back(tl);
          tl.nChars = 0U;
          tl.offset.cy += f.cy_;
          w = 0U;
          lastblank = 0U;
        }
        else                        // go back to last blank
        {
          size_t cur = tl.nChars;
          tl.nChars = lastblank;
          l.l_.push_back(tl);
          tl.nChars = cur - lastblank + 1;
          tl.offset.cy += f.cy_;
          w = w - lastblankwidth + f.cx_[c];
          lastblank = 0U;
        }
      }
      else
      {
        ++tl.nChars;
        w += f.cx_[c];
        if (c == ' ')
        {
          lastblank = tl.nChars;  // remember last blank
          lastblankwidth = w;     // and the width til there
        }
      }
    }
    if (tl.nChars > 0U) l.l_.push_back(tl);
    return l;
  }

  COLORREF UI::Darken(COLORREF color, int amount) noexcept
  {
    return RGB(GetRValue(color) - amount, GetGValue(color) - amount, GetBValue(color) - amount);
  }
  COLORREF UI::Lighten(COLORREF color, int amount) noexcept { return Darken(color, -amount); }

}