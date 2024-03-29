
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
  DECLARE_DYNCREATE(CChildFrame)
public:
  CChildFrame() noexcept;

// Attributes
protected:
  CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
  virtual ~CChildFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
  DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnterSizeMove();
    afx_msg void OnExitSizeMove();
};
