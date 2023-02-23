#if !defined(AFX_FILEDLG_H__EAFAA6A1_F4DD_11D1_B673_444553540000__INCLUDED_)
#define AFX_FILEDLG_H__EAFAA6A1_F4DD_11D1_B673_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StlFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StlFileDlg dialog

class StlFileDlg : public CFileDialog
{
// Construction
public:
    StlFileDlg();

    int m_iSurface;
// Dialog Data
	//{{AFX_DATA(StlFileDlg)
	enum { IDD = IDD_STLFILEDLG };
	CComboBox	m_cbSurface;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StlFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StlFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__EAFAA6A1_F4DD_11D1_B673_444553540000__INCLUDED_)
