// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ShevTest.h"

#include "MainFrm.h"
#include "ShevTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_MOV_XM, OnMovXm)
	ON_COMMAND(ID_MOV_XP, OnMovXp)
	ON_COMMAND(ID_MOV_YM, OnMovYm)
	ON_COMMAND(ID_MOV_YP, OnMovYp)
	ON_COMMAND(ID_MOV_ZM, OnMovZm)
	ON_COMMAND(ID_MOV_ZP, OnMovZp)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_ROT_X_CCW, OnRotXCcw)
	ON_COMMAND(ID_ROT_X_CW, OnRotXCw)
	ON_COMMAND(ID_ROT_Y_CCW, OnRotYCcw)
	ON_COMMAND(ID_ROT_Y_CW, OnRotYCw)
	ON_COMMAND(ID_ROT_Z_CCW, OnRotZCcw)
	ON_COMMAND(ID_ROT_Z_CW, OnRotZCw)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_dAngle = 45.;
    m_dScale = 1.2;
    m_dDist = 0.5;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnReset() 
{
    view->reset();	
}

void CMainFrame::OnRotXCcw() 
{
	view->rotate(m_dAngle,1,0,0);
}

void CMainFrame::OnRotXCw() 
{
	view->rotate(-m_dAngle,1,0,0);
}

void CMainFrame::OnRotYCcw() 
{
	view->rotate(m_dAngle,0,1,0);
}

void CMainFrame::OnRotYCw() 
{
	view->rotate(-m_dAngle,0,1,0);
}

void CMainFrame::OnRotZCcw() 
{
	view->rotate(m_dAngle,0,0,1);
}

void CMainFrame::OnRotZCw() 
{
	view->rotate(-m_dAngle,0,0,1);
}

void CMainFrame::OnZoomIn() 
{
	view->scale ( 1./m_dScale );
}

void CMainFrame::OnZoomOut() 
{
	view->scale ( m_dScale );
}

void CMainFrame::OnMovXm() 
{
	view->move ( -m_dDist, 0., 0. );
}

void CMainFrame::OnMovXp() 
{
	view->move ( m_dDist, 0., 0. );
}

void CMainFrame::OnMovYm() 
{
	view->move ( 0., -m_dDist, 0. );
}

void CMainFrame::OnMovYp() 
{
	view->move ( 0., m_dDist, 0. );
}

void CMainFrame::OnMovZm() 
{
	view->move ( 0., 0., -m_dDist );
}

void CMainFrame::OnMovZp() 
{
	view->move ( 0., 0., m_dDist );
}

void intersect2d_test();
void intersect3d_test();
void momentum2d_test();
void polyhedron_test();
void S2D_Model_test();
void approx2d_test();
void approx3d_test();
void compress_test();
void shevlist_test();
void vector2d_test();
void vector3d_test();
void complex_test();
void trian2d_test();
void filePYH_test();
void approx_test();
void func1d_test();
void func2d_test();
void func3d_test();
void func4d_test();
void matrix_test();
void linalg_test();
void opti1d_test();
void opti2d_test();
void opti2d_3d_test();
void opti3d_test();
void array_test();
void recon_test();
void calc_test();
void file_test();
void math_test();
void rand_test();
void tree_test();
void test();

void CMainFrame::OnTest() 
{
//    intersect3d_test();
//    momentum2d_test();
//    polyhedron_test();
//    S2D_Model_test();
//    approx3d_test();
//    testBlackRing();
//    compress_test();
//    shevlist_test();
    vector2d_test();
//    complex_test();
//    trian2d_test();
//    func2d_test();
//    opti3d_test();
//    matrix_test();
//    linalg_test();
//    recon_test();
//    array_test();
//    calc_test();
//    file_test();
//    math_test();
//    rand_test();
//    tree_test();
//    filePYH_test();
//    test();
}

//***************************************************************************//
// 
//  Метод Якоби нахождения собственных значений и собственных векторов
//                       для симметричных матриц
//    Входные данные:
//        n - размерность матрицы
//        a - исходная матрица. В процессе работы наддиагональные элементы
//            будут изменены, но их легко восстановить по поддиагональным
//    Выходные данные:
//        d - массив собственных значений
//        v - массив собственных векторов
//
//***************************************************************************//

#include <vector>

typedef unsigned int nat; // Натуральное число

void jacobi1 ( const nat n, double * const * a, double * d, double * const * v )
{
    if ( n == 0 ) return;
    std::vector<double> b ( n ), z ( n );
    nat i, j;
    for ( i = 0; i < n; ++i )
    {
        z[i] = 0.;
        b[i] = d[i] = a[i][i];
        for ( j = 0; j < n; ++j ) v[i][j] = i == j ? 1. : 0.;
    }
    for ( i = 0; i < 50; ++i )
    {
        double sm = 0.;
        nat p, q;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q ) sm += fabs ( a[p][q] );
        }
        if ( sm == 0 ) break;
        const double tresh = i < 3 ? 0.2 * sm / ( n*n ) : 0.;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q )
            {
                const double g = 1e12 * fabs ( a[p][q] );
                if ( i >= 3 && fabs ( d[p] ) > g && fabs ( d[q] ) > g ) a[p][q] = 0.;
                else
                if ( fabs ( a[p][q] ) > tresh )
                {
                    const double theta = 0.5 * ( d[q] - d[p] ) / a[p][q];
                    double t = 1. / ( fabs(theta) + sqrt(1.+theta*theta) );
                    if ( theta < 0 ) t = - t;
                    const double c = 1. / sqrt ( 1. + t*t );
                    const double s = t * c;
                    const double tau = s / ( 1. + c );
                    const double h = t * a[p][q];
                    z[p] -= h;
                    z[q] += h;
                    d[p] -= h;
                    d[q] += h;
                    a[p][q] = 0.;
                    for ( j = 0; j < p; ++j )
                    {
                        const double g = a[j][p];
                        const double h = a[j][q];
                        a[j][p] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = p+1; j < q; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[j][q];
                        a[p][j] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = q+1; j < n; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[q][j];
                        a[p][j] = g - s * ( h + g * tau );
                        a[q][j] = h + s * ( g - h * tau );
                    }
                    for ( j = 0; j < n; ++j )
                    {
                        const double g = v[j][p];
                        const double h = v[j][q];
                        v[j][p] = g - s * ( h + g * tau );
                        v[j][q] = h + s * ( g - h * tau );
                    }
                }
            }
        }
        for ( p = 0; p < n; ++p )
        {
            d[p] = ( b[p] += z[p] );
            z[p] = 0.;
        }
    }
}

//********************** 22.04.2018 ***************************//
//
//      Получение поворота совмещающего пары векторов
//
//********************** 22.04.2018 ***************************//

struct Quaternion
{
    double la, mu, nu, ro;
};

struct Vector3d
{
    double x, y, z;
};

bool makeQuaternion ( std::vector<std::pair<Vector3d, Vector3d>> & data, Quaternion & q )
{
    const nat n = data.size();
    if ( n < 2 ) return false;
    double xx = 0, xy = 0, xz = 0, yx = 0, yy = 0, yz = 0, zx = 0, zy = 0, zz = 0;
    for ( nat i = 0; i < n; ++i )
    {
        const Vector3d & a = data[i].first;
        const Vector3d & b = data[i].second;
        xx += a.x * b.x;
        xy += a.x * b.y;
        xz += a.x * b.z;
        yx += a.y * b.x;
        yy += a.y * b.y;
        yz += a.y * b.z;
        zx += a.z * b.x;
        zy += a.z * b.y;
        zz += a.z * b.z;
    }
    double a0[4], a1[4], a2[4], a3[4], *a[4];
    a[0] = a0;
    a[1] = a1;
    a[2] = a2;
    a[3] = a3;
    a[0][0] = xx - yy - zz;
    a[0][1] = xy + yx;
    a[0][2] = xz + zx;
    a[0][3] = yz - zy;
    a[1][1] = yy - zz - xx;
    a[1][2] = yz + zy;
    a[1][3] = zx - xz;
    a[2][2] = zz - yy - xx;
    a[2][3] = xy - yx;
    a[3][3] = xx + yy + zz;
    double d[4], v0[4], v1[4], v2[4], v3[4], *v[4];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    jacobi1 ( 4, a, d, v );
    double max = 0;
    for ( nat i = 0; i < 4; ++i )
    {
        if ( max >= d[i] ) continue;
        max = d[i];
        q.mu = v[0][i];
        q.mu = v[1][i];
        q.nu = v[2][i];
        q.ro = v[3][i];
    }
    if ( !max )
    {
        q.mu = 
        q.mu = 
        q.nu = 0;
        q.ro = 1;
    }
    return true;
}
