#if !defined(AFX_TRIAN2D_H__E0994CB3_A783_4D90_BF39_EFDC4CCDC96E__INCLUDED_)
#define AFX_TRIAN2D_H__E0994CB3_A783_4D90_BF39_EFDC4CCDC96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Trian2d.h : header file

#include "ShevArray.h"
#include "Vector2d.h"

/////////////////////////////////////////////////////////////////////////////
// Trian2d dialog

class Trian2d : public CDialog
{
    Suite<Vector2d> poly;
// Construction
public:
	Trian2d(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Trian2d)
	enum { IDD = IDD_TRIAN_2D };
	int		m_poly_size;
	int		m_trian_type;
	BOOL	m_global;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Trian2d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Trian2d)
	afx_msg void OnNextPolygon();
	afx_msg void OnTriangulate();
	afx_msg void OnConvexParts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIAN2D_H__E0994CB3_A783_4D90_BF39_EFDC4CCDC96E__INCLUDED_)
