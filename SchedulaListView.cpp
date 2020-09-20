/********************************************************************************/
// SchedulaListView.cpp : Implementation of the SchedulaListView class

#include "StdAfx.h"

#include "Schedula.h"

/********************************************************************************/
// CSchedulaListView - Debuging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif //_DEBUG

#ifdef _DEBUG
void CSchedulaListView::AssertValid() const
{
	CListView::AssertValid();
}
void CSchedulaListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
CSchedulaDoc* CSchedulaListView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSchedulaDoc)));
	return (CSchedulaDoc*)m_pDocument;
}
#endif //_DEBUG


/********************************************************************************/
// CSchedulaListView - Class Initialisation

IMPLEMENT_DYNCREATE(CSchedulaListView, CListView)

BEGIN_MESSAGE_MAP(CSchedulaListView, CListView)
	//{{AFX_MSG_MAP(CSchedulaListView)
	// System Message Handlers
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, OnLvnItemActivate)
	// Program Message Handlers
	ON_COMMAND(ID_LVN_EDIT, OnLvnEdit)
	ON_COMMAND(ID_LVN_DELETE, OnLvnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaListView - Class Construction

CSchedulaListView::CSchedulaListView()
{
}

/********************************************************************************/
// CSchedulaListView - Class Destruction

CSchedulaListView::~CSchedulaListView()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaListView - Virtual Function - PreCreateWindow()
//
// Description: Define Styles for this view
//

BOOL CSchedulaListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS ;

	return CListView::PreCreateWindow(cs);
}

/********************************************************************************/
// CSchedulaListView - Virtual Function - OnInitialUpdate()
//
// Description: Populate rList View with Items
//

void CSchedulaListView::OnInitialUpdate()
{
	// Initialise List View, One Time Only

	static int stat=0;
	if (!stat++)
	{
		// Get List Control

		CListCtrl& rList = GetListCtrl();

		// Create Column Headers

		rList.InsertColumn(0,"",LVCFMT_LEFT,20);
		rList.InsertColumn(1,"From",LVCFMT_LEFT,70);
		rList.InsertColumn(2,"To",LVCFMT_LEFT,70);
		rList.InsertColumn(3,"Days",LVCFMT_LEFT,40);

		// Define List Styles
		
		ListView_SetExtendedListViewStyleEx(rList.GetSafeHwnd(), LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	}

	// Pass Control to MFC

	CListView::OnInitialUpdate();
}

/********************************************************************************/
// CSchedulaListView - Virtual Function - OnUpdate()

void CSchedulaListView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	FOR_CURRENT_DOCUMENT(pDoc)

	// Check for Single View Update

	if(!pDoc->DoUpdate(LISTVIEW))
		return;

	FOR_CURRENT_TABLE(pTable)

	// Get List Control 

	CListCtrl& rList = GetListCtrl();

	// Remove Previous Items

	SetRedraw(FALSE);

	rList.DeleteAllItems();

	// Attach Icons to List

	HIMAGELIST hImgLst = ImageList_Create(12, 12, ILC_COLOR24, 4, 4);
	ImageList_AddIcon( hImgLst, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_V)) );
	ImageList_AddIcon( hImgLst, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_U)) );
	ImageList_AddIcon( hImgLst, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_D)) );
	ImageList_AddIcon( hImgLst, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_S)) );
	ListView_SetImageList(rList.m_hWnd, hImgLst, LVSIL_SMALL);

	FOR_CURRENT_ENTRY(pEntry)

	if (pEntry!=NULL)
	{
		// Add Items to List View

		FOR_CURRENT_ITEM_LIST(pItemList)

		int i=0;
		FOR_EACH_ITEM(pItem, pItemList)
			rList.InsertItem(i,"",pItem->GetTypeNum());
			rList.SetItemText(i,1,pItem->fromStr()->GetBuffer(10));
			rList.SetItemText(i,2,pItem->toStr()->GetBuffer(10));
			rList.SetItemText(i,3,pItem->daysStr());
			rList.SetItemData(i, (DWORD) pItem);
			i++;
		END_EACH
	}

	SetRedraw(TRUE);
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaListView - System Message Handler - OnLvnItemchanged()

void CSchedulaListView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
 {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	FOR_CURRENT_DOCUMENT(pDoc)

	FOR_CURRENT_TABLE(pTable)

	FOR_CURRENT_ENTRY(pEntry)

	// Get List Control

	CListCtrl& rList = GetListCtrl();

	if (pNMLV->uNewState==3)
	{
		// Get Item Associated with Selection and Check it Exists

		if (!pEntry->FindItem( (CItem*) rList.GetItemData(pNMLV->iItem) ))
		{
			MessageBox("\tException : 0x001\n\n\"Unable to perform requested operation.\"\n");
		}

		pDoc->UpdateView(PLANVIEW);
	}

	*pResult = 0;
}

/********************************************************************************/
// CSchedulaListView - System Message Handler - OnLvnItemActivate()

void CSchedulaListView::OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	SendMessage(WM_COMMAND, ID_LVN_EDIT, NULL);

	*pResult = 0;
}

/********************************************************************************/
// CSchedulaListView - System Message Handler - OnRButtonDown()

void CSchedulaListView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListView::OnRButtonDown(nFlags, point);

	UINT pFlags;

	// Get List Control & Retrieve Current Selection

	CListCtrl& rList = GetListCtrl();

	int hSelected = rList.HitTest(point, &pFlags);

	if (hSelected!=-1)
	{
		// Build Popup Menu

		CMenu menu;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_STRING | MF_ENABLED,ID_LVN_EDIT,"Edit Date");
		menu.AppendMenu(MF_STRING | MF_ENABLED,ID_LVN_DELETE,"Delete Date");

		// Get Screen Coordinates from List View Local Coordinates

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
// CSchedulaListView - Program Message Handler - OnLvnEdit()

void CSchedulaListView::OnLvnEdit()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_ENTRY(pEntry)

	// Get List Control & Retrieve Current Selection
	
	CListCtrl& rList = GetListCtrl();

	int hSelected = rList.GetNextItem(-1,LVIS_SELECTED);

	if (hSelected!=-1)
	{
		// Get Item Associated with Selection and Check it Exists

		if (pEntry->FindItem((CItem*) rList.GetItemData(hSelected)))
		{
			// Create and Call Input Dialogue

			CEntryDlg entryDlg(pDoc, UPDATE_DATE);

			entryDlg.DoModal();
		}
		else
		{
			MessageBox("\tException : 0x001\n\n\"Unable to perform requested operation.\"\n");
		}
	}
}

/********************************************************************************/
// CSchedulaListView - Program Message Handler - OnLvnDelete()

void CSchedulaListView::OnLvnDelete()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_ENTRY(pEntry)

	// Get List Control & Retrieve Current Selection
	
	CListCtrl& rList = GetListCtrl();

	int hSelected = rList.GetNextItem(-1,LVIS_SELECTED);

	if (hSelected!=-1)
	{
		// Get Item Associated with Selection and Check it Exists

		if (pEntry->FindItem((CItem*) rList.GetItemData(hSelected))) 
		{
			FOR_CURRENT_ITEM(pItem)

			pDoc->DeleteDate(pItem);

			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox("\tException : 0x001\n\n\"Unable to perform requested operation.\"\n");
		}
	}
}

/********************************************************************************/
// SchedulaListView.cpp - End Of File
/********************************************************************************/

