#if !defined(AFX_RANDOMVEC_H__3FF55C21_1A72_4832_A53D_91B4CBDD622C__INCLUDED_)
#define AFX_RANDOMVEC_H__3FF55C21_1A72_4832_A53D_91B4CBDD622C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RandomVec.h : header file
//
#include "Polyhedron.h"

/////////////////////////////////////////////////////////////////////////////
// RandomVec dialog

class RandomVec : public CDialog
{
// Construction
public:
	RandomVec(CWnd* pParent = NULL);   // standard constructor

    Polyhedron poly;
// Dialog Data
	//{{AFX_DATA(RandomVec)
	enum { IDD = IDD_RANDOM_VEC };
	int		m_nVec;
	int		m_rand_type;
	CString	m_area_min;
	CString	m_area_max;
	CString	m_edge_min;
	CString	m_edge_max;
	CString	m_area_sigma;
	CString	m_edge_sigma;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RandomVec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RandomVec)
	afx_msg void OnRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANDOMVEC_H__3FF55C21_1A72_4832_A53D_91B4CBDD622C__INCLUDED_)
