// DEMODoc.cpp : implementation of the CDEMODoc class
//

#include "stdafx.h"
#include "DEMO.h"
#include "gl\gl.h"

#include "DEMODoc.h"
#include "DEMOView.h"
#include "ioOBJ.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEMODoc

IMPLEMENT_DYNCREATE(CDEMODoc, CDocument)

BEGIN_MESSAGE_MAP(CDEMODoc, CDocument)
	//{{AFX_MSG_MAP(CDEMODoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_MODEL_FLAT, OnModelFlat)
	ON_COMMAND(ID_MODEL_SMOOTH, OnModelSmooth)
	ON_COMMAND(ID_MODEL_WIRE, OnModelWire)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FLAT, OnUpdateModelFlat)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SMOOTH, OnUpdateModelSmooth)
	ON_UPDATE_COMMAND_UI(ID_MODEL_WIRE, OnUpdateModelWire)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEMODoc construction/destruction

CDEMODoc::CDEMODoc() : m_model_view(0)
{
}

CDEMODoc::~CDEMODoc()
{
}

BOOL CDEMODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDEMODoc serialization

void CDEMODoc::Serialize(CArchive& ar)
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
// CDEMODoc diagnostics

#ifdef _DEBUG
void CDEMODoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDEMODoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDEMODoc commands

void CDEMODoc::drawPolyhedron ()
{
    glNewList ( 1, GL_COMPILE );
    draw ( poly, 1, 1, 1, 1, VM_WIRE );
    glEndList();
    glNewList ( 2, GL_COMPILE );
    draw ( poly, 1, 1, 1, 1, VM_FLAT );
    glEndList();
    glNewList ( 3, GL_COMPILE );
    draw ( poly, 1, 1, 1, 1, VM_SMOOTH );
    glEndList();
}

///////////////////////////////////////////////////////////////////////////////

void CDEMODoc::OnFileOpen() 
{
	CFileDialog temp ( TRUE, 0, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       "*.obj;*.stl;*.pyh|*.obj;*.stl;*.pyh||" );
	if ( temp.DoModal() == IDOK )
	{
        OnOpenDocument ( temp.GetPathName() );
    }
}

bool loadPYH ( const char * filename, Polyhedron & poly );
bool loadSTL ( const char * filename, Polyhedron & poly );

BOOL CDEMODoc::OnOpenDocument ( LPCTSTR filename ) 
{
	CString name ( filename );
	name.MakeUpper();
	if ( name.Right(3) == "STL")
    {
        if ( loadSTL ( filename, poly ) == false ) return 0;
    }
    else
	if ( name.Right(3) == "OBJ")
    {
        if ( loadOBJ ( filename, poly ) == false ) return 0;
    }
    else
	if ( name.Right(3) == "PYH")
    {
        if ( loadPYH ( filename, poly ) == false ) return 0;
    }
    if ( poly.vertex.size() > 0 )
    {
        Vector3d min, max;
        min = max = poly.vertex[0];
	    for ( nat i = 1; i < poly.vertex.size(); ++i )
        {
            const Vector3d & v = poly.vertex[i];
            if ( min.x > v.x ) min.x = v.x;
            if ( min.y > v.y ) min.y = v.y;
            if ( min.z > v.z ) min.z = v.z;
            if ( max.x < v.x ) max.x = v.x;
            if ( max.y < v.y ) max.y = v.y;
            if ( max.z < v.z ) max.z = v.z;
        }
        const Vector3d o = -0.5 * ( min + max );
        const double d = norm2 ( max - min );
        glMatrixMode ( GL_MODELVIEW );
        glLoadIdentity ();
        if ( d > 0 )
        {
            const double s = 2.25 / d;
            glScaled ( s, s, s );
        }
        glTranslated ( o.x, o.y, o.z );
    }
    drawPolyhedron ();
    view->Invalidate ( TRUE );
    SetTitle ( filename );
	SetPathName ( filename );
    return 1;
}

bool saveSTLA ( const char * filename, const Polyhedron & poly );
bool saveSTLB ( const char * filename, const Polyhedron & poly );

void CDEMODoc::OnFileSaveAs() 
{
	CFileDialog temp ( FALSE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Wavefront OBJ files (*.obj)|*.obj|STL binary files (*.stl)|*.stl|STL text files (*.stl)|*.stl||" );
	if ( temp.DoModal() == IDOK )
	{
        const CString filename = temp.GetPathName();
        switch ( temp.m_ofn.nFilterIndex )
        {
        case 1: saveOBJ  ( filename, poly ); break;
        case 2: saveSTLB ( filename, poly ); break;
        case 3: saveSTLA ( filename, poly ); break;
        }
        SetTitle ( filename );
    }
}

void CDEMODoc::OnFileSave() 
{
	OnFileSaveAs();
}

void CDEMODoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( poly.vertex.size() );
}

void CDEMODoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( poly.vertex.size() );
}

void CDEMODoc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( 0 );
}

void CDEMODoc::OnModelWire() 
{
	m_model_view = 0;
    view->Invalidate(TRUE);
}

void CDEMODoc::OnModelFlat() 
{
	m_model_view = 1;
    view->Invalidate(TRUE);
}

void CDEMODoc::OnModelSmooth() 
{
	m_model_view = 2;
    view->Invalidate(TRUE);
}

void CDEMODoc::OnUpdateModelWire(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( m_model_view != 0 );
}

void CDEMODoc::OnUpdateModelFlat(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( m_model_view != 1 );
}

void CDEMODoc::OnUpdateModelSmooth(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( m_model_view != 2 );
}
