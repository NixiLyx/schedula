/********************************************************************************/
// SchedulaTreeView.h : Interface of the SchedulaTreeView class

#pragma once

/********************************************************************************/
// CSchedulaTreeView - Class Definition

class CSchedulaTreeView : public CTreeView
{
protected:
	DECLARE_DYNCREATE(CSchedulaTreeView)

	//{{AFX_VIRTUAL(CSchedulaTreeView)
	// Virtual Functions
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaTreeView)
	// System Message Handlers
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// Program Message Handlers
	afx_msg void OnTvnEdit();
	afx_msg void OnTvnAdd();
	afx_msg void OnTvnDelete();
	afx_msg void OnTvnAppend();
	afx_msg void OnTvnSeries();
	afx_msg void OnTvnSchedule();
	afx_msg void OnTvnReport();
	afx_msg void OnTvnReset();
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	DECLARE_MESSAGE_MAP()

// User Stuff Below

private:
	HTREEITEM m_hRoot;

protected:
	CSchedulaTreeView();
	~CSchedulaTreeView();

	virtual CSchedulaDoc* GetDocument() const;

public:

};

/********************************************************************************/

#ifndef _DEBUG  // debug version in SchedulaTreeView.cpp
inline CSchedulaDoc* CSchedulaTreeView::GetDocument() const
{ 
	return reinterpret_cast<CSchedulaDoc*>(m_pDocument);
}
#endif

/********************************************************************************/
// SchedulaTreeView.h - End Of File
/********************************************************************************/


