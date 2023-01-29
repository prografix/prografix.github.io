#if !defined(AFX_MAX2D3D_H__83B7BAF5_D2B3_4F16_B9B7_EEE2C1A2F5CD__INCLUDED_)
#define AFX_MAX2D3D_H__83B7BAF5_D2B3_4F16_B9B7_EEE2C1A2F5CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Max2d3d.h : header file
//

#include "ShevArray.h"
#include "Vector2d.h"
#include "Polyhedron.h"
#include "Rand.h"

/////////////////////////////////////////////////////////////////////////////
// Max2d3d dialog

class Max2d3d : public CDialog
{
    Suite<Vector2d> inner;
    Polyhedron outer;
    QRand2Vector3d vrand;
    Suite<Vector3d> point;
// Construction
public:
	Max2d3d(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Max2d3d)
	enum { IDD = IDD_MAX_2D3D };
	int		m_inner_nv;
	int		m_outer_nv;
	int		m_inner;
	int		m_outer;
	int		m_rotate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Max2d3d)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Max2d3d)
	afx_msg void OnNextRun();
	afx_msg void OnNextOuter();
	afx_msg void OnNextInner();
	afx_msg void OnSelchangeComboInner();
	afx_msg void OnSelchangeComboInnerNv();
	afx_msg void OnSelchangeComboOuter();
	afx_msg void OnSelchangeComboOuterNv();
	afx_msg void OnSelchangeComboRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Max2d3d2 dialog

class Max2d3d2 : public CDialog
{
// Construction
public:
	Max2d3d2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Max2d3d2)
	enum { IDD = IDD_MAX_2D3D };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Max2d3d2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Max2d3d2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAX2D3D_H__83B7BAF5_D2B3_4F16_B9B7_EEE2C1A2F5CD__INCLUDED_)
