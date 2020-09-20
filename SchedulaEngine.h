/********************************************************************************/
// SchedulaEngine.h : Interface of the CSchedulaEngine class

#pragma once

/********************************************************************************/
// CEngineDlg - Class Definition

class CEngineDlg : public CDialog
{
protected:
	DECLARE_DYNAMIC(CEngineDlg)

	//{{AFX_VIRTUAL(CEngineDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEngineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEngineSelectAll();
	afx_msg void OnEngineClear();
	afx_msg void OnEngineReset();
	afx_msg void OnEngineRun();
	afx_msg void OnEngineDone();
	afx_msg void OnEngineRadio(UINT	nID);
	//}}AFX_MSG

	//{{AFX_DATA(CEngineDlg)
	enum { IDD = IDD_ENGINEDLG };
	CProgressCtrl	m_progress;
	CListBox		m_list;
	CListBox		m_info;
	CButton			m_radio;
	//}}AFX_DATA

	// {{AFX_INSERT_LOCATION}}

	DECLARE_MESSAGE_MAP()

// User Stuff Below

private:
	int m_selbuf[MAXENTRYS];
	int m_method;
	CPtrList* m_engineList;
	CPtrList* m_engineSelList;

	CSchedulaDoc* pDoc;

protected:
	void Message(CString msg);
	void Process1();

public:
	CEngineDlg(CSchedulaDoc* pDoc);
	virtual ~CEngineDlg();

	void AddEntry(CEntry* pEntry);

};

/********************************************************************************/
// CDataHead - Class Definition

class /*CLASS_DECLSPEC*/ CDataHead
{
private:

protected:

public:
//TODO: Make private
	CEntry* pEntry;
	int items;
	int dayweight;
	int wkdweight;
	int order;

public:
	CDataHead(CEntry* pEntry);
	~CDataHead();
};

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CDataBody - Class Definition

class /*CLASS_DECLSPEC*/ CDataBody
{
private:

protected:

public:
//TODO: Make private
	CDataHead* pHead;
	CItem* pItem;
	int weight;

public:
	CDataBody(CDataHead* pHead, CItem* pItem, int weight);
	~CDataBody();
};

/********************************************************************************/


					//----------Section Break----------//


/********************************************************************************/
// CDataSort - Class Definition

class CSchedulaDoc ;

class /*CLASS_DECLSPEC*/ CDataSort
{
private:
	CPtrList Headers;
	CPtrList Data[367];
	int days;
	BOOL weekday;
    
protected:
	void Bubble(CPtrList* pData);
	BOOL BubbleAlg(CDataBody* pHighData, CDataBody* pLowData);
	CDataBody* Sieve(CPtrList* pData);
	BOOL SieveAlg(CDataBody* pHighData, CDataBody* pLowData, int tweak);

public:
	CDataSort(int days);
	~CDataSort();

	void Reset();
	CDataHead* AddHead(CEntry* pEntry);
	BOOL AddData(CDataHead* pHead, CItem* pItem, int day, int weight);
	void Schedule(CSchedulaDoc* pDoc);
	void Report(CSchedulaDoc* pDoc);
};

/********************************************************************************/
// CSchedulaEngine.h - End Of File
/********************************************************************************/
