// ShevTestDoc.h : interface of the CShevTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHEVTESTDOC_H__B3A60004_9C31_4A9C_97F7_017610D7072D__INCLUDED_)
#define AFX_SHEVTESTDOC_H__B3A60004_9C31_4A9C_97F7_017610D7072D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShevTestDoc : public CDocument
{
protected: // create from serialization only
	CShevTestDoc();
	DECLARE_DYNCREATE(CShevTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShevTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShevTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShevTestDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEVTESTDOC_H__B3A60004_9C31_4A9C_97F7_017610D7072D__INCLUDED_)
