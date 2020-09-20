/********************************************************************************/
// SchedulaDoc.cpp : Implementation of the SchedulaDoc class

#include "StdAfx.h"

#include "Schedula.h"

//#include "SchedulaDoc.h"

/********************************************************************************/
// SchedulaDoc - Diagnostics

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif //_DEBUG

#ifdef _DEBUG
void CSchedulaDoc::AssertValid() const
{
	CDocument::AssertValid();
}
void CSchedulaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/********************************************************************************/
// SchedulaDoc - Class Initialisation

IMPLEMENT_DYNCREATE(CSchedulaDoc, CDocument)

BEGIN_MESSAGE_MAP(CSchedulaDoc, CDocument)
	//{{AFX_MSG_MAP(CSchedulaDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaDoc - Class Construction

CSchedulaDoc::CSchedulaDoc()
{
	// Create Table - Container for main program data
	
	pTable = new CTable;

}

/********************************************************************************/
// CSchedulaDoc - Class Destruction

CSchedulaDoc::~CSchedulaDoc()
{
	delete pTable;
}

/********************************************************************************/


					//----------Section Break----------//

/********************************************************************************/
// CSchedulaDoc - Virtual Function - OnNewDocument()

BOOL CSchedulaDoc::OnNewDocument()
{
	// Indirect Call to DeleteContents()

	if (!CDocument::OnNewDocument())
		return FALSE;

	// Make All Views Active

	SetActiveView(ALLVIEWS);

	return TRUE;
}

/********************************************************************************/
// CSchedulaDoc - Virtual Function - DeleteContents()

void CSchedulaDoc::DeleteContents()
{
	CDocument::DeleteContents();

//TODO: Crap, cause pTable is created above and then distroyed here straight away !

	delete pTable;

	pTable = new CTable;

	// Set Default Operational States

	SetScheduled(FALSE);
	SetSwapSource(NULL, NULL);
	SetSwapDestination(NULL, NULL);
}

/********************************************************************************/
// CSchedulaDoc - Virtual Function - Serialize()

void CSchedulaDoc::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())						// Write Data to File
	{
		ar << CString("1.1.0.0");						// File Version

		ar << 1;										// Table Count

		ar << pTable->GetName();						// Table Name

		ar << pTable->GetYear();						// Table Year

		CEntryList* pEntryList = pTable->GetEntryList();

		ar << (int) pEntryList->GetCount();				// Entry List Length

		FOR_EACH_ENTRY(pEntry, pEntryList)

			ar << pEntry->GetName();					// Entry Name

			CItemList* pItemList = pEntry->GetItemList();

			ar << (int) pItemList->GetCount();			// Item List Length

			FOR_EACH_ITEM(pItem, pItemList)

				ar << pItem->GetType();				// Type
				pItem->GetDateFrom()->Serialize(ar);	// From Date
				pItem->GetDateTo()->Serialize(ar);		// To Date

			END_EACH

		END_EACH

	}
	else									// Read Data from File
	{
		CString ver, str;
		CDate fd, td;
		int e, i, ec, ic, year;

		ar >> ver;								// File Version

		if (ver=="1.1.0.0")						// Version 3 Data
		{
			int tc;

			ar >> tc;									// Table Count
			ar >> str;									// Table Name
			ar >> year;									// Table Year
			pTable->SetYear(year);	
			ar >> ec;									// Read Number of Entrys

			for (e=0;e<ec;e++)
			{
				ar >> str;								// Read Entry Name
				AddEntry(str);
				ar >> ic;								// Read Number of Items

				for (i=0;i<ic;i++)
				{
					ar >> str;							// Read Item Type
					fd.Serialize(ar);					// Read From Date
					td.Serialize(ar);					// Read To Date

					AddDate(str, fd, td);
				}
			}
			pTable->SetEntry(NULL);
		}
	}
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaDoc - Class Function - SetModified()

void CSchedulaDoc::SetModified()
{
	if (!IsModified())
	{
		SetModifiedFlag(TRUE);
		//SetTitle("*"+GetTitle());
	}
}

/********************************************************************************/
// CSchedulaDoc - Class Function - UpdateSingleView()

void CSchedulaDoc::UpdateView(int view)
{
	SetActiveView(view);
	CDocument::UpdateAllViews(NULL);
	SetActiveView(ALLVIEWS);
}

/********************************************************************************/
// CSchedulaDoc - Class Function - DoUpdate()

BOOL CSchedulaDoc::DoUpdate(int view)
{
	if(GetActiveView()==0||GetActiveView()==view)
		return TRUE;
	else
		return FALSE;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaDoc - Class Function - AddEntry()

CEntry* CSchedulaDoc::AddEntry(CString name)
{
	CEntry* pEntry = pTable->AddEntry();
	pEntry->UpdateEntry(name);
	SetModified();
	return pEntry;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - DeleteEntry()

bool CSchedulaDoc::DeleteEntry(CEntry* pEntry)
{
	if(pTable->DelEntry(pEntry)) 
	{
		SetModified();
		return TRUE;
	}
	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - UpdateEntry()

CEntry* CSchedulaDoc::UpdateEntry(CString name)
{
	FOR_CURRENT_ENTRY(pEntry);

	if(pEntry!=NULL)
	{
		pEntry->UpdateEntry(name);
		SetModified();
	}
	return pEntry;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - SelEntry()

BOOL CSchedulaDoc::SelEntry(CEntry* pEntry)
{
	if (pTable->FindEntry(pEntry))
	{
		return TRUE;
	}
	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - AddDate()

void CSchedulaDoc::AddDate(CString type, CDate& fromDate, CDate& toDate)
{
	ASSERT(fromDate.GetNumDays()<=toDate.GetNumDays());

	FOR_CURRENT_DATE(pDate);
	FOR_CURRENT_ENTRY(pEntry);
	
	if (pEntry!=NULL)
	{
		CItem* pItem = pEntry->AddItem();
		pItem->UpdateItem(type, fromDate, toDate);		

		if(type=="V" || type=="D" || type=="U") 
		{
			pEntry->IncTotalDays(pItem->GetDays());

			for(int i=fromDate.GetNumDays();i<=toDate.GetNumDays();i++)
			{
				CDate* pThisDate = new CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i));
				if ( pThisDate->IsDay(i, CD_FRI | CD_SAT | CD_SUN) )
				{
					pEntry->IncTotalWeekEnds();
					while( i<toDate.GetNumDays() && pDate->IsDay(i, CD_FRI | CD_SAT /*| CD_SUN*/) ) i++;
				}
				delete pThisDate;
			}
		}

		SetModified();
	}
}

/********************************************************************************/
// CSchedulaDoc - Class Function - DeleteDate()

bool CSchedulaDoc::DeleteDate(CItem* date)
{
	FOR_CURRENT_DATE(pDate)
	FOR_CURRENT_ENTRY(pEntry);

	if(pEntry!=NULL)
	{
		if( (pEntry->FindItem(date)) != NULL )
		{
			FOR_CURRENT_ITEM(pItem);

			if(pItem->GetType()=="v" || pItem->GetType()=="d" || pItem->GetType()=="u")
			{
				//TODO: Add warning message
				return DeleteSeries(pTable->GetEntry()->GetItem()->GetType());
			}

			if(pItem->GetType()=="V" || pItem->GetType()=="D" || pItem->GetType()=="U") 
			{
				pEntry->DecTotalDays(pItem->GetDays());

				for(int i=pItem->GetDateFrom()->GetNumDays();i<=pItem->GetDateTo()->GetNumDays();i++)
				{
					CDate* pThisDate = new CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i));
					if ( pThisDate->IsDay(i, CD_FRI | CD_SAT | CD_SUN) )
					{
						pEntry->DecTotalWeekEnds();
						while( i<pItem->GetDateTo()->GetNumDays() && pDate->IsDay(i, CD_FRI | CD_SAT /*| CD_SUN*/) ) i++;
					}
					delete pThisDate;
				}
			}

			pEntry->DelItem(pItem);

			SetModified();
			return TRUE;
		}
	}

	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - AddSeries()

void CSchedulaDoc::AddSeries(CString type, CDate& fromDate, CDate& toDate)
{
//TODO: Not very elegant solution

	ASSERT(fromDate.GetNumDays()<=toDate.GetNumDays());

	FOR_CURRENT_DATE(pDate)
	FOR_CURRENT_ENTRY(pEntry)
	
	if (pEntry!=NULL)
	{
		CDate fd;
		if(type=="V")type="v";
		if(type=="D")type="d";
		if(type=="U")type="u";

		for(int i=fromDate.GetNumDays();i<=toDate.GetNumDays();i+=7)
		{
			CItem* pItem = pEntry->AddItem();
			pItem->UpdateItem(type, 
				CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i)), 
				CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i))
			);		

			if(type=="v" || type=="d" || type=="u")
			{
				pEntry->IncTotalDays(pItem->GetDays());

				if (pItem->GetDateFrom()->IsDay(i, CD_FRI | CD_SAT | CD_SUN))
					pEntry->IncTotalWeekEnds();

			}

			fromDate.AddDays(7);
		}
		SetModified();
	}
}

/********************************************************************************/
// CSchedulaDoc - Class Function - DeleteSeries()

bool CSchedulaDoc::DeleteSeries(CString type)
{
//TODO: Not very elegant solution, will deleted all series days without asking

	FOR_CURRENT_DATE(pDate)
	FOR_CURRENT_ENTRY(pEntry)
	FOR_CURRENT_ITEM_LIST(pItemList)
	
	FOR_EACH_ITEM(pItem, pItemList)

		if(pItem->GetType()==type)
		{
			pEntry->DecTotalDays(pItem->GetDays());

			if (pItem->GetDateFrom()->IsDay(pItem->GetDateFrom()->GetNumDays(), CD_FRI | CD_SAT | CD_SUN))
				pEntry->DecTotalWeekEnds();

			pEntry->DelItem(pItem);
		}
	
	END_EACH

	pEntry->SetItem(NULL);

	SetModified();
	return TRUE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - UpdateDate()

void CSchedulaDoc::UpdateDate(CString type, CDate& fromDate, CDate& toDate)
{
	FOR_CURRENT_DATE(pDate)
	FOR_CURRENT_ENTRY(pEntry);

	if(pEntry!=NULL)
	{
		FOR_CURRENT_ITEM(pItem);

		int i;

		if(pItem!=NULL)
		{
			if(type=="V" || type=="D" || type=="U")
			{
				pEntry->DecTotalDays(pItem->GetDays());

				for(i=pItem->GetDateFrom()->GetNumDays();i<=pItem->GetDateTo()->GetNumDays();i++)
				{
					CDate* pThisDate = new CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i));
					if ( pThisDate->IsDay(i, CD_FRI | CD_SAT | CD_SUN) )
					{
						pEntry->DecTotalWeekEnds();
						while( i<pItem->GetDateTo()->GetNumDays() && pDate->IsDay(i, CD_FRI | CD_SAT /*| CD_SUN*/) ) i++;
					}
					delete pThisDate;
				}
			}

			pItem->UpdateItem(type, fromDate, toDate);

			if(type=="V" || type=="D" || type=="U")
			{
				pEntry->IncTotalDays(pItem->GetDays());

				for(i=fromDate.GetNumDays();i<=toDate.GetNumDays();i++)
				{
					CDate* pThisDate = new CDate(pDate->GetYear(),pDate->MonthfromDays(i),pDate->DayfromDays(i));
					if ( pThisDate->IsDay(i, CD_FRI | CD_SAT | CD_SUN) )
					{
						pEntry->IncTotalWeekEnds();
						while( i<toDate.GetNumDays() && pDate->IsDay(i, CD_FRI | CD_SAT /*| CD_SUN*/) ) i++;
					}
					delete pThisDate;
				}
			}

			SetModified();
		}
	}
}

/********************************************************************************/
// CSchedulaDoc - Class Function - FindDate()

BOOL CSchedulaDoc::FindDate(int Year, int Month, int Day)
{
	FOR_CURRENT_ENTRY(pEntry);
	
	if (pEntry!=NULL)
	{
		CItem *pItemTemp = pEntry->GetItem();

		FOR_EACH_ITEM(pItem, pEntry->GetItemList())

			if( pItem->GetDateFrom()->GetYear()==Year &&
				pItem->GetDateFrom()->GetMonth()==Month &&
				pItem->GetDateFrom()->GetDay()==Day )
			{
				pEntry->SetItem(pItem);
				return TRUE;
			}

		END_EACH

		pEntry->SetItem(pItemTemp);
	}

	// pEntry->SetItem(NULL);

	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - FindAnyDate()

BOOL CSchedulaDoc::FindAnyDate(CDate& date, CString type)
{
	FOR_CURRENT_ENTRY_LIST(pEntryList)

	CEntry *pEntryTemp = pTable->GetEntry();

	FOR_EACH_ENTRY(pEntry, pEntryList)

		CItem *pItemTemp = pEntry->GetItem();

		FOR_EACH_ITEM(pItem, pEntry->GetItemList())

			if( pItem->GetDateFrom()->GetYear()==date.GetYear() &&
				pItem->GetDateFrom()->GetMonth()==date.GetMonth() &&
				pItem->GetDateFrom()->GetDay()==date.GetDay() &&
				pItem->GetType()==type )
			{
				pTable->SetEntry(pEntry);
				pEntry->SetItem(pItem);
				return TRUE;
			}

		END_EACH

		pEntry->SetItem(pItemTemp);

	END_EACH

	pTable->SetEntry(pEntryTemp);

	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - SwapDates()

BOOL CSchedulaDoc::SwapDates()
{
	POSITION pos;

	if( m_swapentry1 != m_swapentry2)
	{
		if(m_swapentry1!=NULL && m_swapitem1!=NULL && m_swapentry2!=NULL && m_swapitem2!=NULL)
		{
			if( SelEntry(m_swapentry1) && pTable->GetEntry()->GetItemList()->Find(m_swapitem1) ) {
				if( SelEntry(m_swapentry2) && pTable->GetEntry()->GetItemList()->Find(m_swapitem2 ) )
				{
					if(m_swapitem1->GetDays()==m_swapitem2->GetDays())
					{
						SelEntry(m_swapentry1);
						CItemList* pItemList1 = pTable->GetEntry()->GetItemList();
						pos = pItemList1->Find(m_swapitem1);
						pItemList1->SetAt(pos, m_swapitem2);

						SelEntry(m_swapentry2);
						CItemList* pItemList2 = pTable->GetEntry()->GetItemList();
						pos = pItemList2->Find(m_swapitem2);
						pItemList2->SetAt(pos, m_swapitem1);

						m_swapentry1 = m_swapentry2 = NULL;
						m_swapitem1 = m_swapitem2 = NULL;

						return TRUE;
					}
				}
			}
		}
	}

	m_swapentry1 = m_swapentry2 = NULL;
	m_swapitem1 = m_swapitem2 = NULL;

	return FALSE;
}

/********************************************************************************/
// CSchedulaDoc - Class Function - MoveDate()

BOOL CSchedulaDoc::MoveDate(int month, int day)
{
	if(m_swapentry1!=NULL && m_swapitem1!=NULL && m_swapentry2!=NULL)
	{
		if( SelEntry(m_swapentry1) && pTable->GetEntry()->GetItemList()->Find(m_swapitem1) )
		{
			if(m_swapitem1->GetDays()==1)
			{
				SelEntry(m_swapentry2);
				AddDate(m_swapitem1->GetType(), CDate(m_swapitem1->GetDateFrom()->GetYear(), month, day), CDate(m_swapitem1->GetDateTo()->GetYear(), month, day) );

				SelEntry(m_swapentry1);
				DeleteDate(m_swapitem1);

				SelEntry(m_swapentry2);

				m_swapentry1 = m_swapentry2 = NULL;
				m_swapitem1 = m_swapitem2 = NULL;

				return TRUE;
			}
		}
	}

	m_swapentry1 = m_swapentry2 = NULL;
	m_swapitem1 = m_swapitem2 = NULL;

	return FALSE;
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaDoc - Class Function - Schedule()

void CSchedulaDoc::Schedule()
{
	FOR_CURRENT_ENTRY_LIST(pEntryList)

	CEngineDlg engineDlg(this);

	FOR_EACH_ENTRY(pEntry, pEntryList)

		engineDlg.AddEntry(pEntry);

	END_EACH

	engineDlg.DoModal();

	SetScheduled(TRUE);
}

/********************************************************************************/
// CSchedulaDoc - Class Function - Reset()

void CSchedulaDoc::Reset()
{
	FOR_CURRENT_ENTRY_LIST(pEntryList)

	FOR_EACH_ENTRY(pEntry, pEntryList)

		FOR_EACH_ITEM(pItem, pEntry->GetItemList())

			if(pItem->GetType()=="s")
			{
				pEntry->DelItem(pItem);
			}

		END_EACH

	END_EACH

	SchApp.ReportDlg->Reset();

	SetScheduled(FALSE);
}

/********************************************************************************/
// SchedulaDoc.cpp - End Of File
/********************************************************************************/
