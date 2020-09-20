/********************************************************************************/
// SchedulaEngine.cpp : Implementation of the SchedulaEngine class

#include <stdAfx.h>
#include <afxtempl.h>

#include "Schedula.h"

/********************************************************************************/
// CEntryDlg - Class Initialisation

IMPLEMENT_DYNAMIC(CEngineDlg, CDialog)

BEGIN_MESSAGE_MAP(CEngineDlg, CDialog)
	//{{AFX_MSG_MAP(CEngineDlg)
	ON_BN_CLICKED(ID_ENGINE_SELECTALL, OnEngineSelectAll)
	ON_BN_CLICKED(ID_ENGINE_CLEAR, OnEngineClear)
	ON_BN_CLICKED(ID_ENGINE_RESET, OnEngineReset)
	ON_BN_CLICKED(ID_ENGINE_RUN, OnEngineRun)
	ON_BN_CLICKED(ID_ENGINE_DONE, OnEngineDone)
	ON_COMMAND_RANGE(IDC_ENGINE_RADIO1, IDC_ENGINE_RADIO8, OnEngineRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CEngineDlg - Class Construction

CEngineDlg::CEngineDlg(CSchedulaDoc* pDoc) : CDialog(CEngineDlg::IDD)
{
	CEngineDlg::pDoc=pDoc;

	m_method=1;
	m_engineList = new CPtrList;
	m_engineSelList = new CPtrList;

}

/********************************************************************************/
// CEngineDlg - Class Destruction

CEngineDlg::~CEngineDlg()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEngineDlg - Virtual Function - DoDataExchange()

void CEngineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineDlg)
	DDX_Control(pDX, IDC_ENGINE_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_ENGINE_LIST, m_list);
	DDX_Control(pDX, IDC_ENGINE_INFO, m_info);
	DDX_Control(pDX, IDC_ENGINE_RADIO1, m_radio);
	//}}AFX_DATA_MAP
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEntryDlg - System Message Handler - OnInitDialog()

BOOL CEngineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_radio.SetCheck(1);

	m_progress.SetRange(0,366);
	m_progress.SetStep(1);

	Message("Scheduling Engine Version 1.0");
	Message("Ready");

	int i;
	
	FOR_EACH_ENTRY(pEntry, m_engineList)

		i=m_list.InsertString(-1,pEntry->GetName());
		m_list.SetItemDataPtr(i,pEntry);

	END_EACH
	
	return TRUE;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineSelectAll()

void CEngineDlg::OnEngineSelectAll()
{
	int c=m_list.GetCount();
	m_list.SelItemRange(TRUE,0,c);
}

/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineClear()

void CEngineDlg::OnEngineClear()
{
	int c=m_list.GetCount();
	m_list.SelItemRange(FALSE,0,c);
}

/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineReset()

void CEngineDlg::OnEngineReset()
{

}

/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineRun()

void CEngineDlg::OnEngineRun()
{
	int i,c ;
	POSITION pos;

	Message("Loading Selections");

	c = m_list.GetSelItems(MAXENTRYS,m_selbuf);

	m_engineSelList->RemoveAll();

	for (i=0;i<c;i++)
	{
		pos=m_engineList->FindIndex(m_selbuf[i]);
		m_engineSelList->AddTail(m_engineList->GetAt(pos));
	}

	FOR_EACH_ENTRY(pEntry, m_engineSelList)
		Message(">"+pEntry->GetName());
	END_EACH

	Message("Loading Dates");

	if (m_method==1) Message("Method - Sequential");
	if (m_method==2) Message("Method - Balanced");
	if (m_method==3) Message("Method - Week at Once");
	if (m_method==4) Message("Method - Week Ends");
	if (m_method==5) Message("Method - Unused");
	if (m_method==6) Message("Method - Unused");
	if (m_method==7) Message("Method - Unused");
	if (m_method==8) Message("Method - Unused");

	Message("Analysing ...");

	Message("Scheduling ...");

	if (m_method==1) Process1();
}

/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineDone()

void CEngineDlg::OnEngineDone()
{
	OnOK();
}

/********************************************************************************/
// CEngineDlg - Program Message Handler - OnEngineRadio()

void CEngineDlg::OnEngineRadio(UINT	nID)
{
	m_method=nID-IDC_ENGINE_RADIO1+1;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CEngineDlg - Class Function - AddEntry()

void CEngineDlg::Message(CString msg)
{
	m_info.InsertString(-1,msg);
	m_info.SetCurSel(m_info.GetCount()-1);
}

/********************************************************************************/
// CEngineDlg - Class Function - AddEntry()

void CEngineDlg::AddEntry(CEntry* pEntry)
{
	m_engineList->AddTail (pEntry);
}

/********************************************************************************/
// CEngineDlg - Class Function - Process1()

void CEngineDlg::Process1()
{
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_ENTRY_LIST(pEntryList)
	FOR_CURRENT_DATE(pDate)

	if (pEntryList->GetCount()==0)
		return;		//TODO: Message ?

	int i, w, day[367], days = pDate->DaysinTotal(12,31);

	CDataSort* DataSort = new CDataSort(days);

	DataSort->Reset();

	FOR_EACH_ENTRY(pEntry, pEntryList)

		// Create Data Header

		CDataHead* pHead = DataSort->AddHead(pEntry);

		// Zero Day Array

		for(i=1;i<=days;i++) day[i]=0;

		FOR_EACH_ITEM(pItem, pEntry->GetItemList())

			// Remove Previously Scheduled Days

			if(pItem->GetType()=="s")
			{
				pEntry->DelItem(pItem);
				continue;
			}

			if(pItem->GetType()=="S")
			{
				//TODO: Do Somthing to Obey Manually Scheduled Days
			}

			// Calculate Weight 
		
			if(pItem->GetTypeNum()==TYPE_V) w=+2;
			if(pItem->GetTypeNum()==TYPE_D) w=+1;
			if(pItem->GetTypeNum()==TYPE_U) w=-1;
			if(pItem->GetTypeNum()==TYPE_S) w=-2;

			// Create Data Entry for Each Day in Date Range
			
			for(i=pItem->GetDateFrom()->GetNumDays();i<=pItem->GetDateTo()->GetNumDays();i++)
			{
				DataSort->AddData(pHead, pItem, i, w);
				day[i]=w;
			}

		END_EACH

		// Add NULL Item for Unweighted Days

		for(i=1;i<=days;i++)
		{
			m_progress.StepIt();

			if(day[i]==0)
			{
				DataSort->AddData(pHead, NULL, i, 0);
			}
		}
	
	END_EACH

	DataSort->Schedule(pDoc);

	//UpdateAllViews(NULL);

	DataSort->Report(pDoc);

	delete DataSort;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CDataHead - Class Construction

CDataHead::CDataHead(CEntry* pEntry)
{
	CDataHead::pEntry=pEntry;
	CDataHead::items=0;
	CDataHead::dayweight=0;
	CDataHead::wkdweight=0;
	order=0;
}

/********************************************************************************/
// CDataHead - Class Destruction

CDataHead::~CDataHead()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CDataBody - Class Construction

CDataBody::CDataBody(CDataHead* pHead, CItem* pItem, int weight)
{
	CDataBody::pHead=pHead;
	CDataBody::pItem=pItem;
	CDataBody::weight=weight;
}

/********************************************************************************/
// CDataBody - Class Destruction

CDataBody::~CDataBody()
{
}

/********************************************************************************/


					//----------Section Break----------//



/********************************************************************************/
// CDataSort - Class Construction

CDataSort::CDataSort(int days)
{
	CDataSort::days=days;
}

/********************************************************************************/
// CDataSort - Class Destruction

CDataSort::~CDataSort()
{
	POSITION pos = Headers.GetHeadPosition();

	while(pos)
	{
		CDataHead* pHead = (CDataHead*) Headers.GetNext(pos);
		delete pHead;
	}

	Headers.RemoveAll();

	for(int i=1;i<=days;i++)
	{
		POSITION pos = Data[i].GetHeadPosition();

		while(pos)
		{
			CDataBody* pBody = (CDataBody*) Data[i].GetNext(pos);
			delete pBody;
		}
		if(pos!=NULL)
			Data[i].RemoveAll();
	}
}

/********************************************************************************/
// CDataSort - Class Function - Reset()

void CDataSort::Reset()
{
	for(int i=1;i<=days;i++)
	{
		POSITION pos = Data[i].GetHeadPosition();

		while(pos)
		{
			CDataBody* pBody = (CDataBody*) Data[i].GetNext(pos);
			pBody->pHead->items=0;	// Never Used !!!
			delete pBody;
		}
		if(pos!=NULL)
			Data[i].RemoveAll();
	}
}

/********************************************************************************/
// CDataSort - Class Function - AddHead()

CDataHead* CDataSort::AddHead(CEntry* pEntry)
{
	POSITION pos = Headers.GetHeadPosition();

	while(pos)
	{
		CDataHead* pDataHead = (CDataHead*) Headers.GetNext(pos);

		if(pDataHead->pEntry==pEntry)
			return pDataHead;
	}

	CDataHead* pDataHead = new CDataHead(pEntry);

	Headers.AddTail(pDataHead);

	return pDataHead;
}

/********************************************************************************/
// CDataSort - Class Function - AddData()

BOOL CDataSort::AddData(CDataHead* pHead, CItem* pItem, int day, int weight)
{
	if(day > days)
	{
		ASSERT(0);
		return FALSE;
	}

	if(!Headers.Find(pHead))
	{
		ASSERT(0);
		return FALSE;
	}

	CDataBody* pBody = new CDataBody(pHead, pItem, weight);

	Data[day].AddTail(pBody);

	if(weight!=0)pHead->items++;	// Never Used !!!

	return TRUE;
}

/********************************************************************************/
// CDataSort - Class Function - SiftData()

void CDataSort::Schedule(CSchedulaDoc* pDoc)
{ 
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	// For Each Day in Year ...

	for(int i=1;i<=days;i++)
	{
		// Find Best Matching Item

		CDataBody* Ref = Sieve(&Data[i]);

		if (Ref==NULL) 
			continue; // No Match Found, Advance to Next Day

		// Create Start Date 

		CDate* pCallDate1 = new CDate(pDate->GetYear(), pDate->MonthfromDays(i), pDate->DayfromDays(i));

		// Check if Date is Week Day

		weekday = pDate->IsDay(i, CD_MON | CD_TUE | CD_WED | CD_THU);

		if(weekday)
		{
			Ref->pHead->order=i;			// Save Day (Last Day Used) in Header
			Ref->pHead->dayweight++;		// Increment Number of Days Scheduled

			// Schedule Call Day

			if (pDoc->SelEntry(Ref->pHead->pEntry))
				pDoc->AddDate("s", *pCallDate1, *pCallDate1);
		}
		else
		{
			// Account for FRI-SAT-SUN Rule
			//TODO: Fails to account for SAT & SUN possibly being a vacation !

			while( i<days && pDate->IsDay(i, CD_FRI | CD_SAT /*| CD_SUN*/) ) i++;

			Ref->pHead->order=i;			// Save Day (Last Day Used) in Header
			Ref->pHead->wkdweight++;		// Increment Number of Long Weekends Scheduled

			// Create End Date

			CDate* pCallDate2 = new CDate(pDate->GetYear(), pDate->MonthfromDays(i), pDate->DayfromDays(i));

			// Schedule Call Weekend
		
			if (pDoc->SelEntry(Ref->pHead->pEntry))
				pDoc->AddDate("s", *pCallDate1, *pCallDate2);

			delete pCallDate2;
		}

		delete pCallDate1;
	}
}

/********************************************************************************/
// CDataSort - Class Function - Sieve()

CDataBody* CDataSort::Sieve(CPtrList* pData)
{
	POSITION pos = pData->GetHeadPosition();

	if(pos==NULL) return NULL;

	int tweak=0, maxtweak=(int)pData->GetCount();

	// Sort Item Data By Header Into Order of Recent Use (Most Recently Used Header Last)

	Bubble(pData);

	// Get Reference Item (First on List)

	CDataBody* Temp = NULL, * Item, * Ref;

	// Search for Better Item than Reference Item

	while(TRUE)
	{
		// Go to Top of List

		POSITION pos = pData->GetHeadPosition();

		Ref = (CDataBody*) pData->GetNext(pos);

		while(pos)
		{
			// Get List Item to Compare to Reference Item

			Item = (CDataBody*) pData->GetNext(pos);

			// Check if Manually Scheduled

			if(Item->weight<-1) // Type S (-2)
			{
				return Item;
			}

			// Check if Day is Desirable
/*
			if(Item->weight<0) // Type D (-2)
			{
				return Item;
			}
*/
			// Compare List Item with Reference Item for Better Match
	
			if(SieveAlg(Item, Ref, tweak))
			{
				Ref = Item;			// List Item Becomes New Reference Item
			}
		}

		// If Final Reference Item is Vacation Day, Offset Total Weight with Tweak and Try Again

		if(Ref->weight>+1) // Type V (+2)
		{
			tweak++;
			if(tweak<maxtweak)
				continue;
			else
				return NULL;
		}
/*
		if(Ref->weight>0) // Type U (+1)
		{
			if (Temp!=NULL)
				Temp = Ref;
			tweak++;
			if(tweak<maxtweak)
				continue;
			else
				return Ref;
		}
*/
		break;
	}

	return Ref;
}

/********************************************************************************/
// CDataSort - Class Function - SieveAlg()

BOOL CDataSort::SieveAlg(CDataBody* pItemData, CDataBody* pRefData, int tweak)
{
	int TotItemWeight = (pItemData->pHead->wkdweight*3) + pItemData->pHead->dayweight;
	int TotRefWeight = (pRefData->pHead->wkdweight*3) + pRefData->pHead->dayweight;

	if(pItemData->pHead->order-tweak<pRefData->pHead->order)
	{
		return TRUE;
	}

	if(pItemData->pHead->order-tweak==pRefData->pHead->order)
	{
		if(TotItemWeight + pItemData->weight < TotRefWeight + pRefData->weight)
			return TRUE;
		else
			return FALSE;

		if(weekday)
		{
			if(pItemData->pHead->dayweight < pRefData->pHead->dayweight)
				return TRUE;
		}
		else
		{
			if(pItemData->pHead->wkdweight < pRefData->pHead->wkdweight)
				return TRUE;
		}
	}

	return FALSE;
//***


	if(TotItemWeight /*+ pItemData->weight*/ - tweak < TotRefWeight /*+ pRefData->weight*/)
		return TRUE;
	else
		return FALSE;
/*
	if(weekday)
	{
		if(pItemData->pHead->dayweight+pItemData->weight-tweak < pRefData->pHead->dayweight+pRefData->weight)
			return TRUE;
	}
	else
	{
		if(pItemData->pHead->wkdweight+pItemData->pHead->dayweight+pItemData->weight-tweak < pRefData->pHead->wkdweight+pRefData->pHead->dayweight+pRefData->weight)
			return TRUE;
	}
	return FALSE;
*/
}

/********************************************************************************/
// CDataSort - Class Function - Bubble()

void CDataSort::Bubble(CPtrList* pData)
{
	int flag=TRUE;

	while(flag)
	{
		flag=FALSE;

		POSITION pos = pData->GetHeadPosition();

		while(pos)
		{
			CDataBody* HighData = (CDataBody*) pData->GetNext(pos);	// Get Item 1
			if(pos==NULL)break;
			CDataBody* LowData = (CDataBody*) pData->GetAt(pos);		// Get Item 2

			if(BubbleAlg(HighData, LowData))		// Compare
			{
				pData->SetAt(pos, HighData);							// Item 2 = Item 1
				pData->GetPrev(pos);
				pData->SetAt(pos, LowData);							// Item 1 = Item 2
				pData->GetNext(pos);
				flag=TRUE;
			}
		}
	}

// !!! Debug Code to verify sort is working !!!

	POSITION pos = pData->GetHeadPosition();
	int d[15], i=0;
	CString dn[15];
	while(pos)
	{
		CDataBody* Data = (CDataBody*) pData->GetNext(pos);
		dn[i]=Data->pHead->pEntry->GetName();
		d[i++]=Data->pHead->order;
	}

}

/********************************************************************************/
// CDataSort - Class Function - BubbleAlg()

BOOL CDataSort::BubbleAlg(CDataBody* pHighData, CDataBody* pLowData)
{
	if (pHighData->pHead->order > pLowData->pHead->order)
		return TRUE;

	return FALSE;
}

/********************************************************************************/
// CDataSort - Class Function - Report()

void CDataSort::Report(CSchedulaDoc* pDoc)
{ 
	SchApp.ReportDlg->Reset();

	CDataHead* pHeadData;

	POSITION pos = Headers.GetHeadPosition();

	while(pos)
	{
		pHeadData = (CDataHead*) Headers.GetNext(pos);

		SchApp.ReportDlg->AddLine(pHeadData->pEntry->GetName(), pHeadData->dayweight, pHeadData->wkdweight);
	}

	if(SchApp.IsShowReport())
		SchApp.ReportDlg->DoModal();
}

/********************************************************************************/
// SchedulaEngine.cpp - End Of File
/********************************************************************************/
