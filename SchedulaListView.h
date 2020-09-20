/********************************************************************************/
// SchedulaListView.h : Interface of the SchedulaTreeView class

#pragma once

/********************************************************************************/
// CSchedulaListView - Class Definition

class CSchedulaListView : public CListView
{
protected:
	DECLARE_DYNCREATE(CSchedulaListView)

	//{{AFX_VIRTUAL(CSchedulaListView)
	// Virtual Functions
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate(); 
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaListView)
	// System Message Handlers
	afx_msg void OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// Program Message Handlers
	afx_msg void OnLvnEdit();
	afx_msg void OnLvnDelete();
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:

protected:
	CSchedulaListView();
	~CSchedulaListView();

	CSchedulaDoc* GetDocument() const;

public:

};

/********************************************************************************/

#ifndef _DEBUG  // debug version in SchedulaListView.cpp
inline CSchedulaDoc* CSchedulaListView::GetDocument() const
{ 
	return reinterpret_cast<CSchedulaDoc*>(m_pDocument);
}
#endif

/********************************************************************************/
// SchedulaListView.h - End Of File
/********************************************************************************/
