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
    FixArray<nat, 2> cntr;
    cntr[0] = 4;
    cntr[1] = n;
    if ( poly.size() != n + 4 )
    {
        poly.resize ( n + 4 );
        poly[0].x = -1;
        poly[0].y = -1;
        poly[1].x =  1;
        poly[1].y = -1;
        poly[2].x =  1;
        poly[2].y =  1;
        poly[3].x = -1;
        poly[3].y =  1;
        randPolygon ( ArrRef<Vector2d> ( poly, 4, n ) ).reverse() *= 0.9;
        poly *= 0.9;
    }
    drawNewList2d ();
    Suite<nat> cntr2;
    Suite<nat> index;
    Suite<Vector2d> buf;
    switch ( m_trian_type )
    {
    case 1: 
        {
            Suite<Set3<nat> > res ( n - 2 );
            trianSweepLine ( cntr, poly, res );
            rebuildDelauney ( poly, res );
            for ( nat i = 0; i < res.size(); ++i )
            {
                const Set3<nat> & t = res[i];
                draw ( Segment2d ( poly[t.a], poly[t.b] ), 1, 1, 0 );
                draw ( Segment2d ( poly[t.b], poly[t.c] ), 1, 1, 0 );
                draw ( Segment2d ( poly[t.c], poly[t.a] ), 1, 1, 0 );
            }
            break;
        }
    case 2: 
        {
            convexParts ( cntr, poly, cntr2, index );
            nat k = 0;
            for ( nat i = 0; i < cntr2.size(); ++i )
            {
                buf.resize();
                nat k1 = k + cntr2[i];
                for ( nat j = k; j < k1; ++j ) buf.inc() = poly[index[j]];
                drawPolygon ( buf, 1, 1, 0 );
                k = k1;
            }
            break;
        }
    case 3: 
        {
            splitPolygon ( cntr, poly, cntr2, index );
            nat k = 0;
            for ( nat i = 0; i < cntr2.size(); ++i )
            {
                buf.resize();
                nat k1 = k + cntr2[i];
                for ( nat j = k; j < k1; ++j ) buf.inc() = poly[index[j]];
                drawPolygon ( buf, 1, 1, 0 );
                k = k1;
            }
            break;
        }
    }
    drawPolygon ( CArrRef<Vector2d> ( poly, 0, 4 ), 0, 1, 1 );
    drawPolygon ( CArrRef<Vector2d> ( poly, 4, n ), 0, 1, 1 );
    endNewList ();
}

void Trian2d::OnConvexParts() 
{
}
