/********************************************************************************/
// Schedula.cpp : Implementation of the SchedulaApp class

#include "StdAfx.h"

#include "Schedula.h"

/********************************************************************************/
// CSchedulaApp - Debuging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/********************************************************************************/
// CSchedulaApp - Class Instance

CSchedulaApp SchApp;

/********************************************************************************/
// CSchedulaApp - Class Initialisation

BEGIN_MESSAGE_MAP(CSchedulaApp, CWinApp)
	//{{AFX_MSG_MAP(CSchedulaApp)
	ON_COMMAND(ID_OPTIONS_SHOWREPORT, OnOptionsShowReport)
	ON_COMMAND(ID_OPTIONS_KEEPFILEOPEN, OnOptionsKeepFileOpen)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnHelpFinder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaApp - Class Construction

CSchedulaApp::CSchedulaApp()
{
//	ReportDlg = new CReportDlg(this);
	ReportDlg = new CReportDlg();
}

/********************************************************************************/
// CSchedulaApp - Class Destruction

CSchedulaApp::~CSchedulaApp()
{
	delete ReportDlg;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaApp - Virtual Function - InitInstance()
//
// Description:	Set MFC Mode,
//				Set Registry Root Key,
//				Load Previous Settings,
//				Register Document Template
//				... some other mscv stuff
//				Open Last Used File

BOOL CSchedulaApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Set Registry Root Key

	SetRegistryKey(_T("Dark-Zone Soft"));

	// Load standard INI file options (including MRU)

	LoadStdProfileSettings();

	// Load Program Settings from Registry

	m_showreport=1;		// Set Defaults
	m_keepfileopen=0;

	if(GetProfileInt(_T("Settings"), _T("ShowReport"), -1) == -1)
	{
		WriteProfileInt(_T("Settings"), _T("ShowReport"), m_showreport);
		WriteProfileInt(_T("Settings"), _T("KeepFileOpen"), m_keepfileopen);
	}
	else
	{
		m_showreport=GetProfileInt(_T("Settings"), _T("ShowReport"), m_showreport);
		m_keepfileopen=GetProfileInt(_T("Settings"), _T("KeepFileOpen"), m_keepfileopen);
	}

	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSchedulaDoc),
		RUNTIME_CLASS(CSchedulaFrame),       // main SDI frame window
		RUNTIME_CLASS(CSchedulaTreeView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Restore Option Menu Status
	
	CMenu* pMenu = m_pMainWnd->GetMenu();
	pMenu->CheckMenuItem(ID_OPTIONS_SHOWREPORT, (m_showreport?MF_CHECKED:MF_UNCHECKED) | MF_BYCOMMAND);
	pMenu->CheckMenuItem(ID_OPTIONS_KEEPFILEOPEN, (m_keepfileopen?MF_CHECKED:MF_UNCHECKED) | MF_BYCOMMAND);

	// Enable drag/drop open

	m_pMainWnd->DragAcceptFiles();

	// Re-Open Last Used File

	if(m_keepfileopen==1)
	{
		CString File = GetProfileString(_T("Recent File List"), _T("File1"), _T(""));
		if(File!=_T(""))
			OpenDocumentFile(File);
	}

	return TRUE;
}

/********************************************************************************/
// CSchedulaApp - Virtual Function - ExitInstance()

int CSchedulaApp::ExitInstance()
{
	// Save Settings to Registry

	WriteProfileInt(_T("Settings"), _T("ShowReport"), m_showreport);
	WriteProfileInt(_T("Settings"), _T("KeepFileOpen"), m_keepfileopen);

	return CWinApp::ExitInstance();
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaDoc - Program Message Handler - OnOptionsShowReport()

void CSchedulaApp::OnOptionsShowReport()
{
	CWnd* pMain = AfxGetMainWnd();

	if (pMain != NULL)
	{
		CMenu* pMenu = pMain->GetMenu();

		if(m_showreport)
			m_showreport=FALSE;
		else
			m_showreport=TRUE;

		pMenu->CheckMenuItem(ID_OPTIONS_SHOWREPORT, (m_showreport?MF_CHECKED:MF_UNCHECKED) | MF_BYCOMMAND);
	}
}

/********************************************************************************/
// CSchedulaDoc - Program Message Handler - OnOptionsKeepFileOpen()

void CSchedulaApp::OnOptionsKeepFileOpen()
{
	CWnd* pMain = AfxGetMainWnd();

	if (pMain != NULL)
	{
		CMenu* pMenu = pMain->GetMenu();

		if(m_keepfileopen)
			m_keepfileopen=FALSE;
		else
			m_keepfileopen=TRUE;

		pMenu->CheckMenuItem(ID_OPTIONS_KEEPFILEOPEN, (m_keepfileopen?MF_CHECKED:MF_UNCHECKED) | MF_BYCOMMAND);
	}
}

/********************************************************************************/
// CSchedulaApp - Program Message Handler - OnAppAbout()

void CSchedulaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/********************************************************************************/
// Schedula.cpp - End Of File
/********************************************************************************/
