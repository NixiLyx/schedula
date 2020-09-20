/********************************************************************************/
// SchedulaPlanView.cpp : Implementation of the SchedulaPlanView class

#include "StdAfx.h"

#include "Schedula.h"

//#include "SchedulaPlanView.h"

/********************************************************************************/
// CSchedulaPlanView - Debuging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif //_DEBUG

#ifdef _DEBUG
void CSchedulaPlanView::AssertValid() const
{
	CView::AssertValid();
}
void CSchedulaPlanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CSchedulaDoc* CSchedulaPlanView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSchedulaDoc)));
	return (CSchedulaDoc*)m_pDocument;
}
#endif //_DEBUG


/********************************************************************************/
// CSchedulaPlanView - Class Initialisation

IMPLEMENT_DYNCREATE(CSchedulaPlanView, CView)

BEGIN_MESSAGE_MAP(CSchedulaPlanView, CView)
	//{{AFX_MSG_MAP(CSchedulaPlanView)
	// System Message Handlers
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	// Program Message Handlers
	ON_COMMAND(ID_PVN_FIND, OnPvnFind)
	ON_COMMAND(ID_PVN_EDIT, OnPvnEdit)
	ON_COMMAND(ID_PVN_ADD, OnPvnAdd)
	ON_COMMAND(ID_PVN_DELETE, OnPvnDelete)
	ON_COMMAND(ID_PVN_MARK, OnPvnMark)
	ON_COMMAND(ID_PVN_SWAP, OnPvnSwap)
	ON_COMMAND(ID_PVN_MOVE, OnPvnMove)
	ON_COMMAND(ID_PVN_CANCEL, OnPvnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/********************************************************************************/
// CSchedulaPlanView - Class Construction

CSchedulaPlanView::CSchedulaPlanView()
{
	tlBrush.CreateSolidBrush(RGB(200,200,200));		// Tile
	bgBrush.CreateSolidBrush(RGB(230,230,230));		// Background
	fgBrush.CreateSolidBrush(RGB(255,255,255));		// Foreground
	hlBrush.CreateSolidBrush(RGB(255,255,196));		// Hi Lite
	slBrush.CreateSolidBrush(RGB(255,196,196));		// Selected
	psBrush.CreateSolidBrush(RGB(196,196,255));		// Picked
	blBrush.CreateSolidBrush(RGB(148,148,255));		// Background Selected
	wrBrush.CreateSolidBrush(RGB(255,128,128));		// Red Warning
	wyBrush.CreateSolidBrush(RGB(255,255,128));		// Yellow Warning
	wgBrush.CreateSolidBrush(RGB(128,255,128));		// Green Warning
	olBrush.CreateSolidBrush(RGB(0,0,0));			// Outline
	tvBrush.CreateSolidBrush(RGB(128,255,128));		// L.Green
	tuBrush.CreateSolidBrush(RGB(255,255,128));		// L.Yellow
	tdBrush.CreateSolidBrush(RGB(255,128,128));		// L.Red
	tsBrush.CreateSolidBrush(RGB(128,128,255));		// L.Blue

	sPen.CreatePen(PS_SOLID,1,RGB(128,128,128));	// Thin Line
	dPen.CreatePen(PS_SOLID,2,RGB(128,128,128));	// Thick Line

	sFont.CreateFont(
		10,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		"Arial");					// lpszFacename

	mFont.CreateFont(
		12,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		"Arial");					// lpszFacename

	m_mode=0;
	m_mark=0;
}

/********************************************************************************/
// CSchedulaPlanView - Class Destruction

CSchedulaPlanView::~CSchedulaPlanView()
{
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaPlanView - Virtual Function - PreCreateWindow()
//
// Description: Define Styles for this view
//

BOOL CSchedulaPlanView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/********************************************************************************/
// CSchedulaPlanView - Virtual Function - OnUpdate()

void CSchedulaPlanView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	FOR_CURRENT_DOCUMENT(pDoc)

	// Check for Single View Update

	if(!pDoc->DoUpdate(PLANVIEW))
		return;

	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_ENTRY(pEntry)	

	if (pEntry!=NULL)	// Individual
		m_mode=0;
	else				// Everyone
		m_mode=1;

	// Zero Day Array

	for (int i=0;i<366;i++) { 
		m_days[i]=0;
		m_info[i]=0;
	}

	FOR_CURRENT_ENTRY_LIST(pEntryList)

	if (pEntryList->GetCount()>0)
	{
		// Define Color Scale Increment

		int scale=(255/(int)pEntryList->GetCount());	// 255 / Num of Doctors

		// Zero Day State Flag Accumulator

		unsigned long state=0;

		FOR_EACH_ENTRY(pEntry, pEntryList)

			// Get Item List

			CItemList* pItemList = pEntry->GetItemList();

			FOR_EACH_ITEM(pItem, pItemList)

				// Set Day State Flags for Current Item

				state=0;

			// * Individual Dates *

				if(pEntry==pTable->GetEntry())			// Is Current Entry ?
				{
					if(pItem==pTable->GetEntry()->GetItem())	// Is Current Item ?
						state|=(unsigned long)pow(2,31);

					if(pItem->GetTypeNum()==TYPE_V) state|=0x01;		// 'V'
					if(pItem->GetTypeNum()==TYPE_D) state|=0x02;		// 'D'
					if(pItem->GetTypeNum()==TYPE_U) state|=0x04;		// 'U'
					if(pItem->GetTypeNum()==TYPE_S) state|=0x08;		// 'S'
				}

			// * All Dates *

				if (pItem->GetTypeNum()==TYPE_S)		// Is Scheduled Day ?
					state|=(unsigned long)pow(2,30);

				if(pItem==pDoc->GetSwapItem())			// Is Item for Swap ?
					state|=(unsigned long)pow(2,29);

				if(pEntry==pDoc->GetSwapEntry())		// Is Entry for Swap ?
					state|=(unsigned long)pow(2,28);

			// * Individual & All Dates *

				// Calculate Date Range of Item

				int fdn = pItem->GetDateFrom()->GetNumDays()-1;	// -1 Adjust for Zero 
				int tdn = pItem->GetDateTo()->GetNumDays()-1;	// Indexed Day Array

				// Update Day Array for Date Range

				for(i=fdn;i<=tdn;i++)
				{
					m_days[i]|=state;
					m_info[i]+=scale;	// Increase Scale by one Doctor 
				}

			END_EACH

		END_EACH
	}
	Invalidate(TRUE);
	UpdateWindow();
}

/********************************************************************************/
// CSchedulaPlanView - Virtual Function - OnDraw()

void CSchedulaPlanView::OnDraw(CDC* pDC)
{
	// Calculate Screen Metrics

	OnDraw_SetMetrics(pDC);

	// Save Current Paint Objects

	oldBrush = pDC->SelectObject(&tlBrush);
	oldPen = pDC->SelectObject(&sPen);
	oldFont = pDC->SelectObject(&sFont);

	// Draw Calender Background

	OnDraw_Calender(pDC);

	// Add Dates

	OnDraw_Dates(pDC);

	// Restore Paint Objects

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
	pDC->SelectObject(&oldFont);
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaPlanView - System Message Handler - OnLButtonUp()

void CSchedulaPlanView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	if(point.x > pl && point.x < pr)
	{
		if(point.y > pt && point.y < pb)
		{
			py = 1 + (point.y-m-h)/vs ;
			px = 1 + (point.x-m-g)/hs - pDate->FirstofMonth(py) + 1 ;

			int i = pDate->DaysinTotal(py,px) - 1 ;

			if(px>=1 && px<=pDate->DaysinMonth(py))
			{
				if((m_days[i]&0xFF)>0)
				{
					if(m_mode==0)
					{
						if(pDoc->FindDate(pDate->GetYear(),py,px))
							OnUpdate(NULL, 0, NULL);
						else
							MessageBox("You must Select the first date in the range.");
					}
				}
			}
		}
	}

}

/********************************************************************************/
// CSchedulaPlanView - System Message Handler - OnRButtonDown()

void CSchedulaPlanView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);

	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	if(point.x > pl && point.x < pr)
	{
		if(point.y > pt && point.y < pb)
		{
			py = 1 + (point.y-m-h)/vs ;
			px = 1 + (point.x-m-g)/hs - pDate->FirstofMonth(py) + 1 ;

			int i = pDate->DaysinTotal(py,px) - 1 ;

			if(px>=1 && px<=pDate->DaysinMonth(py))
			{
				CMenu menu;
				menu.CreatePopupMenu();

				if (m_mode==0)
				{
					if( m_days[i]&(unsigned long)pow(2,30)){
						if((m_days[i]&0x08)) {
							if(!m_mark) {
								menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_MARK,"Pick Date");
								menu.AppendMenu(MF_SEPARATOR);
							} else {
								if(m_mark) {
									menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_SWAP,"Swap Date");
								}
								menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_CANCEL,"Cancel");
								menu.AppendMenu(MF_SEPARATOR);
							}
						} else {
							if(m_mark) {
								menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_MOVE,"Move Date");
								menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_CANCEL,"Cancel");
								menu.AppendMenu(MF_SEPARATOR);
							}
							menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_FIND,"Find Schedule");
							menu.AppendMenu(MF_SEPARATOR);
						}
					}
					if((m_days[i]&0xFF)>0){
						menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_EDIT,"Edit Date");
						menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_DELETE,"Delete Date");
					} else {
						menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_ADD,"Add Date");
					}
				}
				else
				{
					if(m_days[i]&(unsigned long)pow(2,30) && !(m_days[i]&0x08))
						menu.AppendMenu(MF_STRING | MF_ENABLED,ID_PVN_FIND,"Find Schedule");
				}
				ClientToScreen(&point);

				menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
											point.x, point.y,
											AfxGetApp()->m_pMainWnd,NULL);
				menu.Detach();
			}
		}
	}
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnEdit()

void CSchedulaPlanView::OnPvnEdit()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	if (pDoc->FindDate(pTable->GetYear(),py,px))
	{
		OnUpdate(NULL, 0, NULL);

		// Create and Call Input Dialogue
	
		CEntryDlg entryDlg(pDoc,UPDATE_DATE);

		entryDlg.DoModal();
	}
	else
	{
		MessageBox("You must Edit the first date in the range.");
	}
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnAdd()

void CSchedulaPlanView::OnPvnAdd()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	// Create and Call Input Dialogue

	CEntryDlg entryDlg(pDoc, ADD_DATE);

	entryDlg.SetDateFrom(CDate(pTable->GetYear(),py,px));
	entryDlg.SetDateTo(CDate(pTable->GetYear(),py,px));

	entryDlg.DoModal();
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnDelete()

void CSchedulaPlanView::OnPvnDelete()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	
	//TODO: What if more than one date entry exists on this day ?
	if (pDoc->FindDate(pTable->GetYear(),py,px))
	{
		FOR_CURRENT_ENTRY(pEntry)
		FOR_CURRENT_ITEM(pItem)

		pDoc->DeleteDate(pItem);

		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("You must Delete the first date in the range.");
	}
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnFind()

void CSchedulaPlanView::OnPvnFind()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	if(pDoc->FindAnyDate(CDate(pDate->GetYear(),py,px),"S"))
		pDoc->UpdateAllViews(NULL);
	else if(pDoc->FindAnyDate(CDate(pDate->GetYear(),py,px),"s"))
		pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnMark()

void CSchedulaPlanView::OnPvnMark()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	m_mark=1;

	if (pDoc->FindDate(pDate->GetYear(),py,px))
	{
		pDoc->SetSwapSource(pTable->GetEntry(), pTable->GetEntry()->GetItem());
	}

	pDoc->UpdateView(PLANVIEW);
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnSwap()

void CSchedulaPlanView::OnPvnSwap()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	m_mark=0;

	if (pDoc->FindDate(pDate->GetYear(),py,px))
	{
		pDoc->SetSwapDestination(pTable->GetEntry(), pTable->GetEntry()->GetItem());
	}

	if(!pDoc->SwapDates())
		MessageBox("Unable to Swap these dates.");

	pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnMove()

void CSchedulaPlanView::OnPvnMove()
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)

	m_mark=0;

	pDoc->SetSwapDestination(pTable->GetEntry(), pTable->GetEntry()->GetItem());

	if(!pDoc->MoveDate(py,px))
		MessageBox("Unable to Move this date.");

	pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/
// CSchedulaPlanView - Program Message Handler - OnPvnCancel()

void CSchedulaPlanView::OnPvnCancel()
{
	FOR_CURRENT_DOCUMENT(pDoc)

	m_mark=0;

	pDoc->SetSwapSource(NULL, NULL);
	pDoc->SetSwapDestination(NULL, NULL);

	pDoc->UpdateAllViews(NULL);
}

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CSchedulaPlanView - Macros

#define DATE(m, d)	m_days[pDate->DaysinTotal(m+1,d+1)-1]
#define INFO(m, d)	m_info[pDate->DaysinTotal(m+1,d+1)-1]

#define BOX(x, y)	&CRect( pl+(l*hs)+x+1, pt+y+1, pl+(l*hs)+x+hs, pt+y+vs )
#define ICON(x, y)	&CRect( pl+(l*hs)+x+(hs/4), pt+y+(vs/2), pl+(l*hs)+x+hs-(hs/4), pt+y+vs-(vs/4) )
#define SEL(x, y)	&CRect( pl+(l*hs)+x+(hs/4)-1, pt+y+(vs/2)-1, pl+(l*hs)+x+hs-(hs/4)+1, pt+y+vs-(vs/4)+1 )
#define CALL(x, y)	&CRect( pl+(l*hs)+x+hs-(hs/4), pt+y+vs-(vs/8), pl+(l*hs)+x+hs, pt+y+vs )

/********************************************************************************/
// CSchedulaPlanView - Class Function - OnDraw_SetMetrics()

void CSchedulaPlanView::OnDraw_SetMetrics(CDC* pDC)
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	TEXTMETRIC metrics;
	pDC->GetOutputTextMetrics(&metrics);

	h=metrics.tmHeight*2;								// Header
	ho=(h-metrics.tmHeight)/2;							// Header Offset
	g=metrics.tmAveCharWidth*4*2;						// Gutter
	go=(g-(metrics.tmAveCharWidth*4))/2;				// Gutter Offset

	wd = pDate->FirstofMonth(1)-1;						// Week Day

	int maxdays=0;
	for(i=0,j=wd;i<12;i++)								// Max Plan Width in Days
	{
		j+=pDate->DaysinMonth(i);
		maxdays=max(maxdays,j);
		j=(j%7);
	}

	GetClientRect(&rcClient);

	m=(rcClient.Width()/100)*3;							// Margin
	w=((rcClient.Width()-(m*2)-g)/maxdays)*maxdays;		// Plan Width
	d=((rcClient.Height()-(m*2)-h)/12)*12;				// Plan Depth
	hs=w/maxdays;										// Horizontal Spacing
	vs=d/12;											// Vertical Spacing
	vo=(vs-metrics.tmHeight)/2;							// Vertical Offset

	pt=m+h;			// Plan Top
	pl=m+g;			// Plan Left
	pb=pt+d;		// Plan Bottom
	pr=pl+w;		// Plan Right
}

/********************************************************************************/
// CSchedulaPlanView - Class Function - OnDraw_Calender()

void CSchedulaPlanView::OnDraw_Calender(CDC* pDC)
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	// ***** Draw Plan Background & Label Dates *****

	const CString dates[31]={"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

	pDC->SetBkMode(TRANSPARENT);						// Configure Text
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	// Draw Top & Left Hand Side Borders of Plan

	pDC->FillRect(&CRect(m,m,pr,pt), &tlBrush);			// Header
	pDC->FillRect(&CRect(m,m,pl,pb), &tlBrush);			// Gutter

	for (y=0,i=0,l=wd;y<d;y+=vs,i++)
	{
		int daysinmonth = pDate->DaysinMonth(i+1);

		// Draw a Plan Line - Days Before / During / After that Month

		pDC->FillRect(&CRect(pl,pt+y,pl+(l*hs),pt+y+vs), &bgBrush);								// Head
		pDC->FillRect(&CRect(pl+(l*hs),pt+y,pl+(l*hs)+(daysinmonth*hs),pt+y+vs), &fgBrush);	// Days in Months
		pDC->FillRect(&CRect(pl+(l*hs)+(daysinmonth*hs),pt+y,pr,pt+y+vs), &bgBrush);			// Tail

		for(j=0;j<daysinmonth;j++)
		{
			// Add Weekends on Top

			if ((j+l)%7==0 || (j+l+1)%7==0)
				pDC->FillRect(BOX(j*hs, y), &hlBrush);					// Sundays

			// Add Outline/Highlight for Days with Multiple Date Entries 

			int t=0;
			if(DATE(i,j)&0x01)t++;		// V
			if(DATE(i,j)&0x02)t++;		// D
			if(DATE(i,j)&0x04)t++;		// U
			if(DATE(i,j)&0x08)t++;		// S & s
			if(t>1) {
				wkBrush=NULL;
				int bm=(DATE(i,j)&0xFF);
				if(bm==(0x08|0x01)) wkBrush=&wrBrush;	// S & V
				if(bm==(0x08|0x02)) wkBrush=&wgBrush;	// S & D
				if(bm==(0x08|0x04)) wkBrush=&wyBrush;	// S & U
				if(wkBrush!=NULL) pDC->FillRect(BOX(j*hs, y), wkBrush);
			}

			// Draw Day Numbers 1-31

			pDC->TextOut(pl+(l*hs)+(j*hs)+(hs/2),pt+y+(vo/4),dates[j]);	// Date Labels
		}

		l=(l+daysinmonth)%7;
	}

	// ***** Draw Plan Forground & Label Borders *****

	const CString months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	const CString days[7]={"S", "M", "T", "W", "T", "F", "S"};

	pDC->SelectObject(&mFont);

	pDC->SetTextAlign(TA_LEFT | TA_TOP);

	// Draw Horizontal Grid Lines

	pDC->MoveTo(m,m); pDC->LineTo(pr,m);			// Header Divider Line

	for(y=0,i=0;y<=d;y+=vs,i++)						// Horizontal Grid Lines
	{
		pDC->MoveTo(m,pt+y); pDC->LineTo(pr,pt+y);
		if(i<12)
		{
			// Add Month Labels

			pDC->TextOut(m+go,pt+y+vo,months[i]);	// Month Labels
		}
	}

	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	// Draw Vertical Grid Lines

	pDC->MoveTo(m,m); pDC->LineTo(m,pb);			// Gutter Divder Line

	for(x=0,i=0;x<=w;x+=hs,i++)						// Vertical Grid Lines
	{
		pDC->MoveTo(pl+x,m); pDC->LineTo(pl+x,pb);
		if(x<w)
		{
			// Add Day Labels

			pDC->TextOut(pl+x+(hs/2),m+ho,days[i%7]);		// Day Labels
		}
	}
}

/********************************************************************************/
// CSchedulaPlanView - Class Function - OnDraw_Dates()

void CSchedulaPlanView::OnDraw_Dates(CDC* pDC)
{
	FOR_CURRENT_DOCUMENT(pDoc)
	FOR_CURRENT_TABLE(pTable)
	FOR_CURRENT_DATE(pDate)

	// ***** Draw Plan Data *****

	pDC->SelectObject(&dPen);

	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	for(y=0,i=0,l=wd;y<d;y+=vs,i++)
	{
		int daysinmonth = pDate->DaysinMonth(i+1);

		for(x=0,j=0;j<daysinmonth;x+=hs,j++)
		{
			if (m_mode==0)			// Individual Dates
			{
				// Highlight Current Selection

				if(DATE(i,j)&(unsigned long)pow(2,31))
					pDC->FillRect(SEL(x, y), &olBrush);			// Current Selection

				// Highlight Swap Items

				if(DATE(i,j)&(unsigned long)pow(2,29)) {
					pDC->FillRect(BOX(j*hs, y), &psBrush);		// Pick & Swap

//TODO; Figure this out !?!
//					if((DATE(i,j)&0xFF)>0)
//						m_mark=2;
//					else
//						m_mark=1;
				}

				// Draw a Date V/D/U/S

				if (DATE(i, j)&0xFF)
				{
					wkBrush=&fgBrush;
					if(DATE(i,j)&0x01)wkBrush=&tvBrush;
					if(DATE(i,j)&0x02)wkBrush=&tdBrush;
					if(DATE(i,j)&0x04)wkBrush=&tuBrush;
					if(DATE(i,j)&0x08)wkBrush=&tsBrush;
					pDC->FillRect(ICON(x, y), wkBrush);
				}
			}
			else					// All Dates
			{
				if (INFO(i,j)>0)
				{
					// Draw Shaded Box Scaled by Day Population 

					if ((bv=(INFO(i,j))&(unsigned long)0xFF)<128) {		// Green to Yellow
						br=bv*2;
						bg=255;
					} else {											// Yellow to Red
						br=255;
						bg=(255-bv)*2;
					}
					pDC->FillRect(ICON(x,y), &CBrush( br | (bg<<8) ));
				}
			}
								// Individual & All Dates

//			if(pDoc->IsScheduled())
			{
				if(m_mark) {
					if((DATE(i,j)&(unsigned long)pow(2,28)))
						pDC->FillRect(CALL(x,y), &CBrush( RGB(0,0,255) ) );		// Swap Possibilites
				} else {
					if((DATE(i,j)&(unsigned long)pow(2,30))) 
					{
//TODO: Quick fix to show doctors initials
						CEntry *pEntryTemp = pTable->GetEntry();
						if (pDoc->FindAnyDate(CDate(pDate->GetYear(), i+1, j+1),"s") ||
							pDoc->FindAnyDate(CDate(pDate->GetYear(), i+1, j+1),"S") )
						{
							if (pTable->GetEntry()!=NULL)
								pDC->TextOut(pl+(l*hs)+x+(hs/2)+1, pt+y+(vs/4),pTable->GetEntry()->GetInitials());		// Initials
						}
						pTable->SetEntry(pEntryTemp);

//						pDC->FillRect(CALL(x,y), &CBrush( RGB(0,0,0) ) );		// On Call
					}
					else
						pDC->FillRect(CALL(x,y), &CBrush( RGB(255,0,0) ) );		// Not Scheduled
				}

			}

		}
		l=(l+daysinmonth)%7;
	}
}

/********************************************************************************/
// SchedulaPlanView.cpp - End Of File
/********************************************************************************/
