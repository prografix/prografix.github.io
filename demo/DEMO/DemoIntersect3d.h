#if !defined(AFX_DEMOINTERSECT3D_H__C0EBCA90_B941_4A7F_BB65_2B65C55AC656__INCLUDED_)
#define AFX_DEMOINTERSECT3D_H__C0EBCA90_B941_4A7F_BB65_2B65C55AC656__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DemoIntersect3d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect3d dialog

class DemoIntersect3d : public CDialog
{
// Construction
    class Intersect3d_data * data;
public:
	DemoIntersect3d(CWnd* pParent = NULL);   // standard constructor
    ~DemoIntersect3d();

// Dialog Data
	//{{AFX_DATA(DemoIntersect3d)
	enum { IDD = IDD_INTERSECT_3D };
	int		m_type1;
	int		m_type2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DemoIntersect3d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DemoIntersect3d)
	afx_msg void OnNext1();
	afx_msg void OnNext2();
	afx_msg void OnSelchangeCombo2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOINTERSECT3D_H__C0EBCA90_B941_4A7F_BB65_2B65C55AC656__INCLUDED_)
