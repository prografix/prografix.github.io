// GraphViewerDoc.h : interface of the CGraphViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHVIEWERDOC_H__82C7965A_D236_45FD_98D1_66C6441BF6DE__INCLUDED_)
#define AFX_GRAPHVIEWERDOC_H__82C7965A_D236_45FD_98D1_66C6441BF6DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraphViewerDoc : public CDocument
{
protected: // create from serialization only
	CGraphViewerDoc();
	DECLARE_DYNCREATE(CGraphViewerDoc)

// Attributes
public:
    class GraphicInterface * grdata;
    class GLDevice * device;

// Operations
public:
    void set ( GraphicInterface * gi ) ;
    void draw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphViewerDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnImportStl();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnImportObj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEWERDOC_H__82C7965A_D236_45FD_98D1_66C6441BF6DE__INCLUDED_)
