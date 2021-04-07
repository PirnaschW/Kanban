
namespace Kanban
{

  class Board
  {
  public:
    Board(size_t n) noexcept;
    Board(std::string title = "new Board") noexcept;
    ~Board(void) noexcept;

    Board(CArchive* ar);
    void Serialize(CArchive* ar) const;
    void Draw(CDC* pDC) const;
    //bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);      // react to keyboard input (not menu shortcuts, but typing)
    //bool React(unsigned int command);                                               // react to button/menu command
    bool React(unsigned int event, unsigned int nFlags, const CPoint& p);             // react to mouse events
    //void React(CCmdUI* pUI);                                                        // react to UI events (allows to set buttons greyed, etc.)
    //void DragTo(const CPoint& p) { dragPoint_ = p; }
    //void DragStart(const CPoint& p);
    //void DragEnd(const CPoint& p);

    Card* GetCard(const CPoint& p) const;
    Column* GetColumn(const CPoint& p) const;
    void AddColumn(Column* c);
    void RemoveColumn(Column* c);

  private:
    std::string title_{};

    std::list<Column*> columns_{};
    std::vector<std::string> owners_{};
    // filter_
    double scale_{ 1.0 };

    Card* selected_{ nullptr };
    const void* dragObject_{};         // currently dragged object
    CPoint dragPoint_{};           // point the object is dragged to

  };

}
