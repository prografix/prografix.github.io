// ShevTest.h : main header file for the SHEVTEST application
//

#if !defined(AFX_SHEVTEST_H__2DD56634_87C3_4FD1_9E62_821CE3A7CFAB__INCLUDED_)
#define AFX_SHEVTEST_H__2DD56634_87C3_4FD1_9E62_821CE3A7CFAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CShevTestApp:
// See ShevTest.cpp for the implementation of this class
//

class CShevTestApp : public CWinApp
{
public:
	CShevTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShevTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CShevTestApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEVTEST_H__2DD56634_87C3_4FD1_9E62_821CE3A7CFAB__INCLUDED_)
