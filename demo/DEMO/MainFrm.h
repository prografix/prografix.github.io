// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__09BB1A5E_8AD2_4F71_9F4B_29DDB6D02B31__INCLUDED_)
#define AFX_MAINFRM_H__09BB1A5E_8AD2_4F71_9F4B_29DDB6D02B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
    double m_dAngle, m_dScale, m_dDist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar  m_wndToolBar;
    CDialog * rv_pnl;
    CDialog * trian2d_pnl;
    CDialog * trian3d_pnl;
    CDialog * lineX_pnl;
    CDialog * maxsph_pnl;
    CDialog * maxcir_pnl;
    CDialog * max2d3d_pnl;
    CDialog * min2d_pnl;
    CDialog * min3d_pnl;
    CDialog * approx2d;
    CDialog * approx3d;
    CDialog * intersect2d;
    CDialog * intersect3d;
    CDialog * other;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnScene();
	afx_msg void OnMovXm();
	afx_msg void OnMovXp();
	afx_msg void OnMovYm();
	afx_msg void OnMovYp();
	afx_msg void OnMovZm();
	afx_msg void OnMovZp();
	afx_msg void OnReset();
	afx_msg void OnRotXCcw();
	afx_msg void OnRotXCw();
	afx_msg void OnRotYCcw();
	afx_msg void OnRotYCw();
	afx_msg void OnRotZCcw();
	afx_msg void OnRotZCw();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnDemoRandomVectors();
	afx_msg void OnDemoLinesX2d();
	afx_msg void OnDemoMaxi3d();
	afx_msg void OnDemoMini3d();
	afx_msg void OnDemoMaxi2d();
	afx_msg void OnDemoMini2d();
	afx_msg void OnDemoApprox2d();
	afx_msg void OnDemoApprox3d();
	afx_msg void OnDemoIntersect2d();
	afx_msg void OnDemoTrian3d();
	afx_msg void OnDemoTrian2d();
	afx_msg void OnDemoIntersect3d();
	afx_msg void OnDemoOther();
	afx_msg void OnDemoMaxi2d3d();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__09BB1A5E_8AD2_4F71_9F4B_29DDB6D02B31__INCLUDED_)
