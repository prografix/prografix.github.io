// MaxCircle.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "DEMO.h"
#include "MaxCircle.h"

#include "func2d.h"
#include "opti2d.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MaxCircle dialog

inline
void makePolygon ( Suite<Vector2d> & poly, bool convex )
{
    if ( convex )
        randConvexPolygon ( poly );
    else
        randPolygon ( poly );
    Conform2d conf = standardPosition ( dimensions ( poly ) );
    conf.magn *= 1.8;
    conf.trans *= 1.8;
    poly *= Affin2d ( conf );
}

/////////////////////////////////////////////////////////////////////////////
// MaxCircle 

MaxCircle::MaxCircle(CWnd* pParent /*=NULL*/) : CDialog(MaxCircle::IDD, pParent)
{
	//{{AFX_DATA_INIT(MaxCircle)
	m_inner_nv = 0;
	m_outer_nv = 4;
	m_inner = 0;
	m_outer = 0;
	m_rotate = 1;
	//}}AFX_DATA_INIT
}

void MaxCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MaxCircle)
	DDX_CBIndex(pDX, IDC_COMBO_INNER_NV, m_inner_nv);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER_NV, m_outer_nv);
	DDX_CBIndex(pDX, IDC_COMBO_INNER, m_inner);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER, m_outer);
	DDX_CBIndex(pDX, IDC_COMBO_ROTATE, m_rotate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MaxCircle, CDialog)
	//{{AFX_MSG_MAP(MaxCircle)
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
// MaxCircle message handlers

void MaxCircle::OnNextOuter() 
{
	UpdateData();
    outer.resize ( 3 + m_outer_nv );
    makePolygon ( outer, m_outer == 0 );
    OnNextRun();
}

void MaxCircle::OnNextInner() 
{
	UpdateData();
    inner.resize ( 3 + m_inner_nv );
    regularPolygon ( inner );
    OnNextRun();
}

void MaxCircle::OnNextRun() 
{
    if ( outer.size() == 0 )
    {
	    UpdateData();
        outer.resize ( 3 + m_outer_nv );
        makePolygon ( outer, m_outer == 0 );
    }
    drawNewList2d ();
    drawPolygon ( outer, 0, 1, 1 );
    Def<Conform2d> conf;
    DynArray<Vector2d> poly;
    if ( isConvex ( outer ) )
    {
        switch ( m_inner )
        {
        case 0:
            {
                draw ( maxCircleInConvexPolygon ( outer ), 1, 0, 0 );
                break;
            }
        case 1:
            {
                if ( m_rotate )
                    draw ( maxFixEllipseInConvexPolygon ( 0.5, outer ), 1, 0, 0 );
                else
                    draw ( maxFixEllipseInConvexPolygonNR ( 2, outer ), 1, 0, 0 );
                break;
            }
        case 2:
            {
                if ( m_rotate )
                    draw ( maxEllipseInConvexPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxEllipseInConvexPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 3:
            {
                if ( m_rotate )
                    draw ( maxRectangleInConvexPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxRectangleInConvexPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 4:
            {
                if ( m_rotate )
                    draw ( maxRhombusInConvexPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxRhombusInConvexPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 5:
            {
                if ( m_rotate )
                    conf = maxPolygonInConvexPolygon ( inner, outer );
                else
                    conf = maxPolygonInConvexPolygonNR ( inner, outer );
                if ( conf.isDef )
                {
                    poly = inner;
                    poly *= conf;
                }
                else
        	        AfxMessageBox ( "Не получилось :-( " );
                break;
            }
        }
    }
    else
    {
        switch ( m_inner )
        {
        case 0:
            {
                draw ( maxCircleInPolygon ( outer ), 1, 0, 0 );
                break;
            }
        case 1:
            {
                if ( m_rotate )
                    draw ( maxFixEllipseInPolygon ( 0.5, outer ), 1, 0, 0 );
                else
                    draw ( maxFixEllipseInPolygonNR ( 2, outer ), 1, 0, 0 );
                break;
            }
        case 2:
            {
                if ( m_rotate )
                    draw ( maxEllipseInPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxEllipseInPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 3:
            {
                if ( m_rotate )
                    draw ( maxRectangleInPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxRectangleInPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 4:
            {
                if ( m_rotate )
                    draw ( maxRhombusInPolygonA ( outer ), 1, 0, 0 );
                else
                    draw ( maxRhombusInPolygonANR ( outer ), 1, 0, 0 );
                break;
            }
        case 5:
            {
                if ( m_rotate )
                    conf = maxConvexPolygonInPolygon ( inner, outer );
                else
                    conf = maxConvexPolygonInPolygonNR ( inner, outer );
                if ( conf.isDef )
                {
                    poly = inner;
                    poly *= conf;
                }
                else
        	        AfxMessageBox ( "Не получилось :-( " );
                break;
            }
        default:
	        AfxMessageBox ( "Этот вариант вписания не реализован " );
        }
    }
    drawPolygon ( poly, 1, 0, 0 );
    endNewList ();
}

void MaxCircle::OnSelchangeComboInner() 
{
    OnNextInner();
}

void MaxCircle::OnSelchangeComboInnerNv() 
{
    OnNextInner();
}

void MaxCircle::OnSelchangeComboOuter() 
{
    OnNextOuter();
}

void MaxCircle::OnSelchangeComboOuterNv() 
{
    OnNextOuter();
}

void MaxCircle::OnSelchangeComboRotate() 
{
    UpdateData();
    OnNextRun();
}
