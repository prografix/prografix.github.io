#if !defined(AFX_DEMOINTERSECT2D_H__3EEFFC52_77F1_4D5B_AFFD_FF13EA343065__INCLUDED_)
#define AFX_DEMOINTERSECT2D_H__3EEFFC52_77F1_4D5B_AFFD_FF13EA343065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DemoIntersect2d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect2d dialog

class DemoIntersect2d : public CDialog
{
// Construction
    class Intersect2d_data * data;
public:
	DemoIntersect2d(CWnd* pParent = NULL);   // standard constructor
    ~DemoIntersect2d();

// Dialog Data
	//{{AFX_DATA(DemoIntersect2d)
	enum { IDD = IDD_INTERSECT_2D };
	int		m_nv1;
	int		m_nv2;
	int		m_type1;
	int		m_type2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DemoIntersect2d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DemoIntersect2d)
	afx_msg void OnNext1();
	afx_msg void OnNext2();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo1Nv();
	afx_msg void OnSelchangeCombo2Nv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOINTERSECT2D_H__3EEFFC52_77F1_4D5B_AFFD_FF13EA343065__INCLUDED_)
