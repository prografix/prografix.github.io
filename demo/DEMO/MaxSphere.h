#if !defined(AFX_MAXSPHERE_H__947C42BB_945C_4D1B_A75B_26D6160A6F17__INCLUDED_)
#define AFX_MAXSPHERE_H__947C42BB_945C_4D1B_A75B_26D6160A6F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaxSphere.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MaxSphere dialog

class MaxSphere : public CDialog
{
// Construction
    class Max3d_data * data;
public:
	MaxSphere(CWnd* pParent = NULL);   // standard constructor
    ~MaxSphere();

// Dialog Data
	//{{AFX_DATA(MaxSphere)
	enum { IDD = IDD_MAX_3D };
	int		m_nvert;
	int		m_inner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MaxSphere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MaxSphere)
	afx_msg void OnNextPoly();
	afx_msg void OnSelchangeComboInner();
	afx_msg void OnSelchangeComboOuter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAXSPHERE_H__947C42BB_945C_4D1B_A75B_26D6160A6F17__INCLUDED_)
