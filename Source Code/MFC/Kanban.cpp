
// Kanban.cpp : Defines the class behaviors for the application.
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKanbanApp

BEGIN_MESSAGE_MAP(CKanbanApp, CWinAppEx)
  ON_COMMAND(ID_APP_ABOUT, &CKanbanApp::OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CKanbanApp construction

CKanbanApp::CKanbanApp() noexcept
{
  m_bHiColorIcons = TRUE;

  // support Restart Manager
  m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
  // If the application is built using Common Language Runtime support (/clr):
  //     1) This additional setting is needed for Restart Manager support to work properly.
  //     2) In your project, you must add a reference to System.Windows.Forms in order to build.
  System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

  // recommended format for string is CompanyName.ProductName.SubProduct.VersionInformation
  SetAppID(_T("WP.Kanban.1000"));

}

// The one and only CKanbanApp object

CKanbanApp theApp;


// CKanbanApp initialization

BOOL CKanbanApp::InitInstance()
{
  CWinAppEx::InitInstance();


  EnableTaskbarInteraction(FALSE);

  // AfxInitRichEdit2() is required to use RichEdit control
  // AfxInitRichEdit2();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need
  // Change the registry key under which our settings are stored
  SetRegistryKey(_T("WP//Kanban"));
  LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)


  InitContextMenuManager();

  InitKeyboardManager();

  InitTooltipManager();
  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
    RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(IDR_KanbanTYPE,
    RUNTIME_CLASS(CKanbanDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CKanbanView));
  if (!pDocTemplate)
    return FALSE;
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
  m_pMainWnd = pMainFrame;

  // call DragAcceptFiles only if there's a suffix
  //  In an MDI app, this should occur immediately after setting m_pMainWnd
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Enable DDE Execute open
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);


  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  // The main window has been initialized, so show and update it
  pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
  pMainFrame->UpdateWindow();

  return TRUE;
}

int CKanbanApp::ExitInstance()
{
  return CWinAppEx::ExitInstance();
}

// CKanbanApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
public:
//    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// App command to run the dialog
void CKanbanApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

// CKanbanApp customization load/save methods

void CKanbanApp::PreLoadState()
{
  BOOL bNameValid;
  CString strName;
  bNameValid = strName.LoadString(IDS_EDIT_MENU);
  ASSERT(bNameValid);
  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CKanbanApp::LoadCustomState()
{
}

void CKanbanApp::SaveCustomState()
{
}

// CKanbanApp message handlers
