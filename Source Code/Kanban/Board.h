
namespace Kanban
{

  class Board
  {
  public:
    Board(size_t n) noexcept;
    Board(CArchive* ar);
    void Serialize(CArchive* ar) const;
    ~Board(void) noexcept;

    CSize CalcSize(CDC* pDC) const noexcept;
    void Draw(CDC* pDC) const;
    //bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);      // react to keyboard input (not menu shortcuts, but typing)
    //bool React(unsigned int command);                                               // react to button/menu command
    //void React(CCmdUI* pUI);                                                        // react to UI events (allows to set buttons greyed, etc.)
    //void DragTo(const CPoint& p) { dragPoint_ = p; }
    //void DragStart(const CPoint& p);
    //void DragEnd(const CPoint& p);

    Card* GetCard(const CPoint& p, CSize& offset) const;
    Column* GetColumn(const CPoint& p) const;
    //void RemoveColumn(Column* c);

  private:
    // situational & display data
    //mutable Card* selected_{ nullptr };   // currently selected / dragged Card
    //mutable bool dragging_{ false };       // currently dragging a card
    //mutable CPoint dragPoint_{};           // point the Card is dragged to
    //mutable HWND hView_{};

    // object data
    std::wstring title_{};
    std::vector<Column*> column_{};      // all Columns

  };

}
