/********************************************************************************/
// SchedulaDialogLib.cpp : Implementation of the SchedulaDialogLib classes

#pragma once

/********************************************************************************/
// CAboutDlg - Class Definition

class CAboutDlg : public CDialog
{
protected:
	//DECLARE_DYNAMIC(CEntryDlg)

// Generated virtual function overrides
//{{AFX_VIRTUAL(CAboutDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Generated message map functions
//{{AFX_MSG(CAboutDlg)
//}}AFX_MSG

public:
// Dialog Data
//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
//}}AFX_DATA

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:

protected:

public:
	CAboutDlg();
};

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// Macros

#define ADD_NAME		0
#define ADD_DATE		1
#define UPDATE_NAME		2
#define UPDATE_DATE		3
#define ADD_SERIES		4

/********************************************************************************/
// CEntryDlg - Class Definition

class CEntryDlg : public CDialog
{
protected:
	DECLARE_DYNAMIC(CEntryDlg)

	//{{AFX_VIRTUAL(CEntryDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEntryDlg)
	// System Message Handlers
	virtual BOOL OnInitDialog();
	// Program Message Handlers
	afx_msg void OnEntryAdd();
	afx_msg void OnEntryDone();
	//}}AFX_MSG

	//{{AFX_DATA(CEntryDlg)
	enum { IDD = IDD_ENTRYDLG };
	CEdit name;
	CDateTimeCtrl from;
	CDateTimeCtrl to;
	CComboBox type;
	//}}AFX_DATA

	// {{AFX_INSERT_LOCATION}}

	DECLARE_MESSAGE_MAP()

// User Stuff Below

private:

	CString m_name;
	CDate m_from;
	CDate m_to;
	CString m_type;

	CSchedulaDoc* pDoc;
	int m_status;

protected:

public:
	CEntryDlg(CSchedulaDoc* pDoc, int status);
	virtual ~CEntryDlg();

	inline void SetName (CString name) {m_name = name;};
	inline void SetDateFrom (CDate &from) {m_from = from;};
	inline void SetDateTo (CDate &to) {m_to = to;};
	inline void SetType (CString type) {m_type = type;};
};

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - Class Definition

class CReportDlg : public CDialog
{
protected:
	DECLARE_DYNAMIC(CReportDlg)

	//{{AFX_VIRTUAL(CReportDlg)
	// Virtual Functions
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CReportDlg)
	// System Message Handlers
	virtual BOOL OnInitDialog();
	// Program Message Handlers
	afx_msg void OnReportClose();
	//}}AFX_MSG

	//{{AFX_DATA(CReportDlg)
	enum { IDD = IDD_REPORTDLG };
	CListCtrl list;
	//}}AFX_DATA

	// {{AFX_INSERT_LOCATION}}

	DECLARE_MESSAGE_MAP()

// User Stuff Below

private:

	class CReportData
	{
	public:
		CString m_name;
		CString m_days;
		CString m_wedays;
	public:
		CReportData(CString n, CString d, CString w) {m_name=n; m_days=d; m_wedays=w;};
   	} *m_reportdata;

	int m_totaldays;
	CPtrList m_list;

protected:

public:
//	CReportDlg(CSchedulaDoc* pDoc);   // standard constructor
	CReportDlg();   // standard constructor
	virtual ~CReportDlg();

	void Reset();
	void AddLine(CString name, int days, int wedays);

};

/********************************************************************************/
// SchedulaDialogLib.cpp - End Of File
/********************************************************************************/
