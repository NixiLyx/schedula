/********************************************************************************/
// SchedulaDoc.h : Interface of the CSchedulaDoc class

#pragma once

/********************************************************************************/

// Known Error Conditions
//
// 0x001 : FindDate() Failed to find reference to an CItem Ptr retrieved from ListView item
// 0x002 : ??


/********************************************************************************/

#define ALLVIEWS	0
#define TREEVIEW	1
#define LISTVIEW	2
#define PLANVIEW	3

#define MAXENTRYS	30

/********************************************************************************/
// CSchedulaDoc - Class Definition

class CSchedulaDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CSchedulaDoc)

	//{{AFX_VIRTUAL(CSchedulaDoc)
	// Virtual Functions
	virtual BOOL OnNewDocument();
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaDoc)
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:
	int m_active_view;
	CTable* pTable;
	BOOL m_scheduled;

	class CEntry *m_swapentry1, *m_swapentry2;
	class CItem *m_swapitem1, *m_swapitem2;

protected:
	CSchedulaDoc();
	~CSchedulaDoc();

	inline int GetActiveView() { return m_active_view; }
	inline void SetActiveView(int view) { m_active_view=view; }

public:
	inline CTable* GetTable() { return pTable; } 
	inline BOOL IsScheduled() { return m_scheduled; }
	inline void SetScheduled(BOOL scheduled) { m_scheduled=scheduled; }
	inline void SetSwapSource(CEntry* entrySource, CItem* itemSource) { m_swapentry1=entrySource; m_swapitem1=itemSource; }
	inline void SetSwapDestination(CEntry* entryDest, CItem* itemDest) { m_swapentry2=entryDest; m_swapitem2=itemDest; }
	inline CEntry* GetSwapEntry() { return m_swapentry1; }
	inline CItem* GetSwapItem() { return m_swapitem1; }

	void SetModified();
	void UpdateView(int view);
	BOOL DoUpdate(int view);

	CEntry* AddEntry(CString Name);
	bool DeleteEntry(CEntry* entry);
	CEntry* UpdateEntry(CString Name);
	BOOL SelEntry(CEntry* entry);

	void AddDate(CString type, CDate& fromDate, CDate& toDate);
	bool DeleteDate(CItem* date);
	void AddSeries(CString type, CDate& fromDate, CDate& toDate);
	bool DeleteSeries(CString type);
	void UpdateDate(CString type, CDate& fromDate, CDate& toDate);
	BOOL FindDate(int Year, int Month, int Day);
	BOOL FindAnyDate(CDate& date, CString type);
	BOOL SwapDates();
	BOOL MoveDate(int month, int date);

	void Schedule();
	void Reset();
};

/********************************************************************************/
// CSchedulaDoc.h - End Of File
/********************************************************************************/
