
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
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
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


  if (resizing_)
  {
    CRect client{};
    GetClientRect(&client);
  //    ValidateRect(oldsize_);
    if (client.bottom > oldsize_.bottom)
    {
      CRect b{ 0,oldsize_.bottom,oldsize_.right,client.bottom };
      pDoc->board_->Draw(pDC, b);
//      InvalidateRect(b, true);
    }
    if (client.right > oldsize_.right)
    {
      CRect r{ oldsize_.right,0,client.right,client.bottom };
      //InvalidateRect(r, true);
      pDoc->board_->Draw(pDC, r);
    }
    oldsize_ = client;
  }

  else
  {
    CRect clip{};
    pDC->GetClipBox(&clip);
    pDoc->board_->Draw(pDC, clip);
  }
}

void CKanbanView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CDC* pDC = GetDC();

  CSize size = GetDocument() ->board_->CalcSize(pDC);
  SetScrollSizes(MM_TEXT, size);
  selected_ = GetDocument()->board_->NoCard();
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
  if (selected_ != GetDocument()->board_->NoCard())
  {
    CSize offset;
    (*selected_)->PtInCard({ 0,0 }, offset);
    CRect r0{ CPoint{-offset}, (*selected_)->GetSize(true) };
//    r0.InflateRect(2, 2);
    InvalidateRect(&r0, true);
  }

  CSize offset{};
  CSize scrolled{ GetScrollPosition() };
  selected_ = GetDocument()->board_->GetCard(point + scrolled, offset);  // find clicked card - could be nullptr
  if (selected_ != GetDocument()->board_->NoCard())
  {
    dragging_ = true;     // any mouse move will now drag this card
    dragPoint_ = point + scrolled;
    dragOffset_ = offset;
  }
  else
  {
    dragging_ = false;
  }
}

void CKanbanView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (selected_ == GetDocument()->board_->NoCard()) return;

  CSize scrolled{ GetScrollPosition() };
  // invalidate the last place the Card was dragged to
  CRect r{ dragPoint_, (*selected_)->GetSize(true) };
  r.OffsetRect(-dragOffset_ - scrolled);
  InvalidateRect(&r, true);
  dragging_ = false;
  // Note: selected_ still contains the selected Card - it is now Selected

  // invalidate the home of the Card
  CSize offset;
  (*selected_)->PtInCard({ 0,0 }, offset);   // trick to read Card's screen coordinates
  CRect r0{ CPoint{-offset - scrolled}, (*selected_)->GetSize(true) };
  InvalidateRect(&r0);

  // the selected card gets a double outline
  //CDC* pDC{ GetDC() };
  //CSize offset;
  //selected_->PtInCard({ 0,0 }, offset);
  //CRect r0{ CPoint{-offset}, selected_->GetSize(true) };
  //ValidateRect(&r0);
  //pDC->Rectangle(r0);    // use method Rectangle, not FrameRect, to clear background
  //selected_->Draw(pDC, r0, CPoint(-offset), false);
}

void CKanbanView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (selected_ == GetDocument()->board_->NoCard()) return;

  CRect r{ dragPoint_, (*selected_)->GetSize(true) };
  r.OffsetRect(-dragOffset_);
  r.bottom = GetTotalSize().cy;
  
  if ((*selected_)->Edit()) InvalidateRect(&r, true);  // if Card was changed, invalidate it and all Cards below it
}

void CKanbanView::OnRButtonUp(UINT nFlags, CPoint point)
{
  ClientToScreen(&point);
  OnContextMenu(this, point);
}

void CKanbanView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (!dragging_) return;

  CSize scrolled{ GetScrollPosition() };

  CRect r{ dragPoint_ - dragOffset_ - scrolled, (*selected_)->GetSize(true) };
 // r.OffsetRect(-dragOffset_);
  r.InflateRect(1, 1, 3, 3);
  InvalidateRect(&r, true);

  dragPoint_ = point + scrolled;  // safe current point

  r = { dragPoint_, (*selected_)->GetSize(true) };
  r.OffsetRect(-dragOffset_ - scrolled);
  // when dragged, the card gets a double outline, and we need to clear the background
  CDC* pDC{ GetDC() };
  ValidateRect((*selected_)->DrawPlaceholder(pDC,scrolled));               // draw a placeholder in original location
  (*selected_)->Draw(pDC, r, dragPoint_ - dragOffset_ - scrolled, false);     // draw the card at the mouse position, but don't save location
  ReleaseDC(pDC);

  ValidateRect(&r);
}

void CKanbanView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// ?
}


void CKanbanView::OnSize(UINT nType, int cx, int cy)
{
 // CScrollView::OnSize(nType, cx, cy);
  //if (resizing_)
  //{
  //  ValidateRect(oldsize_);
  //  CRect client{};
  //  GetClientRect(&client);
  //  if (client.bottom > oldsize_.bottom)
  //  {
  //    CRect b{ 0,oldsize_.bottom,client.right,client.bottom };
  //    InvalidateRect(b, true);
  //  }
  //  if (client.right > oldsize_.right)
  //  {
  //    CRect r{ oldsize_.right,0,client.right,client.bottom };
  //    InvalidateRect(r, true);
  //  }

  //  oldsize_ = client;
  //}
}


void CKanbanView::OnResizeStart()
{
  resizing_ = true;
  GetClientRect(&oldsize_);
}


void CKanbanView::OnResizeEnd()
{
  resizing_ = false;

  CRect client{};
  GetClientRect(&client);
//  InvalidateRect(client, true);

//  ValidateRect(oldsize_);
}


BOOL CKanbanView::OnEraseBkgnd(CDC* pDC)
{
  if (resizing_)
  {
    CRect client{};
    GetClientRect(&client);
  //    ValidateRect(oldsize_);
    if (client.bottom > oldsize_.bottom)
    {
      CRect b{ 0,oldsize_.bottom,oldsize_.right,client.bottom };
      pDC->FillRect(b, &Kanban::UI::brushBackground_);
    }
    if (client.right > oldsize_.right)
    {
      CRect r{ oldsize_.right,0,client.right,client.bottom };
      pDC->FillRect(r, &Kanban::UI::brushBackground_);
    }
    return false;
  }

  else
    return CScrollView::OnEraseBkgnd(pDC);
}
