#if !defined(AFX_CTRIAN3D_H__59288EC9_867A_481B_9AF5_4DFAE3E90E4B__INCLUDED_)
#define AFX_CTRIAN3D_H__59288EC9_867A_481B_9AF5_4DFAE3E90E4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTrian3d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Trian3d dialog

class Trian3d : public CDialog
{
// Construction
public:
	Trian3d(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Trian3d)
	enum { IDD = IDD_TRIAN_3D };
	int		m_N_vertex;
	int		m_var;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Trian3d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Trian3d)
	afx_msg void OnRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRIAN3D_H__59288EC9_867A_481B_9AF5_4DFAE3E90E4B__INCLUDED_)
