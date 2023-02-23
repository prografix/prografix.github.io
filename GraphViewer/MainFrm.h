// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1DAC4DFF_6CA2_45CD_AB19_E89257BB9868__INCLUDED_)
#define AFX_MAINFRM_H__1DAC4DFF_6CA2_45CD_AB19_E89257BB9868__INCLUDED_

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
    double m_dAngle, m_dScale, m_dDist;

// Operations
public:

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
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnMovXm();
	afx_msg void OnMovXp();
	afx_msg void OnReset();
	afx_msg void OnRotXCcw();
	afx_msg void OnRotXCw();
	afx_msg void OnRotYCcw();
	afx_msg void OnRotYCw();
	afx_msg void OnRotZCcw();
	afx_msg void OnRotZCw();
	afx_msg void OnScene();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnMovYm();
	afx_msg void OnMovYp();
	afx_msg void OnMovZm();
	afx_msg void OnMovZp();
	afx_msg void OnRunForward();
	afx_msg void OnRunBack();
	afx_msg void OnRunPause();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1DAC4DFF_6CA2_45CD_AB19_E89257BB9868__INCLUDED_)
