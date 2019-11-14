// ShevTestView.h : interface of the CShevTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHEVTESTVIEW_H__0B536E66_1852_488B_9739_473D2D106922__INCLUDED_)
#define AFX_SHEVTESTVIEW_H__0B536E66_1852_488B_9739_473D2D106922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShevTestDoc;

class CShevTestView : public CView
{
protected: // create from serialization only
	CShevTestView();
	DECLARE_DYNCREATE(CShevTestView)

// Attributes
    HGLRC   m_hrc;
	CDC *   m_pDC;
    int     sx, sy;
    CPoint  m_point;
    bool    button_is_down;
public:
	CShevTestDoc * GetDocument();

// Operations
public:
    void reset();
    void scale ( double s );
    void move ( double x, double y, double z );
    void rotate ( double a, double x, double y, double z );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShevTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShevTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShevTestView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ShevTestView.cpp
inline CShevTestDoc* CShevTestView::GetDocument()
   { return (CShevTestDoc*)m_pDocument; }
#endif

extern CShevTestView * view;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEVTESTVIEW_H__0B536E66_1852_488B_9739_473D2D106922__INCLUDED_)
