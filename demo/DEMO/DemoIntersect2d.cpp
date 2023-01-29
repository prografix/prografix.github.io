// DemoIntersect2d.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "DEMO.h"
#include "DemoIntersect2d.h"

#include "Vector2d.h"
#include "rand.h"
#include "draw.h"
#include "func2d.h"
#include "intersect2d.h"
#include "ShevArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect2d dialog

class Intersect2d_data
{
    Circle2d cir;
    Ellipse2d elli;
    Suite<Segment2d> seg;
    FixArray<Vector2d,4> quad;
    Suite<Vector2d> poly1, poly2, poly3;
    Segment2d seg1, seg2;
    PRandPoint2d rand;
public:
    explicit Intersect2d_data ( nat n1, nat n2 )
    {
//        outer.resize ( nvo );
//        randConvexPolygon ( outer );
        quad[0] = Vector2d ( 2, 2 );
        quad[1] = Vector2d (-2, 2 );
        quad[2] = Vector2d (-2,-2 );
        quad[3] = Vector2d ( 2,-2 );
        next1 ( n1 );
        next2 ( n2 );
    }
    void next1 ( nat n1 )
    {
        poly1.resize ( n1 );
        randPolygon ( poly1 );
        seg1.a = rand();
        seg1.b = rand();
        cir.o = 0.5 * seg1.a;
        cir.r = 0.3 + 0.5 * fabs ( seg1.b.x );
        elli.a = 0.2 + fabs ( cir.o.y );
        elli.b = 0.2 + fabs ( cir.o.x );
        elli.spin = Spin2d ( rand().x );
        elli.o = rand();
    }
    void next2 ( nat n2 )
    {
        poly2.resize ( n2 );
        randPolygon ( poly2 );
        poly3.resize ( n2 );
        randConvexPolygon ( poly3 );
        seg2.a = rand();
        seg2.b = rand();
    }
    void picture ( nat type1, nat type2 )
    {
        Conform2d conf;
        DynArray<Vector2d> temp;
        drawNewList2d ();
        switch ( type1 )
        {
        case 0:
            switch ( type2 )
            {
            case 0:
                {
                    const Line2d line1 ( seg1.a, seg1.b );
                    const Line2d line2 ( seg2.a, seg2.b );
                    if ( intersection ( quad, line1, seg ).size() > 0 ) draw ( seg[0], 1, 0, 0 );
                    if ( intersection ( quad, line2, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
                    const Def<Vector2d> p = intersection ( line1, line2 );
                    if ( p.isDef ) drawPoint ( p, 0, 1, 1 );
                }
                break;
            case 1:
                {
                    const Line2d line1 ( seg1.a, seg1.b );
                    if ( intersection ( quad, line1, seg ).size() > 0 ) draw ( seg[0], 1, 0, 0 );
                    draw ( seg2, 1, 1, 0 );
                    const Def<Vector2d> p = intersection ( line1, seg2 );
                    if ( p.isDef ) drawPoint ( p, 0, 1, 1 );
                }
                break;
            case 2:
                {
                    drawPolygon ( poly2, 1, 1, 0 );
                    const Line2d line1 ( seg1.a, seg1.b );
                    if ( intersection ( quad, line1, seg ).size() > 0 ) draw ( seg[0], 1, 0, 0 );
                    intersection ( line1, poly2, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            case 3:
                {
                    drawPolygon ( poly3, 1, 1, 0 );
                    const Line2d line1 ( seg1.a, seg1.b );
                    if ( intersection ( quad, line1, seg ).size() > 0 ) draw ( seg[0], 1, 0, 0 );
                    intersection ( line1, poly3, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            }
            break;
        case 1:
            switch ( type2 )
            {
            case 0:
                {
                    const Line2d line2 ( seg2.a, seg2.b );
                    draw ( seg1, 1, 0, 0 );
                    if ( intersection ( quad, line2, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
                    const Def<Vector2d> p = intersection ( seg1, line2 );
                    if ( p.isDef ) drawPoint ( p, 0, 1, 1 );
                }
                break;
            case 1:
                {
                    const Def<Vector2d> p = intersection ( seg1, seg2 );
                    draw ( seg1, 1, 0, 0 );
                    draw ( seg2, 1, 1, 0 );
                    if ( p.isDef ) drawPoint ( p, 0, 1, 1 );
                }
                break;
            case 2:
                {
                    drawPolygon ( poly2, 1, 1, 0 );
                    draw ( seg1, 1, 0, 0 );
                    intersection ( seg1, poly2, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            case 3:
                {
                    drawPolygon ( poly3, 1, 1, 0 );
                    draw ( seg1, 1, 0, 0 );
                    intersection ( seg1, poly3, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            }
            break;
        case 2:
            switch ( type2 )
            {
            case 0:
                {
                    draw ( cir, 1, 0, 0 );
                    const Line2d line2 ( seg2.a, seg2.b );
                    if ( intersection ( quad, line2, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
                    const Def<Segment2d> res = intersection ( cir, line2 );
                    draw ( res, 0, 1, 1 );
                }
                break;
            case 1:
                {
                    draw ( cir, 1, 0, 0 );
                    draw ( seg2, 1, 1, 0 );
                    const Def<Segment2d> res = intersection ( cir, seg2 );
                    draw ( res, 0, 1, 1 );
                }
                break;
            default:
                AfxMessageBox ( "Этот вариант пересечения не реализован" );
            }
            break;
        case 3:
            switch ( type2 )
            {
            case 0:
                {
                    draw ( elli, 1, 0, 0 );
                    const Line2d line2 ( seg2.a, seg2.b );
                    if ( intersection ( quad, line2, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
                    const Def<Segment2d> res = intersection ( elli, line2 );
                    draw ( res, 0, 1, 1 );
                }
                break;
            case 1:
                {
                    draw ( elli, 1, 0, 0 );
                    draw ( seg2, 1, 1, 0 );
                    const Def<Segment2d> res = intersection ( elli, seg2 );
                    draw ( res, 0, 1, 1 );
                }
                break;
            default:
                AfxMessageBox ( "Этот вариант пересечения не реализован" );
            }
            break;
        case 4:
            switch ( type2 )
            {
            case 0:
                {
                    drawPolygon ( poly1, 1, 0, 0 );
                    const Line2d line2 ( seg2.a, seg2.b );
                    if ( intersection ( quad, line2, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
                    intersection ( poly1, line2, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            case 1:
                {
                    drawPolygon ( poly1, 1, 0, 0 );
                    draw ( seg2, 1, 1, 0 );
                    intersection ( poly1, seg2, seg );
                    for ( nat i = 0; i < seg.size(); ++i ) draw ( seg[i], 0, 1, 1 );
                }
                break;
            case 2:
                {
                    drawPolygon ( poly1, 1, 0, 0 );
                    drawPolygon ( poly2, 1, 1, 0 );
                    Suite< Suite<Vector2d> > res; 
                    intersection ( poly2, poly1, res );
                    for ( nat i = 0; i < res.size(); ++i )
                    {
                        drawPolygon ( res[i], 0, 1, 1 );
                    }
                }
                break;
            case 3:
                {
                    drawPolygon ( poly1, 1, 0, 0 );
                    drawPolygon ( poly3, 1, 1, 0 );
                    Suite< Suite<Vector2d> > res; 
                    intersect1c ( poly3, poly1, res );
                    for ( nat i = 0; i < res.size(); ++i )
                    {
                        drawPolygon ( res[i], 0, 1, 1 );
                    }
                }
                break;
            }
        }
        endNewList ();
    }
};


DemoIntersect2d::DemoIntersect2d(CWnd* pParent /*=NULL*/)
	: CDialog(DemoIntersect2d::IDD, pParent)
{
	//{{AFX_DATA_INIT(DemoIntersect2d)
	m_type1 = 0;
	m_type2 = 0;
	m_nv1 = 1;
	m_nv2 = 0;
	//}}AFX_DATA_INIT
    data = new Intersect2d_data ( m_nv1 + 3, m_nv2 + 3 );
}

DemoIntersect2d::~DemoIntersect2d()
{
    delete data;
}

void DemoIntersect2d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DemoIntersect2d)
	DDX_CBIndex(pDX, IDC_COMBO_1_NV, m_nv1);
	DDX_CBIndex(pDX, IDC_COMBO_2_NV, m_nv2);
	DDX_CBIndex(pDX, IDC_COMBO_1, m_type1);
	DDX_CBIndex(pDX, IDC_COMBO_2, m_type2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DemoIntersect2d, CDialog)
	//{{AFX_MSG_MAP(DemoIntersect2d)
	ON_BN_CLICKED(ID_NEXT_1, OnNext1)
	ON_BN_CLICKED(ID_NEXT_2, OnNext2)
	ON_CBN_SELCHANGE(IDC_COMBO_1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_2, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO_1_NV, OnSelchangeCombo1Nv)
	ON_CBN_SELCHANGE(IDC_COMBO_2_NV, OnSelchangeCombo2Nv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DemoIntersect2d message handlers

void DemoIntersect2d::OnNext1() 
{
	UpdateData();
    data->next1 ( m_nv1 + 3 );
    data->picture ( m_type1, m_type2 );
}

void DemoIntersect2d::OnNext2() 
{
	UpdateData();
    data->next2 ( m_nv2 + 3 );
    data->picture ( m_type1, m_type2 );
}

void DemoIntersect2d::OnSelchangeCombo2() 
{
	UpdateData();
    data->picture ( m_type1, m_type2 );
}

void DemoIntersect2d::OnSelchangeCombo1() 
{
	UpdateData();
    data->picture ( m_type1, m_type2 );
}

void DemoIntersect2d::OnSelchangeCombo1Nv() 
{
	OnNext1();
}

void DemoIntersect2d::OnSelchangeCombo2Nv() 
{
	OnNext2();
}
