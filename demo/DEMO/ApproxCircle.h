#if !defined(AFX_APPROXCIRCLE_H__9FB20940_C29B_44BA_83CD_E082214CEF6B__INCLUDED_)
#define AFX_APPROXCIRCLE_H__9FB20940_C29B_44BA_83CD_E082214CEF6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApproxCircle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ApproxCircle dialog

class ApproxCircle : public CDialog
{
    class Appr2d_data * data;
// Construction
public:
	ApproxCircle(CWnd* pParent = NULL);   // standard constructor
    ~ApproxCircle();

// Dialog Data
	//{{AFX_DATA(ApproxCircle)
	enum { IDD = IDD_APPROX_2D };
	int		m_points_num;
	int		m_appr_var;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ApproxCircle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ApproxCircle)
	afx_msg void OnNextPoints();
	afx_msg void OnSelchangeComboVar();
	afx_msg void OnSelchangePointsNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPROXCIRCLE_H__9FB20940_C29B_44BA_83CD_E082214CEF6B__INCLUDED_)
