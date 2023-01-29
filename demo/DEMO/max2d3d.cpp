// Max2d3d.cpp : implementation file
//

#include "stdafx.h"
#include "gl\gl.h"
#include "demo.h"
#include "DEMOView.h"
#include "Max2d3d.h"
#include "opti2d_3d.h"
#include "func2d.h"
#include "func3d.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Max2d3d dialog

template<class T> inline 
void _draw ( const T & inner, const Polyhedron & outer, bool flat = false )
{
    glNewList ( 1, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, VM_WIRE );
    draw ( outer, 0, 1, 1, 1, VM_WIRE );
    glEndList();
    glNewList ( 4, GL_COMPILE );
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    draw ( outer, 0, 1, 1, 0.4f, VM_FLAT );
    glDisable ( GL_BLEND );
    glEndList();
    glNewList ( 2, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, VM_FLAT );
    glCallList ( 4 );
    glEndList();
    glNewList ( 3, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, flat ? VM_FLAT : VM_SMOOTH );
    glCallList ( 4 );
    glEndList();
}

Max2d3d::Max2d3d(CWnd* pParent /*=NULL*/) : CDialog(Max2d3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(Max2d3d)
	m_inner_nv = 0;
	m_outer_nv = 4;
	m_inner = 0;
	m_outer = 0;
	m_rotate = 1;
	//}}AFX_DATA_INIT
}

void Max2d3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Max2d3d)
	DDX_CBIndex(pDX, IDC_COMBO_INNER_NV, m_inner_nv);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER_NV, m_outer_nv);
	DDX_CBIndex(pDX, IDC_COMBO_INNER, m_inner);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER, m_outer);
	DDX_CBIndex(pDX, IDC_COMBO_ROTATE, m_rotate);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Max2d3d message handlers

BEGIN_MESSAGE_MAP(Max2d3d, CDialog)
	//{{AFX_MSG_MAP(Max2d3d)
	ON_BN_CLICKED(ID_NEXT_RUN, OnNextRun)
	ON_BN_CLICKED(ID_NEXT_OUTER, OnNextOuter)
	ON_BN_CLICKED(ID_NEXT_INNER, OnNextInner)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER, OnSelchangeComboInner)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER_NV, OnSelchangeComboInnerNv)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER, OnSelchangeComboOuter)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER_NV, OnSelchangeComboOuterNv)
	ON_CBN_SELCHANGE(IDC_COMBO_ROTATE, OnSelchangeComboRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Max2d3d2 dialog


Max2d3d2::Max2d3d2(CWnd* pParent /*=NULL*/)
	: CDialog(Max2d3d2::IDD, pParent)
{
	//{{AFX_DATA_INIT(Max2d3d2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Max2d3d2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Max2d3d2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Max2d3d2, CDialog)
	//{{AFX_MSG_MAP(Max2d3d2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Max2d3d2 message handlers

void Max2d3d::OnNextOuter() 
{
	UpdateData();
    point.resize ( 4 + m_outer_nv ) << vrand;
    convexHull ( point, outer );
    OnNextRun();
}

void Max2d3d::OnNextInner() 
{
	UpdateData();
    inner.resize ( 3 + m_inner_nv );
    regularPolygon ( inner );
    OnNextRun();
}

void Max2d3d::OnNextRun() 
{
    switch ( m_inner )
    {
    case 0:
        _draw ( maxCircleInConvexPolyhedron ( outer ), outer ); break;
    case 1:
        _draw ( maxTriangleInConvexPolyhedronA ( outer ), outer ); break;
    case 2:
        _draw ( maxParallelogramInConvexPolyhedronA ( outer ), outer ); break;
    case 3:
        _draw ( maxRectangleInConvexPolyhedronA ( outer ), outer ); break;
    case 4:
        _draw ( maxRhombusInConvexPolyhedronA ( outer ), outer ); break;
    }
    view->Invalidate(TRUE);
}

void Max2d3d::OnSelchangeComboInner() 
{
    OnNextInner();
}

void Max2d3d::OnSelchangeComboInnerNv() 
{
    OnNextInner();
}

void Max2d3d::OnSelchangeComboOuter() 
{
    OnNextOuter();
}

void Max2d3d::OnSelchangeComboOuterNv() 
{
    OnNextOuter();
}

void Max2d3d::OnSelchangeComboRotate() 
{
    UpdateData();
    OnNextRun();
}
