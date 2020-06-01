
// Kanban.h : main header file for the Kanban application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKanbanApp:
// See Kanban.cpp for the implementation of this class
//

class CKanbanApp : public CWinAppEx
{
public:
	CKanbanApp() noexcept;

public:
	size_t next_{ 0 };

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKanbanApp theApp;
