#if !defined(AFX_SCENEPAR_H__AED4B22C_BB57_453B_83EE_B60FAEC9873B__INCLUDED_)
#define AFX_SCENEPAR_H__AED4B22C_BB57_453B_83EE_B60FAEC9873B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScenePar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ScenePar dialog

class ScenePar : public CDialog
{
// Construction
public:
	ScenePar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ScenePar)
	enum { IDD = IDD_MOVEPROP };
	CSliderCtrl	m_slDistance;
	CSliderCtrl	m_slMagnify;
	CSliderCtrl	m_slAngle;
	double	m_dAngle;
	double	m_dDistance;
	double	m_dMagnify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ScenePar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ScenePar)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEPAR_H__AED4B22C_BB57_453B_83EE_B60FAEC9873B__INCLUDED_)
