// DlgCard.cpp : implementation file
//

#include "pch.h"

// DlgCard dialog

IMPLEMENT_DYNAMIC(DlgCard, CDialogEx)

DlgCard::DlgCard(Kanban::Card& c, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGCARD, pParent)
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
