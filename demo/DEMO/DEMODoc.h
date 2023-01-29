// DEMODoc.h : interface of the CDEMODoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODOC_H__29C109C8_90E6_4FF7_8B83_1A815E61B434__INCLUDED_)
#define AFX_DEMODOC_H__29C109C8_90E6_4FF7_8B83_1A815E61B434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Polyhedron.h"

class CDEMODoc : public CDocument
{
protected: // create from serialization only
	CDEMODoc();
	DECLARE_DYNCREATE(CDEMODoc)

// Attributes
public:
    int m_model_view;
    Polyhedron poly;

// Operations
public:
    void drawPolyhedron ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDEMODoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDEMODoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDEMODoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnModelFlat();
	afx_msg void OnModelSmooth();
	afx_msg void OnModelWire();
	afx_msg void OnUpdateModelFlat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelSmooth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelWire(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODOC_H__29C109C8_90E6_4FF7_8B83_1A815E61B434__INCLUDED_)
