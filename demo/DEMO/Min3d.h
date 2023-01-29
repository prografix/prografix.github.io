#if !defined(AFX_MIN3D_H__8BEA1C9E_B917_408A_95A6_DF1B7B66F2B7__INCLUDED_)
#define AFX_MIN3D_H__8BEA1C9E_B917_408A_95A6_DF1B7B66F2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Min3d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Min3d dialog

class Min3d : public CDialog
{
    class Min3d_data * data;
// Construction
public:
	Min3d(CWnd* pParent = NULL);   // standard constructor
    ~Min3d();

// Dialog Data
	//{{AFX_DATA(Min3d)
	enum { IDD = IDD_MIN_3D };
	int		m_inner_nv;
	int		m_combo_outer;
	int		m_combo_inner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Min3d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Min3d)
	afx_msg void OnNextInner();
	afx_msg void OnSelchangeComboOuter();
	afx_msg void OnSelchangeComboInnerNv();
	afx_msg void OnSelchangeComboInner();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIN3D_H__8BEA1C9E_B917_408A_95A6_DF1B7B66F2B7__INCLUDED_)
