// Min3d.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "Min3d.h"
#include "gl\gl.h"
#include "DEMOView.h"

#include "math.h"
#include "draw.h"
#include "rand.h"
#include "func3d.h"
#include "opti3d.h"
#include "Vector3d.h"
#include "Vector3f.h"
#include "ShevArray.h"
#include "Polyhedron.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Min3d dialog

class Min3d_data
{
    CmbSuite<Vector3d, 9> inner;
    CmbSuite<Sphere3d, 9> spheres;
    CmbSuite<  Line3d, 9> line;
    PRandVector3d vrand;
    PRandPoint3d prand;
    PRand rand;
public:
    Min3d_data ()
    {
        nextInner ( 4 );
    }
    void nextInner ( nat nvi )
    {
        line.resize ( nvi );
        inner.resize ( nvi );
        spheres.resize ( nvi );
        for ( nat i = 0; i < nvi; ++i )
        {
            spheres[i].o = inner[i] = 0.7 * prand();
            spheres[i].r = 0.35 * rand();
            line[i].dir = vrand();
            line[i].point = inner[i] + 0.2 * vrand();
        }
    }
    void draw ( nat intype, nat outtype )
    {
        switch ( intype )
        {
        case 0:
            switch ( outtype )
            {
            case 0:
                {
                    const Sphere3d sphere = minSphereAroundPoints ( inner );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( sphere, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( sphere, 0, 1, 1, 1, VM_SMOOTH );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            case 1:
                {
                    const Ellipsoid3d elli = minEllipsoidV ( inner );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( elli, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( elli, 0, 1, 1, 1, VM_SMOOTH );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            case 2:
                {
                    const Cylinder3d fig = minCylinderR ( inner );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( fig, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( fig, 0, 1, 1, 1, VM_SMOOTH );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            case 3:
                {
                    Polyhedron poly;
                    minRegularTetrahedronAroundPoints ( inner, poly );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( poly, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( poly, 0, 1, 1, 1, VM_FLAT );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            case 4:
                {
                    const Cuboid3d para = minCuboidAroundPointsNR ( inner );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( para, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( para, 0, 1, 1, 1, VM_FLAT );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            case 5:
                {
                    Polyhedron poly;
                    const Cuboid3d para = minCuboidAroundConvexPolyhedronV ( convexHull ( inner, poly ) );
                    glNewList ( 1, GL_COMPILE );
                    ::draw ( para, 0, 1, 1, 1, VM_WIRE );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    ::draw ( para, 0, 1, 1, 1, VM_FLAT );
                    drawPoints ( inner, 1, 0, 0 );
                    glEndList();
                }
                break;
            }
            break;
        case 1:
            switch ( outtype )
            {
            case 0:
                {
                    nat i;
                    DynArray<Vector3f> color ( spheres.size() );
                    const Sphere3d sphere = minSphereAroundSpheres ( spheres );
                    glNewList ( 1, GL_COMPILE );
                    for ( i = 0; i < spheres.size(); ++i )
                    {
                        color[i].x = (float) rand();
                        color[i].y = (float) rand();
                        color[i].z = (float) rand();
                        ::draw ( spheres[i], color[i].x, color[i].y, color[i].z, 1, VM_WIRE );
                    }
                    ::draw ( sphere, 0, 1, 1, 1, VM_WIRE );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    for ( i = 0; i < spheres.size(); ++i )
                    {
                        ::draw ( spheres[i], color[i].x, color[i].y, color[i].z, 1, VM_SMOOTH );
                    }
                    glEnable ( GL_BLEND );
                    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                    ::draw ( sphere, 0, 1, 1, 0.6f, VM_SMOOTH );
                    glDisable ( GL_BLEND );
                    glEndList();
                }
                break;
            default:
                AfxMessageBox ( "Вокруг сфер можно описывать только сферу" );
            }
            break;
        case 2:
            switch ( outtype )
            {
            case 0:
                {
                    nat i;
                    const Sphere3d sphere = minSphere ( line );
                    glNewList ( 1, GL_COMPILE );
                    for ( i = 0; i < line.size(); ++i )
                    {
                        const Segment3d seg ( line[i].point + 9 * line[i].dir, 
                                              line[i].point - 9 * line[i].dir );
                        ::draw ( seg, 1, 1, 0 );
                    }
                    ::draw ( sphere, 0, 1, 1, 1, VM_WIRE );
                    glEndList();
                    glNewList ( 2, GL_COMPILE );
                    for ( i = 0; i < line.size(); ++i )
                    {
                        const Segment3d seg ( line[i].point + 9 * line[i].dir, 
                                              line[i].point - 9 * line[i].dir );
                        ::draw ( seg, 1, 1, 0 );
                    }
                    ::draw ( sphere, 0, 1, 1, 1, VM_SMOOTH );
                    glEndList();
                }
                break;
            default:
                AfxMessageBox ( "Вокруг прямых можно описывать только сферу" );
            }
            break;
        }
        glNewList ( 3, GL_COMPILE );
        glCallList ( 2 );
        glEndList();
        view->Invalidate(TRUE);
    }
};


Min3d::Min3d(CWnd* pParent /*=NULL*/)
	: CDialog(Min3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(Min3d)
	m_inner_nv = 0;
	m_combo_outer = 0;
	m_combo_inner = 0;
	//}}AFX_DATA_INIT
    data = new Min3d_data ();
}

Min3d::~Min3d()
{
    delete data;
}


void Min3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Min3d)
	DDX_CBIndex(pDX, IDC_COMBO_INNER_NV, m_inner_nv);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER, m_combo_outer);
	DDX_CBIndex(pDX, IDC_COMBO_INNER, m_combo_inner);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Min3d, CDialog)
	//{{AFX_MSG_MAP(Min3d)
	ON_BN_CLICKED(ID_NEXT_INNER, OnNextInner)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER, OnSelchangeComboOuter)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER_NV, OnSelchangeComboInnerNv)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER, OnSelchangeComboInner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Min3d message handlers

void Min3d::OnNextInner() 
{
	UpdateData();
    data->nextInner ( m_inner_nv + 4 );
    data->draw ( m_combo_inner, m_combo_outer );	
}

void Min3d::OnSelchangeComboOuter() 
{
	UpdateData();
    data->draw ( m_combo_inner, m_combo_outer );
}

void Min3d::OnSelchangeComboInnerNv() 
{
    OnNextInner();
}

void Min3d::OnSelchangeComboInner() 
{
    OnNextInner();
}
