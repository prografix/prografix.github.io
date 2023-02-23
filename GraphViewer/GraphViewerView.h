// GraphViewerView.h : interface of the CGraphViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHVIEWERVIEW_H__371AD6F2_9441_4910_B7B9_D7D7B7F15DBE__INCLUDED_)
#define AFX_GRAPHVIEWERVIEW_H__371AD6F2_9441_4910_B7B9_D7D7B7F15DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraphViewerView : public CView
{
protected: // create from serialization only
	CGraphViewerView();
	DECLARE_DYNCREATE(CGraphViewerView)

// Attributes
    HGLRC   m_hrc;
	CDC *   m_pDC;
    CPoint  m_point;
    bool    button_is_down;
    int     sx, sy;
public:

// Operations
public:
	class CGraphViewerDoc * GetDocument();
    void refresh_glContext();
    void reset();
    void scale ( double s );
    void move ( double x, double y, double z );
    void rotate ( double a, double x, double y, double z );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphViewerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GraphViewerView.cpp
inline CGraphViewerDoc* CGraphViewerView::GetDocument()
   { return (CGraphViewerDoc*)m_pDocument; }
#endif

extern CGraphViewerView * view;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEWERVIEW_H__371AD6F2_9441_4910_B7B9_D7D7B7F15DBE__INCLUDED_)
