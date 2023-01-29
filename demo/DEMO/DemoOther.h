#if !defined(AFX_DEMOOTHER_H__759115BF_06B4_490A_85FC_9B4DCA4C9DE0__INCLUDED_)
#define AFX_DEMOOTHER_H__759115BF_06B4_490A_85FC_9B4DCA4C9DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DemoOther.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DemoOther dialog

class DemoOther : public CDialog
{
// Construction
public:
	DemoOther(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DemoOther)
	enum { IDD = IDD_OTHER };
	int		m_var;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DemoOther)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DemoOther)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOOTHER_H__759115BF_06B4_490A_85FC_9B4DCA4C9DE0__INCLUDED_)
