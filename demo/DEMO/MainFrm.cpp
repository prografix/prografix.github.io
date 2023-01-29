// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DEMO.h"
#include "DEMODoc.h"
#include "DEMOView.h"
#include "MainFrm.h"
#include "ScenePar.h"
#include "RandomVec.h"
#include "CTrian2d.h"
#include "CTrian3d.h"
#include "LineX2d.h"
#include "ApproxCircle.h"
#include "MaxSphere.h"
#include "MaxCircle.h"
#include "Max2d3d.h"
#include "Min2d.h"
#include "Min3d.h"
#include "DemoApprox3d.h"
#include "DemoIntersect2d.h"
#include "DemoIntersect3d.h"
#include "DemoOther.h"

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
	ON_COMMAND(ID_SCENE, OnScene)
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
	ON_COMMAND(ID_DEMO_RANDOM_VEC, OnDemoRandomVectors)
	ON_COMMAND(ID_DEMO_LINES_X_2D, OnDemoLinesX2d)
	ON_COMMAND(ID_DEMO_MAXI_3D, OnDemoMaxi3d)
	ON_COMMAND(ID_DEMO_MINI_3D, OnDemoMini3d)
	ON_COMMAND(ID_DEMO_MAXI_2D, OnDemoMaxi2d)
	ON_COMMAND(ID_DEMO_MINI_2D, OnDemoMini2d)
	ON_COMMAND(ID_DEMO_APPROX_2D, OnDemoApprox2d)
	ON_COMMAND(ID_DEMO_APPROX_3D, OnDemoApprox3d)
	ON_COMMAND(ID_DEMO_INTERSECT_2D, OnDemoIntersect2d)
	ON_COMMAND(ID_DEMO_TRIAN_3D, OnDemoTrian3d)
	ON_COMMAND(ID_DEMO_TRIAN_2D, OnDemoTrian2d)
	ON_COMMAND(ID_DEMO_INTERSECT_3D, OnDemoIntersect3d)
	ON_COMMAND(ID_DEMO_OTHER, OnDemoOther)
	ON_COMMAND(ID_DEMO_MAXI_2D_3D, OnDemoMaxi2d3d)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_dAngle = 45.;
    m_dScale = 1.2;
    m_dDist = 0.5;
    rv_pnl = new RandomVec;
    trian2d_pnl = new Trian2d;
    trian3d_pnl = new Trian3d;
    lineX_pnl = new LineX2d;
    maxsph_pnl = new MaxSphere;
    maxcir_pnl = new MaxCircle;
    max2d3d_pnl = new Max2d3d;
    min2d_pnl = new Min2d;
    min3d_pnl = new Min3d;
    approx2d = new ApproxCircle;
    approx3d = new DemoApprox3d;
    intersect2d = new DemoIntersect2d;
    intersect3d = new DemoIntersect3d;
    other = new DemoOther;
}

CMainFrame::~CMainFrame()
{
    delete other;
    delete intersect3d;
    delete intersect2d;
    delete approx3d;
    delete approx2d;
    delete min3d_pnl;
    delete min2d_pnl;
    delete max2d3d_pnl;
    delete maxcir_pnl;
    delete maxsph_pnl;
    delete lineX_pnl;
    delete trian2d_pnl;
    delete trian3d_pnl;
    delete rv_pnl;
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

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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


void CMainFrame::OnScene() 
{
    CDEMODoc * doc = view->GetDocument();
	ScenePar scenePar;
    scenePar.m_dAngle = m_dAngle;
    scenePar.m_dMagnify = m_dScale;
    scenePar.m_dDistance = m_dDist;
    if ( scenePar.DoModal() == IDOK )
    {
        m_dAngle = scenePar.m_dAngle;
        m_dScale = scenePar.m_dMagnify;
        m_dDist = scenePar.m_dDistance;
    }
}

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

void CMainFrame::OnDemoRandomVectors() 
{
	rv_pnl->DestroyWindow();
	rv_pnl->Create(RandomVec::IDD, this);
}

void CMainFrame::OnDemoLinesX2d() 
{
	lineX_pnl->DestroyWindow();
	lineX_pnl->Create(LineX2d::IDD, this);
}

void CMainFrame::OnDemoMaxi3d() 
{
	maxsph_pnl->DestroyWindow();
	maxsph_pnl->Create(MaxSphere::IDD, this);
}

void CMainFrame::OnDemoMaxi2d() 
{
	maxcir_pnl->DestroyWindow();
	maxcir_pnl->Create(MaxCircle::IDD, this);
}

void CMainFrame::OnDemoMini2d() 
{
	min2d_pnl->DestroyWindow();
	min2d_pnl->Create(Min2d::IDD, this);
}

void CMainFrame::OnDemoMini3d() 
{
	min3d_pnl->DestroyWindow();
	min3d_pnl->Create(Min3d::IDD, this);
}

void CMainFrame::OnDemoApprox2d() 
{
	approx2d->DestroyWindow();
	approx2d->Create(ApproxCircle::IDD, this);
}

void CMainFrame::OnDemoApprox3d() 
{
	approx3d->DestroyWindow();
	approx3d->Create(DemoApprox3d::IDD, this);
}

void CMainFrame::OnDemoTrian2d() 
{
	trian2d_pnl->DestroyWindow();
	trian2d_pnl->Create(Trian2d::IDD, this);
}

void CMainFrame::OnDemoTrian3d() 
{
	trian3d_pnl->DestroyWindow();
	trian3d_pnl->Create(Trian3d::IDD, this);
}

void CMainFrame::OnDemoIntersect2d() 
{
	intersect2d->DestroyWindow();
	intersect2d->Create(DemoIntersect2d::IDD, this);
}

void CMainFrame::OnDemoIntersect3d() 
{
	intersect3d->DestroyWindow();
	intersect3d->Create(DemoIntersect3d::IDD, this);
}

void CMainFrame::OnDemoOther() 
{
	other->DestroyWindow();
	other->Create(DemoOther::IDD, this);
}

void CMainFrame::OnDemoMaxi2d3d() 
{
	max2d3d_pnl->DestroyWindow();
	max2d3d_pnl->Create(Max2d3d::IDD, this);
}
