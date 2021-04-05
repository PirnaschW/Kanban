

namespace MFC {

  class Doc // use as MFC-disconnected pimpl class
  {
  public:
    Doc(size_t n);
    Doc(Archive& ar);
    ~Doc(void) noexcept;
    void Serialize(Archive& ar);
    void Draw(DC& dc);
    bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);      // react to keyboard input (not menu shortcuts, but typing)
    bool React(unsigned int command);                                               // react to button/menu command
    bool React(unsigned int event, unsigned int nFlags, const Point& p);            // react to mouse events
    void React(MFC::CmdUI* pUI);                                                    // react to UI events (allows to set buttons greyed, etc.)
    void DragTo(const MFC::Point& point) { dragPoint_ = point; }
    void DragStart(const MFC::Point&);
    void DragEnd(const MFC::Point&);

  private:
    void* pImpl_;
    const void* dragObject_{};         // currently dragged object
    MFC::Point dragPoint_{};           // point the object is dragged to
  };

}