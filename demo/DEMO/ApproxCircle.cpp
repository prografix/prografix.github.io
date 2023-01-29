// ApproxCircle.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "DEMO.h"
#include "ApproxCircle.h"

#include "ShevArray.h"
#include "Vector2d.h"
#include "approx2d.h"
#include "func2d.h"
#include "rand.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ApproxCircle dialog

const double M_PI = 3.14159265358979323846;

inline
void _draw ( const Def<Line2d> line, float r, float g, float b )
{
    if ( ! line.isDef ) return;
    const Vector2d p = 4. * line.norm.leftPerpendicular();
    const Vector2d o = -line.dist * line.norm;
    draw ( Segment2d ( o+p, o-p), r, g, b );
}

class Appr2d_data
{
    PRand rand;
    PNormalRand nrand;
    Suite<Vector2d> point, polygon1, polygon2;
    FixArray<Vector2d,64> ellipse;
public:
    explicit Appr2d_data ( nat n )
    {
        resize ( n );
    }
    void resize ( nat n )
    {
        point.resize ( n );
        polygon1.resize ( n );
        nextPolygon ( 2 );
    }
    void nextPolygon ( nat type )
    {
        if ( type < 2 ) return;
        const double coef1 = 2 * M_PI / polygon1.size();
        const double ratio = 0.5 + rand();
        const Vector2d shift ( 0.2 * ( rand() - 0.5 ), 0.1 * ( rand() - 0.5 ) ); 
        const Vector2d x ( cos ( 2 * M_PI * rand() ), sin ( 2 * M_PI * rand() ) );
        const Vector2d y ( x.leftPerpendicular() );
        nat i;
        for ( i = 0; i < polygon1.size(); ++i )
        {
            polygon1[i] = shift + ( x * cos(coef1*i) + y * sin(coef1*i) * ratio ) * ( 0.5 + 0.1 * rand() );
        }
        polygon2.resize ( 9 );
        randConvexPolygon ( polygon2 );
    }
    void picture ( nat type )
    {
        Conform2d conf;
        DynArray<Vector2d> temp;
        drawNewList2d ();
        switch ( type )
        {
        case 0: // аппроксимация точек точкой
            {
                nat i;
                for ( i = 0; i < point.size(); ++i )
                {
                    point[i].x = 2*rand() - 1;
                    point[i].y = 2*rand() - 1;
                }
                drawPoints ( point, 0, 1, 1 );
                drawCross ( getNearPoint1 ( point ), 1, 1, 0 );
                drawCross ( getNearPoint2 ( point ), 1, 0, 0 );
                drawCross ( getNearPointU ( point ), 0, 1, 0 );
            }
            break;
        case 1: // аппроксимация точек прямой
            {
                nat i;
                for ( i = 0; i < point.size(); ++i )
                {
                    double x = 2. * rand() - 1.;
                    double y = rand() - 0.5;
                    y *= 0.5 * fabs ( y );
                    point[i].x = x;
                    point[i].y = y;
                }
                FixArray<Vector2d,2> line;
                _draw ( getLine1 ( point ), 1, 1, 0 );
                _draw ( getLine2 ( point ), 1, 0, 0 );
                _draw ( getLineU ( point ), 0, 1, 0 );
                drawPoints ( point, 0, 1, 1 );
            }
            break;
        case 2: // аппроксимация точек окружностью
            {
                nat i;
                for ( i = 0; i < point.size(); ++i )
                {
                    point[i] = Vector2d ( rand(), rand() ).setNorm2 ( 1. - 2e-2 * nrand() );
                    point[i].x = 1.8 * ( point[i].x - 0.5 );
                    point[i].y = 1.8 * ( point[i].y - 0.5 );
                }
                draw ( getCirclePnt1 ( point ), 1, 1, 0 );
                draw ( getCirclePnt2 ( point ), 1, 0, 0 );
                drawPoints ( point, 0, 1, 1 );
            }
            break;
        case 3: // аппроксимация многоугольника кругом
            {
                drawPolygon ( polygon1, 0, 1, 1 );
                draw ( getCirclePlg ( polygon1 ), 1, 1, 0 );
            }
            break;
        case 4: // аппроксимация многоугольника эллипсом
            {
                drawPolygon ( polygon1, 0, 1, 1 );
                draw ( getEllipsePlg ( polygon1 ), 1, 1, 0 );
            }
            break;
        case 5: // аппроксимация многоугольника прямоугольником
            {
                drawPolygon ( polygon1, 0, 1, 1 );
                draw ( getRectanglePlg ( polygon1 ), 1, 1, 0 );
            }
            break;
        case 6: // аппроксимация многоугольника прямоугольником
            {
                drawPolygon ( polygon1, 0, 1, 1 );
                Def<Conform2d> conf = overlayConvexPolygons ( polygon2, polygon1 );
                drawPolygon ( DynArray<Vector2d> ( polygon2 ) *= conf, 1, 1, 0 );
            }
            break;
        case 7: // сплайн
            {
                nat i;
                DynArray<Vector2d> norm ( polygon1.size() );
                for ( i = 0; i < polygon1.size(); ++i )
                {
                    norm[i] = ( polygon1.cnext(i) - polygon1.cprev(i) ).rightPerpendicular();
                }
                for ( i = 0; i < polygon1.size(); ++i )
                {
                    Spline2d spline ( polygon1[i], polygon1.cnext(i), norm[i], norm.cnext(i) );
                    Vector2d a = spline.getPoint ( 0 );
                    for ( nat j = 1; j < 9; ++j )
                    {
                        Vector2d b = spline.getPoint ( 0.125 * j );
                        draw ( Segment2d ( a, b ), 1, 1, 0 );
                        a = b;
                    }
                }
                drawPoints ( polygon1, 0, 1, 1 );
            }
            break;
        }
        endNewList ();
    }
};

ApproxCircle::ApproxCircle(CWnd* pParent /*=NULL*/)
	: CDialog(ApproxCircle::IDD, pParent)
{
	//{{AFX_DATA_INIT(ApproxCircle)
	m_points_num = 3;
	m_appr_var = 0;
	//}}AFX_DATA_INIT
    data = new Appr2d_data ( m_points_num + 4 );
}

ApproxCircle::~ApproxCircle()
{
    delete data;
}

void ApproxCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ApproxCircle)
	DDX_CBIndex(pDX, IDC_POINTS_NUM, m_points_num);
	DDX_CBIndex(pDX, IDC_COMBO_VAR, m_appr_var);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ApproxCircle, CDialog)
	//{{AFX_MSG_MAP(ApproxCircle)
	ON_BN_CLICKED(IDC_NEXT_POINTS, OnNextPoints)
	ON_CBN_SELCHANGE(IDC_COMBO_VAR, OnSelchangeComboVar)
	ON_CBN_SELCHANGE(IDC_POINTS_NUM, OnSelchangePointsNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ApproxCircle message handlers

void ApproxCircle::OnNextPoints() 
{
	UpdateData();
    data->nextPolygon ( m_appr_var );
    data->picture ( m_appr_var );
}

void ApproxCircle::OnSelchangeComboVar() 
{
	UpdateData();
    data->picture ( m_appr_var );
}

void ApproxCircle::OnSelchangePointsNum() 
{
	UpdateData();
    data->resize ( m_points_num + 4 );
    data->picture ( m_appr_var );
}
