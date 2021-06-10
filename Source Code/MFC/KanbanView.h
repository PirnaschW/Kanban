
class CKanbanView : public CScrollView
{
protected: // create from serialization only
  CKanbanView() noexcept;
  DECLARE_DYNCREATE(CKanbanView)

// Attributes
public:
  CKanbanDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
  virtual void OnInitialUpdate(); // called first time after construct
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
  virtual ~CKanbanView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:
  void OnResizeStart();   // called from ChildFrm
  void OnResizeEnd();     // called from ChildFrm

protected:
private:
  // situational & display data
  Kanban::itCard selected_{};            // currently selected / dragged Card
  bool dragging_{ false };               // currently dragging a card
  bool resizing_{ false };               // currently resizing the view
  CPoint dragPoint_{};                   // point the Card is dragged to
  CSize dragOffset_{};                   // offset between click and card corner
  CRect oldsize_{};                      // client size when resizing starts

// Generated message map functions
protected:
  //{{AFX_MSG(CKanbanView)
  afx_msg void OnFilePrintPreview();
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in KanbanView.cpp
inline CKanbanDoc* CKanbanView::GetDocument() const
   { return reinterpret_cast<CKanbanDoc*>(m_pDocument); }
#endif
