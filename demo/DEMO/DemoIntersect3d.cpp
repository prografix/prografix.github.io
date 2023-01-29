// DemoIntersect3d.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "DEMOView.h"
#include "DemoIntersect3d.h"

#include "gl\gl.h"
#include "draw.h"
#include "rand.h"
#include "func3d.h"
#include "Polyhedron.h"
#include "intersect3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect3d dialog

class Intersect3d_data
{
    Plane3d plane;
    Polyhedron poly1, poly2;
    QRand2Vector3d vrand;
    FixArray<Vector3d, 12> point;
public:
    Intersect3d_data ()
    {
        next1 (0);
        next2 (0);
    }
    void next1 ( int type1 )
    {
        convexHull ( point << vrand, poly1 );
    }
    void next2 ( int type2 )
    {
        plane.norm = vrand();
        plane.dist = 0.3*vrand().x;
    }
    void picture ( int type1, int type2 )
    {
        nat j;
        switch ( type1 )
        {
        case 0:
            switch ( type2 )
            {
            case 0:
                {
                    DynArray<DynArray<Vector3d> > res;
                    intersection ( poly1, plane, res );
                    glNewList ( 1, GL_COMPILE );
                    draw ( poly1, 0, 1, 1, 1, VM_WIRE );
                    for ( j = 0; j < res.size(); ++j ) drawPolygon ( res[j], 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    draw ( poly1, 0, 1, 1, 1, VM_FLAT );
                    glDisable ( GL_LIGHTING );
                    for ( j = 0; j < res.size(); ++j ) drawPolygon ( res[j], 1, 0, 0 );
                    glEndList();
                    glNewList ( 3, GL_COMPILE );
                    glCallList ( 2 );
                    glEndList();
                }
                break;
            case 1:
                {
                    cut ( poly1, plane, poly2 );
                    glNewList ( 1, GL_COMPILE );
                    draw ( poly1, 0, 1, 1, 1, VM_WIRE );
                    draw ( poly2, 1, 0, 1, 1, VM_WIRE );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    draw ( poly1, 0, 1, 1, 1, VM_FLAT );
                    draw ( poly2, 1, 0, 1, 1, VM_FLAT );
                    glDisable ( GL_LIGHTING );
                    glEndList();
                    glNewList ( 3, GL_COMPILE );
                    glCallList ( 2 );
                    glEndList();
                }
                break;
            }
            break;
        }
        view->Invalidate(TRUE);
    }
};


DemoIntersect3d::DemoIntersect3d(CWnd* pParent /*=NULL*/)
	: CDialog(DemoIntersect3d::IDD, pParent)
{
    data = new Intersect3d_data;
	//{{AFX_DATA_INIT(DemoIntersect3d)
	m_type1 = 0;
	m_type2 = 0;
	//}}AFX_DATA_INIT
}

DemoIntersect3d::~DemoIntersect3d()
{
    delete data;
}

void DemoIntersect3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DemoIntersect3d)
	DDX_CBIndex(pDX, IDC_COMBO_1, m_type1);
	DDX_CBIndex(pDX, IDC_COMBO_2, m_type2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DemoIntersect3d, CDialog)
	//{{AFX_MSG_MAP(DemoIntersect3d)
	ON_BN_CLICKED(ID_NEXT_1, OnNext1)
	ON_BN_CLICKED(ID_NEXT_2, OnNext2)
	ON_CBN_SELCHANGE(IDC_COMBO_2, OnSelchangeCombo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect3d message handlers

void DemoIntersect3d::OnNext1() 
{
	UpdateData();
    data->next1 ( m_type1 );
	data->picture ( m_type1, m_type2 );
}

void DemoIntersect3d::OnNext2() 
{
	UpdateData();
    data->next2 ( m_type2 );
	data->picture ( m_type1, m_type2 );
}

void DemoIntersect3d::OnSelchangeCombo2() 
{
	UpdateData();
	data->picture ( m_type1, m_type2 );
}
