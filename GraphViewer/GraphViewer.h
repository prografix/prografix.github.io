// GraphViewer.h : main header file for the GRAPHVIEWER application
//

#if !defined(AFX_GRAPHVIEWER_H__836CF74C_F088_43BE_B958_A2B732183457__INCLUDED_)
#define AFX_GRAPHVIEWER_H__836CF74C_F088_43BE_B958_A2B732183457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerApp:
// See GraphViewer.cpp for the implementation of this class
//

class CGraphViewerApp : public CWinApp
{
public:
	CGraphViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGraphViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEWER_H__836CF74C_F088_43BE_B958_A2B732183457__INCLUDED_)
