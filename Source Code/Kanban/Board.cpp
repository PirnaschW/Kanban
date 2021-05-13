#include "pch.h"

namespace Kanban
{

  Board::Board(size_t n) noexcept : title_(L"Kanban Board " + std::to_wstring(n))
  {
    //column_.reserve(UI::dummycolumns);
    for (size_t z = 0; z < UI::dummycolumns; ++z)
    {
      size_t width = 120U + rand() % 120;
      itColumn iColumn = column_.insert(column_.cend(),new Column(L"", width));  // add a random width column at the end
      for (size_t z = 0; z < UI::dummycards; ++z)                                          // fill it with some cards
      {
        itCard iCard = card_.insert(card_.cend(),new Card());
        std::wstring text = L"Text " + std::to_wstring((ID) *(*iCard)) + L" ";
        for (size_t i = 0U; i < 4U + rand() % 10U; ++i) text += L"Lorem a b c Ipsum d e f ";
        (*iCard)->SetText(text + L"Lorem Ipsum");
        (*iCard)->SetColumn(iColumn);
        (*iCard)->SetColor(RGB(224 + rand() % 32, 224 + rand() % 32, 224 + rand() % 32));
      }
    }
  }
  Board::Board(CArchive* ar)
  {
    size_t zColumn{0};
    size_t zCard{0};
    *ar >> title_ >> zCard >> zColumn;

    for (size_t i = 0U; i < zCard; ++i) card_.insert(card_.cend(),new Card(ar));            // read all Cards from archive
    for (size_t i = 0U; i < zColumn; ++i) column_.insert(column_.cend(), new Column(ar));   // read all Columns from archive
  }
  void Board::Serialize(CArchive* ar) const
  {
    *ar << title_ << card_.size() << column_.size();
    for (const auto* c : card_) c->Serialize(ar);
    for (const auto* c : column_) c->Serialize(ar);
  }
  Board::~Board(void) noexcept
  {
    for (auto& c : card_) delete c;
    for (auto& c : column_) delete c;
  }


  CSize Board::CalcSize(CDC* pDC) const noexcept
  {
    if (UI::fontCardTitle_.cy_ == 0U)
    {
      UI::fontColumnTitle_.font_.CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontColumnTitle_.GetCharWidths(pDC);
      UI::fontCardTitle_.font_.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontCardTitle_.GetCharWidths(pDC);
      UI::fontCardText_.font_.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
      UI::fontCardText_.GetCharWidths(pDC);
    }

    CSize size{ UI::horizontalspace,0 };
    for (const auto& c : column_)
    {
      CSize cSize = c->CalcSize(pDC);
      size.cx += cSize.cx + UI::horizontalspace;
      size.cy = (std::max)(size.cy,cSize.cy);
    }
    size.cy += UI::verticalspace;
    for (itCard it = card_.cbegin(); it != card_.cend(); ++it) (*it)->CalcSize(pDC);
    return size;
  }

  itCard Board::NoCard() const noexcept { return card_.cend(); }
  itCard Board::GetCard(const CPoint& point, CSize& offset) const noexcept
  {
    for (itCard it = card_.cbegin(); it != card_.cend(); ++it) if ((*it)->PtInCard(point, offset)) return it;
    return card_.cend();
  }

  void Board::Draw(CDC* pDC, const CRect& clip) const
  {
    CPoint p{ UI::horizontalspace, UI::verticalspace};           // start drawing at upper left
    for (itColumn iColumn = column_.cbegin(); iColumn != column_.cend(); ++iColumn)
    {
      p.y = UI::verticalspace;                                   // restart for each Column at the top
      (*iColumn)->Draw(pDC, clip, p);                            // draw the column header
      p.y += (*iColumn)->GetHeight();

      // draw all cards of this column
      for (itCard icard = card_.cbegin(); icard != card_.cend(); ++icard)
      {
        if ((*icard)->GetColumn() == iColumn)                    // ignore Cards not in current column
        {
          p.y += UI::verticalspace;                              // move point down to leave some space
          (*icard)->Draw(pDC, clip, p);                          // draw the card at the point
          p.y += (*icard)->GetSize().cy;                         // move point down according to just drawn Card's height
        }
      }

      p.x += (*iColumn)->GetWidth() + UI::horizontalspace;       // move the point right according to just drawn Column's width
    }
  }

}
