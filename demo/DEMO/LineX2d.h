#if !defined(AFX_LINEX2D_H__B27390F2_A724_4744_9043_962D5DDBCE42__INCLUDED_)
#define AFX_LINEX2D_H__B27390F2_A724_4744_9043_962D5DDBCE42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineX2d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LineX2d dialog

#include "rand.h"
#include "Vector2d.h"

class LineX2d : public CDialog
{
    PRand rand;
// Construction
public:
	LineX2d(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LineX2d)
	enum { IDD = IDD_LINES_X_2D };
	int		m_line_num;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LineX2d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LineX2d)
	afx_msg void OnNextLines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEX2D_H__B27390F2_A724_4744_9043_962D5DDBCE42__INCLUDED_)
