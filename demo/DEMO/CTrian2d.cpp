// Trian2d.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "CTrian2d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Trian2d dialog


Trian2d::Trian2d(CWnd* pParent /*=NULL*/)
	: CDialog(Trian2d::IDD, pParent)
{
	//{{AFX_DATA_INIT(Trian2d)
	m_poly_size = 3;
	m_trian_type = 0;
	m_global = FALSE;
	//}}AFX_DATA_INIT
}


void Trian2d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Trian2d)
	DDX_CBIndex(pDX, IDC_POLYGON_SIZE, m_poly_size);
	DDX_CBIndex(pDX, IDC_TRIAN_TYPE, m_trian_type);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_global);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Trian2d, CDialog)
	//{{AFX_MSG_MAP(Trian2d)
	ON_BN_CLICKED(IDC_NEXT_POLYGON, OnNextPolygon)
	ON_BN_CLICKED(IDC_TRIANGULATE, OnTriangulate)
	ON_BN_CLICKED(IDC_CONVEX_PARTS, OnConvexParts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Trian2d message handlers

#include "trian2d.h"
#include "func2d.h"
#include "ShevList.h"
#include "ShevArray.h"
#include "draw.h"

void Trian2d::OnNextPolygon() 
{
    poly.resize();
    OnTriangulate();
}

void Trian2d::OnTriangulate() 
{
	UpdateData();
    const nat n = m_poly_size + 4;
    if ( poly.size() != n )
    {
        poly.resize ( n );
        randPolygon ( poly );
    }
    drawNewList2d ();
    drawPolygon ( poly, 0, 1, 1 );
    Suite<Set3<nat> > res ( n - 2 );
    switch ( m_trian_type )
    {
    case 0: trianSweepLine ( poly, res ); rebuildDelauney ( poly, res ); break;
    case 1: //trianSeidel ( poly, res ); 
        break;
    }
    for ( nat i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & t = res[i];
        nat j = t.a + 1;
        if ( j == n ) j = 0;
        if ( t.b != j )
        {
            draw ( Segment2d ( poly[t.a], poly[t.b] ), 1, 1, 0 );
        }
        j = t.b + 1;
        if ( j == n ) j = 0;
        if ( t.c != j )
        {
            draw ( Segment2d ( poly[t.b], poly[t.c] ), 1, 1, 0 );
        }
        j = t.c + 1;
        if ( j == n ) j = 0;
        if ( t.a != j )
        {
            draw ( Segment2d ( poly[t.c], poly[t.a] ), 1, 1, 0 );
        }
    }
    endNewList ();
}

void Trian2d::OnConvexParts() 
{
}
