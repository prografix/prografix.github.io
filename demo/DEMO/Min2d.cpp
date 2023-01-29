// Min2d.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "DEMO.h"
#include "Min2d.h"

#include "Vector2d.h"
#include "rand.h"
#include "draw.h"
#include "func2d.h"
#include "opti2d.h"
#include "ShevArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Min2d dialog

class Min2d_data
{
    Suite<Vector2d> outer, inner;
    Suite<Circle2d> circle;
    Suite<Line2d> line;
    PRandPoint2d prand;
    PRand rand;
public:
    explicit Min2d_data ( nat nvo )
    {
        outer.resize ( nvo );
        randConvexPolygon ( outer );
    }
    void nextInner ( nat nvi )
    {
        line.resize ( nvi );
        inner.resize ( nvi );
        circle.resize ( nvi );
        for ( nat i = 0; i < nvi; ++i )
        {
            circle[i].o = inner[i] = 0.7 * prand();
            circle[i].r = 0.35 * rand();
            double t = rand();
            line[i].dist = 0.25 * t * t;
            double a = 2. * 3.14159265358979323846 * rand();
            Vector2d & v = line[i].norm;
            v.x = cos(a);
            v.y = sin(a);
        }
    }
    void nextOuter ( nat type, nat nvo )
    {
        switch ( type )
        {
        case 8:
            outer.resize ( nvo );
            randConvexPolygon ( outer );
            break;
        case 9:
            outer.resize ( nvo );
            regularPolygon ( outer );
            break;
        }
    }
    void picture ( nat intype, nat outtype )
    {
        nat i;
        DynArray<Vector2d> temp;
        drawNewList2d ();
        switch ( intype )
        {
        case 0:
            switch ( outtype )
            {
            case 0:
                draw ( minCircleAroundPoints ( inner ), 1, 0, 0 );
                break;
            case 1:
                draw ( minEllipseAroundPoints ( 0.5, inner ), 1, 0, 0 );
                break;
            case 2:
                draw ( minEllipseAroundPointsA ( inner ), 1, 0, 0 );
                break;
            case 3:
                draw ( minTriangleAroundPointsA ( inner ), 1, 0, 0 );
                break;
            case 4:
                draw ( minRectangleAroundPointsP ( inner ), 1, 1, 0 );
                draw ( minRectangleAroundPointsA ( inner ), 1, 0, 0 );
                break;
            case 5:
                draw ( minParallelogramAroundPointsA ( inner ), 1, 0, 0 );
                break;
            case 6:
                draw ( minRhombusAroundPointsP ( inner ), 1, 1, 0 );
                draw ( minRhombusAroundPointsA ( inner ), 1, 0, 0 );
                break;
            case 7:
                {
                    FixArray<Vector2d, 4> trapezoid;
                    minTrapezoidAroundPointsA ( trapezoid, inner );
                    temp = trapezoid;
                }
                break;
            case 8:
                {
                    Conform2d conf = maxPolygonInConvexPolygon ( inner, outer );
                    temp = outer;
                    temp *= Affin2d ( ~conf );
                }
                break;
            case 9:
                {
                    temp.resize ( outer.size() );
                    minEquianglarPolygonAroundPointsA ( temp, inner );
                }
                break;
            }
            drawPolygon ( temp, 1, 0, 0 );
            drawPoints ( inner, 0, 1, 1 );
            break;
        case 1:
            for ( i = 0; i < circle.size(); ++i )
            {
                float r = (float) rand();
                float g = (float) rand();
                float b = (float) rand();
                draw ( circle[i], r, g, b );
            }
            switch ( outtype )
            {
            case 0:
                draw ( minCircleAroundCircles ( circle ), 1, 0, 0 );
                break;
            default:
                AfxMessageBox ( "Вокруг окружностей можно описывать только окружность" );
            }
            break;
        case 2:
            for ( i = 0; i < line.size(); ++i )
            {
                const Vector2d o ( line[i].norm * -line[i].dist );
                const Vector2d p = 8. * line[i].norm.leftPerpendicular();
                draw ( Segment2d ( o - p, o + p ), 0, 1, 1 );
            }
            switch ( outtype )
            {
            case 0:
                draw ( minCircle ( line ), 1, 0, 0 );
                break;
            default:
                AfxMessageBox ( "Вокруг прямых можно описывать только окружность" );
            }
            break;
        }
        endNewList ();
    }
};

Min2d::Min2d(CWnd* pParent /*=NULL*/)
	: CDialog(Min2d::IDD, pParent)
{
	//{{AFX_DATA_INIT(Min2d)
	m_inner = 0;
	m_inner_nv = 4;
	m_outer = 0;
	m_outer_nv = 2;
	//}}AFX_DATA_INIT
    data = new Min2d_data ( m_outer_nv + 3 );
    data->nextInner ( m_inner_nv + 2 );
}

Min2d::~Min2d()
{
    delete data;
}

void Min2d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Min2d)
	DDX_CBIndex(pDX, IDC_COMBO_INNER_NV, m_inner_nv);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER, m_outer);
	DDX_CBIndex(pDX, IDC_COMBO_OUTER_NV, m_outer_nv);
	DDX_CBIndex(pDX, IDC_COMBO_INNER, m_inner);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Min2d, CDialog)
	//{{AFX_MSG_MAP(Min2d)
	ON_BN_CLICKED(ID_NEXT_INNER, OnNextInner)
	ON_BN_CLICKED(ID_NEXT_OUTER, OnNextOuter)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER, OnSelchangeComboOuter)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER, OnSelchangeComboInner)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER_NV, OnSelchangeComboInnerNv)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER_NV, OnSelchangeComboOuterNv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Min2d message handlers

void Min2d::OnNextInner() 
{
	UpdateData();
    data->nextInner ( m_inner_nv + 2 );
    data->picture ( m_inner, m_outer );
}

void Min2d::OnNextOuter() 
{
	UpdateData();
    data->nextOuter ( m_outer, m_outer_nv + 3 );
    data->picture ( m_inner, m_outer );
}

void Min2d::OnSelchangeComboOuter() 
{
	UpdateData();
    data->picture ( m_inner, m_outer );
}

void Min2d::OnSelchangeComboInner() 
{
	UpdateData();
    data->picture ( m_inner, m_outer );
}

void Min2d::OnSelchangeComboInnerNv() 
{
	OnNextInner();
}

void Min2d::OnSelchangeComboOuterNv() 
{
	OnNextOuter();
}
