/********************************************************************************/
// SchedulaFrame.cpp : Implementation of the CSchedulaFrame class

#include "StdAfx.h"

#include "Schedula.h"

//#include "SchedulaFrame.h"

/********************************************************************************/
// CSchedulaFrame - Debuging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
void CSchedulaFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}
void CSchedulaFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/********************************************************************************/
// Globals

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/********************************************************************************/
// CSchedulaFrame - Class Initialisation

IMPLEMENT_DYNCREATE(CSchedulaFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CSchedulaFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSchedulaFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaFrame - Class Construction

CSchedulaFrame::CSchedulaFrame()
{
}

/********************************************************************************/
// CSchedulaFrame - Class Destruction

CSchedulaFrame::~CSchedulaFrame()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaFrame - Virtual Function - PreCreateWindow()
//
// Description: Modify Window Style,
//				Set Window Dimensions
//

BOOL CSchedulaFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = cs.style | WS_MAXIMIZE ;
	cs.dwExStyle = cs.dwExStyle | 0x00;

	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	cs.y = 0;
	cs.x = 0;

	return TRUE;
}

/********************************************************************************/
// CSchedulaFrame - Virtual Function - OnCreateClient()
//
// Description: Create 3 Way Split Client Area
//

BOOL CSchedulaFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (!m_wndSplitterV.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitterH.CreateStatic(&m_wndSplitterV,2,1,WS_CHILD | WS_VISIBLE | WS_BORDER,
		 m_wndSplitterV.IdFromRowCol(0, 0) ))
		return FALSE;

	if (!m_wndSplitterH.CreateView(0, 0,RUNTIME_CLASS(CSchedulaTreeView), CSize(0, 0), pContext) ||
		!m_wndSplitterH.CreateView(1, 0,RUNTIME_CLASS(CSchedulaListView), CSize(0, 0), pContext))
		return FALSE;

	if (!m_wndSplitterV.CreateView(0, 1,RUNTIME_CLASS(CSchedulaPlanView), CSize(0, 0), pContext))
		return FALSE;

	m_wndSplitterH.SetRowInfo(0,250,0);
	m_wndSplitterV.SetColumnInfo(0,200,0);

	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaFrame - System Message Handler - OnCreate()
//
// Description: Create Window,
//				Add Toolbar,
//				Add Status Bar,
//				Make Toolbar Dockable
//

int CSchedulaFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

	return 0;
}

/********************************************************************************/
// CSchedulaFrame.cpp - End Of File
/********************************************************************************/
