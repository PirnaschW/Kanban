
#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CKanbanView, CScrollView)

BEGIN_MESSAGE_MAP(CKanbanView, CScrollView)
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKanbanView::OnFilePrintPreview)
  ON_WM_CONTEXTMENU()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_MOUSEMOVE()
  ON_WM_CHAR()
END_MESSAGE_MAP()

// CKanbanView construction/destruction

CKanbanView::CKanbanView() noexcept
{
  // TODO: add construction code here

}

CKanbanView::~CKanbanView()
{
}

BOOL CKanbanView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CScrollView::PreCreateWindow(cs);
}

// CKanbanView drawing

void CKanbanView::OnDraw(CDC* pDC)
{
  CKanbanDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc)
    return;

  pDoc->board_->Draw(pDC);
}

void CKanbanView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CDC* pDC = GetDC();

  CSize size = GetDocument() ->board_->CalcSize(pDC);
  SetScrollSizes(MM_TEXT, size);
}

// CKanbanView printing


void CKanbanView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
  AFXPrintPreview(this);
#endif
}

BOOL CKanbanView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CKanbanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CKanbanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

// CKanbanView diagnostics

#ifdef _DEBUG
void CKanbanView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CKanbanView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}

CKanbanDoc* CKanbanView::GetDocument() const // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKanbanDoc)));
  return (CKanbanDoc*)m_pDocument;
}
#endif //_DEBUG


// CKanbanView message handlers
void CKanbanView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
  theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CKanbanView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (selected_)
  {
    CSize offset;
    selected_->PtInCard({ 0,0 }, offset);
    CRect r0{ CPoint{-offset}, selected_->GetSize() };
    r0.InflateRect(2, 2);
    InvalidateRect(&r0,true);
  }

  CSize offset{};
  selected_ = GetDocument()->board_->GetCard(point,offset);  // find clicked card - could be nullptr
  if (selected_)
  {
    dragging_ = true;     // any mouse move will now drag this card
    dragPoint_ = point;
    dragOffset_ = offset;
  }
}

void CKanbanView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (!selected_) return;

  CRect r{ dragPoint_, selected_->GetSize() };
  r.OffsetRect(-dragOffset_);
  r.InflateRect(3, 3);
  InvalidateRect(&r, true);
  dragging_ = false;
  // Note: selected_ still contains the selected Card - it is now Selected

  // the selected card gets a double outline
  CDC* pDC{ GetDC() };
  CSize offset;
  selected_->PtInCard({ 0,0 }, offset);
  CRect r0{ CPoint{-offset}, selected_->GetSize() };
  r0.InflateRect(2, 2);
  ValidateRect(&r0);
  pDC->Rectangle(r0);    // use method Rectangle, not FrameRect, to clear background
  selected_->Draw(pDC, CPoint(-offset), false);
}

void CKanbanView::OnLButtonDblClk(UINT nFlags, CPoint p)
{
  if (!selected_) return;

  DlgCard dlg(selected_);
  dlg.DoModal();
  selected_->SetText(L"new shorter text!");

  CRect r{ /*selected_->GetRect()*/ };
  r.InflateRect(2, 2);
  r.bottom = (std::numeric_limits<decltype(r.bottom)>::max)();
  InvalidateRect(&r, true);
}

void CKanbanView::OnRButtonUp(UINT nFlags, CPoint point)
{
  ClientToScreen(&point);
  OnContextMenu(this, point);
}

void CKanbanView::OnMouseMove(UINT nFlags, CPoint point)
{
  static size_t z = 0U;
  z++;

  if (!dragging_) return;
  CRect r{ dragPoint_, selected_->GetSize() };
  r.OffsetRect(-dragOffset_);
  r.InflateRect(3, 3);
  InvalidateRect(&r, true);

  dragPoint_ = point;  // safe current point

  if (z > 400)
  {
    z++;
  }
  r = { dragPoint_, selected_->GetSize() };
  r.OffsetRect(-dragOffset_);
  r.InflateRect(2, 2);
  CDC* pDC{ GetDC() };
  // when dragged, the card gets a double outline, and we need to clear the background
  HGDIOBJ hPenOriginal = pDC->SelectObject(Kanban::UI::penDragging_);  // change Pen
  pDC->Rectangle(r);                                           // use method Rectangle, not FrameRect, to clear background
  pDC->SelectObject(hPenOriginal);                             // change Pen back

  selected_->Draw(pDC, dragPoint_ - dragOffset_,false); // draw the card itself, but don't save location
  ReleaseDC(pDC);

  ValidateRect(&r);
}

void CKanbanView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// ?
}
