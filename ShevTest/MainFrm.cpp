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

void CMainFrame::OnTest() 
{
//    intersect3d_test();
//    momentum2d_test();
//    polyhedron_test();
//    S2D_Model_test();
//    approx_test();
//    testBlackRing();
//    compress_test();
//    shevlist_test();
//    vector2d_test();
//    complex_test();
//    trian2d_test();
//    func2d_test();
//    opti2d_3d_test();
//    matrix_test();
//    linalg_test();
//    recon_test();
//    array_test();
    calc_test();
//    file_test();
//    math_test();
//    rand_test();
//    tree_test();
//    filePYH_test ();
}
