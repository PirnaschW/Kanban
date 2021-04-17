
namespace Kanban
{

  class Column
  {
  public:
    Column(std::wstring title = L"", size_t width = UI::default_columnwidth) noexcept;    // create a new Column
    Column(CArchive* ar);                                           // create from file
    void Serialize(CArchive* ar) const;                             // save to file
    ~Column(void) noexcept;                                         
    operator ID(void) const noexcept { return ID_; }                // cast to ID

    CSize CalcSize(CDC* pDC) const noexcept;
    void SetWidth(size_t width) noexcept;                           // set width for this Column (user input)
    size_t GetWidth() const noexcept;                               
    size_t GetHeight() const noexcept;                              // height of the column's header only
    Card* GetCard(const CPoint& p, CSize& offset) const noexcept;   // find card under this point
                                                                    
    void Draw(CDC* pDC, const CPoint& p) const;                     // display Column starting at p
    bool PtInColumn(const CPoint& p) const noexcept;
    bool RectInColumn(const CRect& r) const noexcept;

  private:
    // Column data
    ID ID_{};
    static inline ID lastID_{ 0U };
    std::wstring title_{};
    size_t wip_limit_{ 0 };
    std::wstring exit_rule_{};
    size_t width_{UI::default_columnwidth};   // user-defined column width
    std::vector<Card*> card_{};                  // all Cards

  private:
    mutable bool bValid_{ false };              // buffered data is valid
    mutable CRect titleRect_{ 0,0,0,0 };        // Title frame
    mutable Lines titleLines_{};                // Title broken into lines

  };

}
