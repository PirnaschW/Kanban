
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
    ON_WM_ENTERSIZEMOVE()
  ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
  // TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
  if( !CMDIChildWndEx::PreCreateWindow(cs) )
    return FALSE;

  return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
  CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers


void CChildFrame::OnEnterSizeMove()
{
    CMDIChildWndEx::OnEnterSizeMove();
    dynamic_cast<CKanbanView*>(GetActiveView())->OnResizeStart();
}


void CChildFrame::OnExitSizeMove()
{
  CMDIChildWndEx::OnExitSizeMove();
  dynamic_cast<CKanbanView*>(GetActiveView())->OnResizeEnd();
}
