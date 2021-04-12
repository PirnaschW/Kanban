
#include "pch.h"

IMPLEMENT_DYNAMIC(DlgCard, CDialogEx)

DlgCard::DlgCard(const Kanban::Card* c, CWnd* pParent) : CDialogEx(IDD_DLGCARD, pParent), card_(c)
{

}

DlgCard::~DlgCard()
{
}

void DlgCard::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgCard, CDialogEx)
END_MESSAGE_MAP()


// DlgCard message handlers
