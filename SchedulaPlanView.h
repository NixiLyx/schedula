/********************************************************************************/
// SchedulaPlanView.h : Interface of the SchedulaTreeView class

#pragma once

/********************************************************************************/
// CSchedulaPlanView - Class Definition

class CSchedulaPlanView : public CView
{
protected:
	DECLARE_DYNCREATE(CSchedulaPlanView)

	//{{AFX_VIRTUAL(CSchedulaPlanView)
	// Virtual Functions
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnDraw(CDC* pDC); 
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSchedulaPlanView)
	// System Message Handlers
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// Program Message Handlers
	afx_msg void OnPvnEdit();
	afx_msg void OnPvnAdd();
	afx_msg void OnPvnDelete();
	afx_msg void OnPvnFind();
	afx_msg void OnPvnMark();
	afx_msg void OnPvnSwap();
	afx_msg void OnPvnMove();
	afx_msg void OnPvnCancel();
	//}}AFX_MSG

	// {{AFX_INSERT_LOCATION}}

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	DECLARE_MESSAGE_MAP()


// User Stuff Below

private:
	CClientDC* pDC;
	CRect rcClient;
	CBrush tlBrush, bgBrush, fgBrush;
	CBrush hlBrush, slBrush, olBrush, blBrush, psBrush;
	CBrush wrBrush, wyBrush, wgBrush;
	CBrush tvBrush, tdBrush, tuBrush, tsBrush;
	CBrush *wkBrush, *oldBrush;
	CPen sPen, dPen, *oldPen;
	CFont sFont, mFont, *oldFont;

	int m, h, ho, g, go, w, d, hs, vs, vo;
	int wd, x, y, i, j, l;
	int pt, pl, pb, pr, py, px;
	int bv, br, bg;

	unsigned long m_days [366];
	unsigned long m_info [366];
	int m_mode;
	int m_mark;

protected:
	CSchedulaPlanView();
	~CSchedulaPlanView();

	CSchedulaDoc* GetDocument() const;

	void OnDraw_SetMetrics(CDC* pDC);
	void OnDraw_Calender(CDC* pDC);
	void OnDraw_Dates(CDC* pDC);

public:

};

/********************************************************************************/

#ifndef _DEBUG  // debug version in SchedulaPlanView.cpp
inline CSchedulaDoc* CSchedulaPlanView::GetDocument() const
{ 
	return reinterpret_cast<CSchedulaDoc*>(m_pDocument);
}
#endif

/********************************************************************************/
// SchedulaPlanView.h - End Of File
/********************************************************************************/

