#if !defined(AFX_MAXCIRCLE_H__04AD4439_2627_4196_9383_0DE89446F1AF__INCLUDED_)
#define AFX_MAXCIRCLE_H__04AD4439_2627_4196_9383_0DE89446F1AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaxCircle.h : header file
//
#include "ShevArray.h"
#include "Vector2d.h"

/////////////////////////////////////////////////////////////////////////////
// MaxCircle dialog

class MaxCircle : public CDialog
{
    Suite<Vector2d> outer;
    Suite<Vector2d> inner;
// Construction
public:
	MaxCircle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MaxCircle)
	enum { IDD = IDD_MAX_2D };
	int		m_inner_nv;
	int		m_outer_nv;
	int		m_inner;
	int		m_outer;
	int		m_rotate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MaxCircle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MaxCircle)
	afx_msg void OnNextRun();
	afx_msg void OnNextOuter();
	afx_msg void OnNextInner();
	afx_msg void OnSelchangeComboInner();
	afx_msg void OnSelchangeComboInnerNv();
	afx_msg void OnSelchangeComboOuter();
	afx_msg void OnSelchangeComboOuterNv();
	afx_msg void OnSelchangeComboRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAXCIRCLE_H__04AD4439_2627_4196_9383_0DE89446F1AF__INCLUDED_)
