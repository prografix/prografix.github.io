// GraphViewerDoc.cpp : implementation of the CGraphViewerDoc class
//

#include "stdafx.h"
#include "gl\gl.h"
#include "GraphViewer.h"
#include "GraphViewerDoc.h"
#include "GraphViewerView.h"
#include "GraphicInterface.h"
#include "CppFileWriter.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "GLDevice.h"
#include "GrClock.h"
#include "StlFileDlg.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// CGraphViewerDoc

IMPLEMENT_DYNCREATE(CGraphViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CGraphViewerDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_IMPORT_STL, OnImportStl)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_IMPORT_OBJ, OnImportObj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerDoc construction/destruction

CGraphViewerDoc::CGraphViewerDoc()
{
	grdata = 0;
    device = 0;
}

CGraphViewerDoc::~CGraphViewerDoc()
{
    delete grdata;
    delete device;
}

BOOL CGraphViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphViewerDoc serialization

void CGraphViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerDoc diagnostics

#ifdef _DEBUG
void CGraphViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerDoc commands

void CGraphViewerDoc::draw()
{
    if ( grdata )
    {
        *grdata >> *device;
    }
    else
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void CGraphViewerDoc::OnFileOpen() 
{
	CFileDialog temp ( TRUE, "gra", NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Graphical files (*.gra)|*.gra||" );
	if ( temp.DoModal() == IDOK )
	{
        OnOpenDocument ( temp.GetPathName() );
    }
}

void CGraphViewerDoc::set ( GraphicInterface * gi ) 
{
    delete grdata;
    grdata = gi;
	POSITION pos = GetFirstViewPosition();
	while ( pos != NULL )
	{
		CGraphViewerView * pView = (CGraphViewerView*) GetNextView(pos);
        pView->refresh_glContext();
        pView->Invalidate(TRUE);
	}
    delete device;
    device = gi ? new GLDevice : 0;
    grClock.begin();
    grClock.forward();
}

BOOL CGraphViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    FileReader * p = new FileReader ( lpszPathName );
    if ( p->size() > 0 )
    {
        set ( p );
        SetPathName(lpszPathName);
    }
    else
    {
        delete p;
        set ( 0 );
        SetTitle("");
    }
	return grdata != 0;
}

void CGraphViewerDoc::OnFileSave() 
{
    if ( grdata )
    {
	    CFileDialog temp ( FALSE, "gra", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Graphical files (*.gra)|*.gra||" );
	    if ( temp.DoModal() == IDOK )
	    {
            const char * comment = "GraphViewer";
            FileWriter file ( temp.GetPathName(), comment );
            *grdata >> file;
        }
    }
}

void CGraphViewerDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( grdata != 0 );
}

void CGraphViewerDoc::OnFileSaveAs() 
{
    if ( grdata )
    {
	    CFileDialog temp ( FALSE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
            "Graphical files (*.gra)|*.gra|C++ files (*.cpp)|*.cpp||" );
	    if ( temp.DoModal() == IDOK )
	    {
            if ( temp.m_ofn.nFilterIndex == 2 )
            {
                CppFileWriter writer ( temp.GetPathName() );
                *grdata >> writer;
            }
            else
	        {
                const char * comment = "GraphViewer";
                FileWriter file ( temp.GetPathName(), comment );
                *grdata >> file;
            }
        }
    }
}

void CGraphViewerDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( grdata != 0 );
}

GraphicInterface * importSTL ( const char * filename, const int surface );

void CGraphViewerDoc::OnImportStl() 
{
  	StlFileDlg temp;
    if ( temp.DoModal () == IDOK )
  	{
        CString filename = temp.GetPathName();
        GraphicInterface * p = importSTL ( filename, temp.m_iSurface );
        if ( p )
        {
            set ( p );
            SetPathName ( filename, 0 );
        }
    }
	
}

GraphicInterface * importOBJ ( const char * filename, const int surface );

void CGraphViewerDoc::OnImportObj() 
{
	CFileDialog temp ( TRUE, "obj", NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "WaveFront OBJ files (*.obj)|*.obj||" );
	if ( temp.DoModal() == IDOK )
	{
        CString filename = temp.GetPathName();
        GraphicInterface * p = importOBJ ( filename, 0 );
        if ( p )
        {
            set ( p );
            SetPathName ( filename, 0 );
        }
    }
}
