// ShevTestDoc.cpp : implementation of the CShevTestDoc class
//

#include "stdafx.h"
#include "ShevTest.h"

#include "ShevTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShevTestDoc

IMPLEMENT_DYNCREATE(CShevTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CShevTestDoc, CDocument)
	//{{AFX_MSG_MAP(CShevTestDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShevTestDoc construction/destruction

CShevTestDoc::CShevTestDoc()
{
}

CShevTestDoc::~CShevTestDoc()
{
}

BOOL CShevTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CShevTestDoc serialization

void CShevTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShevTestDoc diagnostics

#ifdef _DEBUG
void CShevTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShevTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShevTestDoc commands
