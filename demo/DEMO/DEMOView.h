// DEMOView.h : interface of the CDEMOView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOVIEW_H__264A9A43_78A3_4F6E_B48B_E74103C9CFFD__INCLUDED_)
#define AFX_DEMOVIEW_H__264A9A43_78A3_4F6E_B48B_E74103C9CFFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDEMOView : public CView
{
protected: // create from serialization only
	CDEMOView();
	DECLARE_DYNCREATE(CDEMOView)

// Attributes
    HGLRC   m_hrc;
	CDC *   m_pDC;
    int     sx, sy;
    CPoint  m_point;
    bool    button_is_down;
public:
	class CDEMODoc* GetDocument();

// Operations
public:
    void reset();
    void scale ( double s );
    void move ( double x, double y, double z );
    void rotate ( double a, double x, double y, double z );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDEMOView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDEMOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDEMOView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DEMOView.cpp
inline CDEMODoc* CDEMOView::GetDocument()
   { return (CDEMODoc*)m_pDocument; }
#endif

extern CDEMOView * view;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOVIEW_H__264A9A43_78A3_4F6E_B48B_E74103C9CFFD__INCLUDED_)
