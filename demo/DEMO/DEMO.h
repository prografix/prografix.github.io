// DEMO.h : main header file for the DEMO application
//

#if !defined(AFX_DEMO_H__2BE9E9A1_409F_40C6_8422_A3F3F820E61E__INCLUDED_)
#define AFX_DEMO_H__2BE9E9A1_409F_40C6_8422_A3F3F820E61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDEMOApp:
// See DEMO.cpp for the implementation of this class
//

class CDEMOApp : public CWinApp
{
public:
	CDEMOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDEMOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDEMOApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_H__2BE9E9A1_409F_40C6_8422_A3F3F820E61E__INCLUDED_)
