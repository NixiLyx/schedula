/********************************************************************************/
// SchedulaDialogLib.cpp : Implementation of the SchedulaDialogLib classes

#include "StdAfx.h"

#include "Schedula.h"

//#include ".\SchLib\SchLib.h"

/********************************************************************************/
// CAboutDlg - Class Initialisation

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CAboutDlg - Class Construction

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CAboutDlg - Virtual Function - DoDataExchange()

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEntryDlg - Class Initialisation

IMPLEMENT_DYNAMIC(CEntryDlg, CDialog)

BEGIN_MESSAGE_MAP(CEntryDlg, CDialog)
	//{{AFX_MSG_MAP(CEntryDlg)
	ON_BN_CLICKED(ID_ENTRY_ADD, OnEntryAdd)
	ON_BN_CLICKED(ID_ENTRY_DONE, OnEntryDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CEntryDlg - Class Construction

CEntryDlg::CEntryDlg(CSchedulaDoc* pDoc, int status) : CDialog(CEntryDlg::IDD)
{
	FOR_CURRENT_TABLE(pTable)

	m_name="";
	m_from=CDate(pTable->GetYear(),1,1);
	m_to=CDate(pTable->GetYear(),1,1);
	m_type="";

	FOR_CURRENT_ENTRY(pEntry)

	if (pEntry!=NULL)
	{
		m_name=pEntry->GetName();

		FOR_CURRENT_ITEM(pItem)

		if (pItem!=NULL)
		{
			m_from=*pItem->GetDateFrom();
			m_to=*pItem->GetDateTo();
			m_type=pItem->GetType();
		}
	}

	CEntryDlg::pDoc=pDoc;
	m_status=status;
}

/********************************************************************************/
// CEntryDlg - Class Destruction

CEntryDlg::~CEntryDlg()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEntryDlg - Virtual Function - DoDataExchange()

void CEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CEntryDlg)
	DDX_Control(pDX, IDC_ENTRY_NAME, name);
	DDX_Control(pDX, IDC_ENTRY_FROMDATE, from);
	DDX_Control(pDX, IDC_ENTRY_TODATE, to);
	DDX_Control(pDX, IDC_ENTRY_TYPE, type);
	//}}AFX_DATA_MAP
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEntryDlg - System Message Handler - OnInitDialog()

BOOL CEntryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	from.SetFormat("MM/dd/yyyy");
	to.SetFormat("MM/dd/yyyy");

	type.AddString("V");
	type.AddString("D");
	type.AddString("U");
	if(m_status!=4)
		type.AddString("S");
	type.SelectString(0,"V");

	if (m_status==0) // Add Name
	{
		name.SetWindowText("");
		type.SetCurSel(0);
	}
	else // Get Input Field Data
	{
		name.SetWindowText(m_name);
		from.SetTime(&CTime(m_from.GetYear(), m_from.GetMonth(), m_from.GetDay(), 0, 0, 0, 0));
		to.SetTime(&CTime(m_to.GetYear(), m_to.GetMonth(), m_to.GetDay(), 0, 0, 0, 0));
		type.SelectString(0,m_type);
	}

	if (m_status==1) // Add Date
	{
		CWnd* hWnd = CWnd::GetDlgItem(IDC_ENTRY_NAME);
		hWnd->EnableWindow(FALSE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_FROMDATE);
		hWnd->EnableWindow(TRUE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TODATE);
		hWnd->EnableWindow(TRUE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TYPE);
		hWnd->EnableWindow(TRUE);

		hWnd = CWnd::GetDlgItem(ID_ENTRY_ADD);
		hWnd->SetWindowText("Add Date");
	}
	if (m_status==2) // Edit Name
	{
		CWnd* hWnd = CWnd::GetDlgItem(ID_ENTRY_ADD);
		hWnd->SetWindowText("Update");
	}
	if (m_status==3) // Edit Date
	{
		CWnd* hWnd = CWnd::GetDlgItem(IDC_ENTRY_NAME);
		hWnd->EnableWindow(FALSE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_FROMDATE);
		hWnd->EnableWindow(TRUE);
		hWnd->SetFocus();
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TODATE);
		hWnd->EnableWindow(TRUE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TYPE);
		hWnd->EnableWindow(TRUE);

		hWnd = CWnd::GetDlgItem(ID_ENTRY_ADD);
		hWnd->SetWindowText("Update");
	}
	if (m_status==4) // Add Series
	{
		CWnd* hWnd = CWnd::GetDlgItem(IDC_ENTRY_NAME);
		hWnd->EnableWindow(FALSE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_FROMDATE);
		hWnd->EnableWindow(TRUE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TODATE);
		hWnd->EnableWindow(TRUE);
		hWnd = CWnd::GetDlgItem(IDC_ENTRY_TYPE);
		hWnd->EnableWindow(TRUE);

		hWnd = CWnd::GetDlgItem(ID_ENTRY_ADD);
		hWnd->SetWindowText("Add");

		// Set Default Values

		from.SetTime(&CTime(m_from.GetYear(),1, 1, 0, 0, 0, 0));
		to.SetTime(&CTime(m_to.GetYear(), 1, 1, 0, 0, 0, 0));
		type.SetCurSel(0);
	}


	return TRUE;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEntryDlg - Program Message Handler - OnEntryAdd()

void CEntryDlg::OnEntryAdd()
{
	CString str;
	CTime fd, td;
	CWnd* hWnd;

	name.GetWindowText(str);

	if (str.GetLength()>0)
	{
		switch (m_status)
		{
			case 0: // Add Name
			pDoc->AddEntry(str);
			pDoc->UpdateView(ALLVIEWS);

			// Switch to Add Date Mode

			m_status=1;

			hWnd = CWnd::GetDlgItem(IDC_ENTRY_NAME);
			hWnd->EnableWindow(FALSE);
			hWnd = CWnd::GetDlgItem(IDC_ENTRY_FROMDATE);
			hWnd->EnableWindow(TRUE);
			hWnd = CWnd::GetDlgItem(IDC_ENTRY_TODATE);
			hWnd->EnableWindow(TRUE);
			hWnd = CWnd::GetDlgItem(IDC_ENTRY_TYPE);
			hWnd->EnableWindow(TRUE);

			hWnd = CWnd::GetDlgItem(ID_ENTRY_ADD);
			hWnd->SetWindowText("Add Date");

			// Set Default Values

			from.SetTime(&CTime(m_from.GetYear(),1, 1, 0, 0, 0, 0));
			to.SetTime(&CTime(m_to.GetYear(), 1, 1, 0, 0, 0, 0));
			type.SetCurSel(0);

			pDoc->UpdateView(TREEVIEW);
			break;

			case 1: // Add Date
			from.GetTime(fd);
			to.GetTime(td);
			type.GetLBText(type.GetCurSel(),str);

			if (fd > td) {
				MessageBox("Start Date must be less than or equal to End Date.");
				break;
			}

			pDoc->AddDate(str, CDate(fd.GetYear(), fd.GetMonth(), fd.GetDay()), CDate(td.GetYear(), td.GetMonth(), td.GetDay()));

			pDoc->UpdateView(ALLVIEWS);
			break;

			case 2: // Update Name
			pDoc->UpdateEntry(str);

			pDoc->UpdateView(TREEVIEW);
			break;

			case 3: // Update Date
			from.GetTime(fd);
			to.GetTime(td);
			type.GetLBText(type.GetCurSel(),str);

			if (fd > td) {
				MessageBox("Start Date must be less than or equal to End Date.");
				break;
			}
			pDoc->UpdateDate(str, CDate(fd.GetYear(), fd.GetMonth(), fd.GetDay()), CDate(td.GetYear(), td.GetMonth(), td.GetDay()));

			pDoc->UpdateView(ALLVIEWS);
			break;

			case 4: // Add Series
			from.GetTime(fd);
			to.GetTime(td);
			type.GetLBText(type.GetCurSel(),str);

			if (fd > td) {
				MessageBox("Start Date must be less than or equal to End Date.");
				break;
			}

			pDoc->AddSeries(str, CDate(fd.GetYear(), fd.GetMonth(), fd.GetDay()), CDate(td.GetYear(), td.GetMonth(), td.GetDay()));

			pDoc->UpdateView(ALLVIEWS);
			break;
		}
	}
	else
	{
			FlashWindow(TRUE);
			MessageBeep(-1);
	}
}

/********************************************************************************/
// CEntryDlg - Program Message Handler - OnEntryDone()

void CEntryDlg::OnEntryDone()
{
	OnOK();
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - Class Initialisation

IMPLEMENT_DYNAMIC(CReportDlg, CDialog)

BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	//{{AFX_MSG_MAP(CReportDlg)
	ON_BN_CLICKED(IDC_REPORT_CLOSE, OnReportClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CReportDlg - Class Construction

CReportDlg::CReportDlg() : CDialog(CReportDlg::IDD)
{
	m_totaldays=0;
}

/********************************************************************************/
// CReportDlg - Class Destruction

CReportDlg::~CReportDlg()
{
	POSITION pos = m_list.GetHeadPosition();

	while(pos)
	{
		CReportData* pReportData = (CReportData*) m_list.GetNext(pos);
		delete pReportData;
	}
	m_list.RemoveAll();
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - Virtual Function - PreCreateWindow()

BOOL CReportDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	return CDialog::PreCreateWindow(cs);
}

/********************************************************************************/
// CReportDlg - Virtual Function - DoDataExchange()

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDlg)
	DDX_Control(pDX, IDC_REPORT_LIST, list);
	//}}AFX_DATA_MAP
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - System Message Handler - OnInitDialog()

BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//SetRedraw(FALSE);

	DWORD dwStyle = GetWindowLong(list.GetSafeHwnd(), GWL_STYLE);
	dwStyle&=-LVS_TYPEMASK;
	dwStyle|=LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS ;
	SetWindowLong(list.GetSafeHwnd(), GWL_STYLE, dwStyle);

	list.InsertColumn(0,"Name",LVCFMT_LEFT,237);
	list.InsertColumn(1,"Days",LVCFMT_CENTER,100);
	list.InsertColumn(2,"Weekends",LVCFMT_CENTER,100);
	list.DeleteAllItems();

	ListView_SetExtendedListViewStyleEx(list.GetSafeHwnd(), LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	POSITION pos = m_list.GetHeadPosition();

	int i=0;
	while(pos)
	{
		CReportData* pReportData = (CReportData*) m_list.GetNext(pos);

		list.InsertItem(i, pReportData->m_name);
		list.SetItemText(i, 1, pReportData->m_days);
		list.SetItemText(i, 2, pReportData->m_wedays);
		i++;
	}

	CString str;
	sprintf(str.GetBuffer(4), "%3d", m_totaldays);
/*
//TODO:Not relevant	with days rather than total days
	list.InsertItem(i++, " ");
	list.InsertItem(i, "Grand Total");
	list.SetItemText(i, 1, str);
	list.SetItemText(i, 2, " ");
*/
	//SetRedraw(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - Program Message Handler - OnReportClose()

void CReportDlg::OnReportClose()
{
	OnOK();
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CReportDlg - Class Function - Reset()

void CReportDlg::Reset()
{
	POSITION pos = m_list.GetHeadPosition();

	while(pos)
	{
		CReportData* pReportData = (CReportData*) m_list.GetNext(pos);
		delete pReportData;
	}
	m_list.RemoveAll();

	m_totaldays=0;
}

/********************************************************************************/
// CReportDlg - Class Function - AddLine()

void CReportDlg::AddLine(CString name, int days, int wedays)
{
	CString daysstr, wedaysstr;

	sprintf(daysstr.GetBuffer(4), "%3d", days);
	sprintf(wedaysstr.GetBuffer(4), "%3d", wedays);

	m_reportdata = new CReportData(name, daysstr, wedaysstr);

	m_list.AddTail(m_reportdata);

	m_totaldays+=days;
}

/********************************************************************************/
// SchedulaDialogLib.cpp - End Of File
/********************************************************************************/
