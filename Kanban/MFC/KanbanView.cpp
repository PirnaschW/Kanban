
// KanbanView.cpp : implementation of the CKanbanView class
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKanbanView

IMPLEMENT_DYNCREATE(CKanbanView, CView)

BEGIN_MESSAGE_MAP(CKanbanView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKanbanView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONUP()
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

	return CView::PreCreateWindow(cs);
}

// CKanbanView drawing

void CKanbanView::OnDraw(CDC* pDC)
{
	CKanbanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
  MFC::DC dc(pDC);
  pDoc->doc_->Draw(dc);
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
	CView::AssertValid();
}

void CKanbanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
  if (GetDocument()->doc_->React(WM_LBUTTONDOWN, nFlags, MFC::Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CKanbanView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->doc_->React(WM_LBUTTONUP, nFlags, MFC::Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CKanbanView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (GetDocument()->doc_->React(WM_LBUTTONDBLCLK, nFlags, MFC::Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CKanbanView::OnRButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->doc_->React(WM_RBUTTONUP, nFlags, MFC::Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
  else
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }
}

void CKanbanView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (GetDocument()->doc_->React(WM_MOUSEMOVE, nFlags, MFC::Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CKanbanView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (GetDocument()->doc_->React(nChar, nRepCnt, nFlags)) GetDocument()->UpdateAllViews(nullptr);
}
