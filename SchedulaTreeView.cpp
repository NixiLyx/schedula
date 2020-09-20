/********************************************************************************/
// SchedulaTreeView.cpp : Implementation of the CSchedulaTreeView class

#include "StdAfx.h"

#include "Schedula.h"

//#include "SchedulaTreeView.h"

/********************************************************************************/
// CSchedulaTreeView - Debuging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif //_DEBUG

#ifdef _DEBUG
void CSchedulaTreeView::AssertValid() const
{	
	CTreeView::AssertValid();
}
void CSchedulaTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
CSchedulaDoc* CSchedulaTreeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSchedulaDoc)));
	return (CSchedulaDoc*)m_pDocument;
}
#endif //_DEBUG

/********************************************************************************/
// CSchedulaTreeView - Class Initialisation

IMPLEMENT_DYNCREATE(CSchedulaTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CSchedulaTreeView, CTreeView)
	//{{AFX_MSG_MAP(CSchedulaTreeView)
	// System Message Handlers
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	// Program Message Handlers
	ON_COMMAND(ID_TVN_EDIT, OnTvnEdit)
	ON_COMMAND(ID_TVN_ADD, OnTvnAdd)
	ON_COMMAND(ID_TVN_DELETE, OnTvnDelete)
	ON_COMMAND(ID_TVN_APPEND, OnTvnAppend)
	ON_COMMAND(ID_TVN_SERIES, OnTvnSeries)
	ON_COMMAND(ID_TVN_SCHEDULE, OnTvnSchedule)
	ON_COMMAND(ID_TVN_REPORT, OnTvnReport)
	ON_COMMAND(ID_TVN_RESET, OnTvnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaTreeView - Class Construction

CSchedulaTreeView::CSchedulaTreeView()
{
}

/********************************************************************************/
// CSchedulaTreeView - Class Destruction

CSchedulaTreeView::~CSchedulaTreeView()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaTreeView - Virtual Function - PreCreateWindow()
//
// Description: Define Styles for this view
//

BOOL CSchedulaTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

/********************************************************************************/
// CSchedulaTreeView - Virtual Function - OnUpdate()
//
// Description: Populate Tree View with Entrys
//

void CSchedulaTreeView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	FOR_CURRENT_DOCUMENT(pDoc)

	// Check for Single View Update

	if(!pDoc->DoUpdate(TREEVIEW))
		return;

	FOR_CURRENT_TABLE(pTable)

	// Get Tree Control 

	CTreeCtrl& rTree = GetTreeCtrl();

	// Retrieve Current Selection

	HTREEITEM hSelected = rTree.GetSelectedItem();

	CEntry* pLastEntry = NULL;

//	if (hSelected!=NULL)
//		pLastEntry = (CEntry*) rTree.GetItemData(hSelected);
//	else
		pLastEntry = pTable->GetEntry();

	// Remove Previous Items
	
	SetRedraw(FALSE);

	rTree.DeleteAllItems();

	// Create Tree Root

	m_hRoot = rTree.InsertItem(pTable->GetText());
	rTree.SetItemData(m_hRoot, (DWORD) NULL);

	// Add Entrys to Tree View

	FOR_CURRENT_ENTRY_LIST(pEntryList)
	
	FOR_EACH_ENTRY(pEntry, pEntryList)
		HTREEITEM hEntry = rTree.InsertItem(pEntry->GetText(),m_hRoot,TVI_LAST);
		rTree.SetItemData(hEntry, (DWORD) pEntry);
		if (pEntry == pTable->GetEntry()) 
			rTree.SelectItem(hEntry);
	END_EACH

	// Expand Tree 

	rTree.Expand(m_hRoot,TVE_EXPAND);

	// Restore Previous Selection

	if (hSelected==NULL)
	{
		rTree.SelectItem(m_hRoot);
	}
	else
	{
		if (pLastEntry!=NULL)
		{
			HTREEITEM hSelection = m_hRoot ;

			while (hSelection!=NULL)
			{
				CEntry* pThisEntry = (CEntry*) rTree.GetItemData(hSelection);

				if (pThisEntry == pLastEntry)
				{
					rTree.SelectItem(hSelection);
					break;		
				}

				hSelection = rTree.GetNextItem(hSelection, TVGN_NEXTVISIBLE);
			}

			// Do nothing, selection made elsewhere ...
		}
		else
		{
			rTree.SelectItem(m_hRoot);
		}
	}

	SetRedraw(TRUE);
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaTreeView - System Message Handler - OnTvnSelchanged()
//
// Description: New Item Selected, Update Other Views
//

void CSchedulaTreeView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Get Tree Control & Retrieve Current Selection

	CTreeCtrl& rTree = GetTreeCtrl();

	HTREEITEM hSelected = rTree.GetSelectedItem();

	if (hSelected!=NULL)
	{
		// Retrieve & Select Saved Entry

		if (!pTable->FindEntry((CEntry*) rTree.GetItemData(hSelected))) 
		{
//TODO: Can't Handle Empty Tables
//			MessageBox("\tException : 0x001\n\n\"Unable to perform requested operation.\"\n");
		}

		// Update Views
		
		pDoc->UpdateAllViews(this);
	}
	else
	{
		pDoc->SelEntry(NULL);
	}

	*pResult = 0;
}

/********************************************************************************/
// CSchedulaTreeView - System Message Handler - OnLButtonDblClk()
//
// Description: Double Click - Edit Name

void CSchedulaTreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeView::OnLButtonDblClk(nFlags, point);

	UINT pFlags;

	// Get Tree Control & Retrieve Current Selection

	CTreeCtrl& rTree = GetTreeCtrl();

	HTREEITEM hSelected = rTree.HitTest(point, &pFlags);

	if (hSelected!=NULL)
	{
		rTree.SelectItem(hSelected);

		if (rTree.HitTest(point, &pFlags) == m_hRoot)	// Double Click on Root
		{
			// TODO: Rename ?
		}
		else											// Double Click on Branch
		{
			SendMessage(WM_COMMAND, ID_TVN_EDIT, NULL);
		}
	}
}

/********************************************************************************/
// CSchedulaTreeView - System Message Handler - OnRButtonDown()

void CSchedulaTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTreeView::OnRButtonDown(nFlags, point);

	UINT pFlags;

	// Get Tree Control & Retrieve Current Selection

	CTreeCtrl& rTree = GetTreeCtrl();

	HTREEITEM hSelected = rTree.HitTest(point, &pFlags);

	if (hSelected!=NULL)
	{
		rTree.SelectItem(hSelected);

		// Build Popup Menu

		CMenu menu;
		menu.CreatePopupMenu();

		if (rTree.HitTest(point, &pFlags) == m_hRoot)	// Menu on Root
		{
			menu.AppendMenu(MF_STRING | MF_ENABLED,ID_TVN_ADD,"Add Name");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING | MF_GRAYED,ID_TVN_SCHEDULE,"Schedule");
			menu.AppendMenu(MF_STRING | MF_GRAYED,ID_TVN_REPORT,"Show Report");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING | MF_GRAYED,ID_TVN_RESET,"Reset Schedule");
		}
		else											// Menu on Branch
		{
			menu.AppendMenu(MF_STRING | MF_ENABLED,ID_TVN_EDIT,"Edit Name");
			menu.AppendMenu(MF_STRING | MF_ENABLED,ID_TVN_DELETE,"Delete Name");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING | MF_ENABLED,ID_TVN_APPEND,"Add Date");
			menu.AppendMenu(MF_STRING | MF_ENABLED,ID_TVN_SERIES,"Add Series");
		}

		// Get Screen Coordinates from Tree View Local Coordinates

		ClientToScreen(&point);

		// Display Popup and Detach from View - Runs in Own Thread.
		
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
									point.x, point.y,
									AfxGetApp()->m_pMainWnd,NULL);
		menu.Detach();
	}
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnEdit()

void CSchedulaTreeView::OnTvnEdit()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Create and Call Input Dialogue

	CEntryDlg entryDlg(pDoc,UPDATE_NAME);

	entryDlg.DoModal();
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnAdd()

void CSchedulaTreeView::OnTvnAdd()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_ENTRY_LIST(pEntryList)

	if(pEntryList->GetCount()<MAXENTRYS)
	{
		// Create and Call Input Dialogue

		CEntryDlg entryDlg(pDoc, ADD_NAME);

		entryDlg.DoModal();
	}
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnDelete()

void CSchedulaTreeView::OnTvnDelete()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Get Tree

	CTreeCtrl& rTree = GetTreeCtrl();

	HTREEITEM hSelected = rTree.GetSelectedItem();

	if (hSelected!=NULL)
	{
		if (pTable->FindEntry((CEntry*) rTree.GetItemData(hSelected))) 
		{
			FOR_CURRENT_ENTRY(pEntry)

			pDoc->DeleteEntry(pEntry);

			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox("\tException : 0x001\n\n\"Unable to perform requested operation.\"\n");
		}
	}
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnAppend()

void CSchedulaTreeView::OnTvnAppend()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Create and Call Input Dialogue

	CEntryDlg entryDlg(pDoc,ADD_DATE);

	entryDlg.SetDateFrom(CDate(pTable->GetYear(),1,1));
	entryDlg.SetDateTo(CDate(pTable->GetYear(),1,1));
	entryDlg.SetType("");

	entryDlg.DoModal();
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnSeries()

void CSchedulaTreeView::OnTvnSeries()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Create and Call Input Dialogue

	CEntryDlg entryDlg(pDoc, ADD_SERIES);

	entryDlg.SetDateFrom(CDate(pTable->GetYear(),1,1));
	entryDlg.SetDateTo(CDate(pTable->GetYear(),1,1));
	entryDlg.SetType("");
	
	entryDlg.DoModal();
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnSchedule()

void CSchedulaTreeView::OnTvnSchedule()
{
	FOR_CURRENT_DOCUMENT(pDoc)

	pDoc->Schedule();

	pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnReset()

void CSchedulaTreeView::OnTvnReset()
{
	FOR_CURRENT_DOCUMENT(pDoc)

	pDoc->Reset();

	pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/
// CSchedulaTreeView - Program Message Handler - OnTvnReport()

void CSchedulaTreeView::OnTvnReport()
{
	FOR_CURRENT_DOCUMENT(pDoc)

	pDoc->UpdateView(PLANVIEW);

	SchApp.ReportDlg->DoModal();
}

/********************************************************************************/
// SchedulaTreeView.cpp - End Of File
/********************************************************************************/


