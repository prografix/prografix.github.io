#if !defined(AFX_DEMOAPPROX3D_H__99191BD6_1438_45C4_BAA8_9FAF2F041CFD__INCLUDED_)
#define AFX_DEMOAPPROX3D_H__99191BD6_1438_45C4_BAA8_9FAF2F041CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DemoApprox3d.h : header file
//
#include "Polyhedron.h"

/////////////////////////////////////////////////////////////////////////////
// DemoApprox3d dialog

class DemoApprox3d : public CDialog
{
    Polyhedron poly;
// Construction
public:
	DemoApprox3d(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DemoApprox3d)
	enum { IDD = IDD_APPROX_3D };
	int		m_appr_var;
	int		m_points_num;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DemoApprox3d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DemoApprox3d)
	afx_msg void OnNextPoints();
	afx_msg void OnSelchangeComboVar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOAPPROX3D_H__99191BD6_1438_45C4_BAA8_9FAF2F041CFD__INCLUDED_)
