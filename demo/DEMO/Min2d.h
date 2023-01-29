#if !defined(AFX_MIN2D_H__BF9F3A2E_3D8F_444F_A4CB_817AA6BE71D4__INCLUDED_)
#define AFX_MIN2D_H__BF9F3A2E_3D8F_444F_A4CB_817AA6BE71D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Min2d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Min2d dialog

class Min2d : public CDialog
{
    class Min2d_data * data;
// Construction
public:
	Min2d(CWnd* pParent = NULL);   // standard constructor
    ~Min2d();

// Dialog Data
	//{{AFX_DATA(Min2d)
	enum { IDD = IDD_MIN_2D };
	int		m_inner_nv;
	int		m_outer;
	int		m_outer_nv;
	int		m_inner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Min2d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Min2d)
	afx_msg void OnNextInner();
	afx_msg void OnNextOuter();
	afx_msg void OnSelchangeComboOuter();
	afx_msg void OnSelchangeComboInner();
	afx_msg void OnSelchangeComboInnerNv();
	afx_msg void OnSelchangeComboOuterNv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIN2D_H__BF9F3A2E_3D8F_444F_A4CB_817AA6BE71D4__INCLUDED_)
