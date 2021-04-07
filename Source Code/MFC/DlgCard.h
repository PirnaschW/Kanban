#pragma once


// DlgCard dialog

class DlgCard : public CDialogEx
{
  DECLARE_DYNAMIC(DlgCard)

public:
  DlgCard(Kanban::Card& c, CWnd* pParent = nullptr);   // standard constructor
  virtual ~DlgCard();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_DLGCARD };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
};
