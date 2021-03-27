
// KanbanDoc.cpp : implementation of the CKanbanDoc class
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKanbanDoc

IMPLEMENT_DYNCREATE(CKanbanDoc, CDocument)

BEGIN_MESSAGE_MAP(CKanbanDoc, CDocument)
END_MESSAGE_MAP()


// CKanbanDoc construction/destruction

CKanbanDoc::CKanbanDoc() noexcept {}

CKanbanDoc::~CKanbanDoc() {}

BOOL CKanbanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;

	board_ = std::make_unique<Kanban::Board>(++theApp.next_);
	return TRUE;
}


// CKanbanDoc serialization

void CKanbanDoc::Serialize(CArchive& ar)
{
  MFC::Archive AR(&ar);
	if (ar.IsStoring()) board_->Serialize(AR);
	else board_ = std::make_unique<Kanban::Board>(AR);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CKanbanDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CKanbanDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKanbanDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKanbanDoc diagnostics

#ifdef _DEBUG
void CKanbanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKanbanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKanbanDoc commands
