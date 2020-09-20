/********************************************************************************/
// Schedula.h : Interface of the CSchedulaApp class

#pragma once

/********************************************************************************/

#include <stdio.h>
#include <math.h>

#include "Resource.h"

#include ".\SchLib\SchLib.h"

#include "SchedulaDoc.h"
#include "SchedulaEngine.h"
#include "SchedulaFrame.h"
#include "SchedulaInput.h"
#include "SchedulaTreeView.h"
#include "SchedulaListView.h"
#include "SchedulaPlanView.h"

/********************************************************************************/
// CSchedulaApp - Class Definition

class CSchedulaApp : public CWinApp
{
protected:
	//DECLARE_DYNCREATE(CSchedulaApp)

	//{{AFX_VIRTUAL(CSchedulaApp)
	// Virtual Functions
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaApp)
	// Program Message Handlers
	afx_msg void OnAppAbout();
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:

	BOOL m_showreport;
	BOOL m_keepfileopen;

protected:
	void OnOptionsShowReport();
	void OnOptionsKeepFileOpen();

public:
	CSchedulaApp();
	~CSchedulaApp();

	inline BOOL IsShowReport() { return m_showreport; } 

//TODO: Make private
	CReportDlg* ReportDlg;
};

extern CSchedulaApp SchApp;

/********************************************************************************/
// CSchedulaApp.h - End Of File
/********************************************************************************/
