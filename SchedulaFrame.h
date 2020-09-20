/********************************************************************************/
// SchedulaFrame.h : Interface of the CSchedulaFrame class

#pragma once

/********************************************************************************/
// CSchedulaFrame - Class Definition

class CSchedulaFrame : public CFrameWnd
{
protected:
	DECLARE_DYNCREATE(CSchedulaFrame)

	//{{AFX_VIRTUAL(CSchedulaFrame)
	// Virtual Functions
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaFrame)
	// System Message Handlers
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:
	CSplitterWnd m_wndSplitterH;	// Split Client Interface
	CSplitterWnd m_wndSplitterV;

	CStatusBar  m_wndStatusBar;		// Control bar embedded members
	CToolBar    m_wndToolBar;

protected:
	CSchedulaFrame();
	virtual ~CSchedulaFrame();

public:



};

/********************************************************************************/
// CSchedulaFrame.h - End Of File
/********************************************************************************/
