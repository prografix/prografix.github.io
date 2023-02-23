// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "GraphViewerView.h"
#include "MainFrm.h"
#include "ScenePar.h"
#include "GrClock.h"

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
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_MOV_XM, OnMovXm)
	ON_COMMAND(ID_MOV_XP, OnMovXp)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_ROT_X_CCW, OnRotXCcw)
	ON_COMMAND(ID_ROT_X_CW, OnRotXCw)
	ON_COMMAND(ID_ROT_Y_CCW, OnRotYCcw)
	ON_COMMAND(ID_ROT_Y_CW, OnRotYCw)
	ON_COMMAND(ID_ROT_Z_CCW, OnRotZCcw)
	ON_COMMAND(ID_ROT_Z_CW, OnRotZCw)
	ON_COMMAND(ID_SCENE, OnScene)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_MOV_YM, OnMovYm)
	ON_COMMAND(ID_MOV_YP, OnMovYp)
	ON_COMMAND(ID_MOV_ZM, OnMovZm)
	ON_COMMAND(ID_MOV_ZP, OnMovZp)
	ON_COMMAND(ID_RUN_FORWARD, OnRunForward)
	ON_COMMAND(ID_RUN_BACK, OnRunBack)
	ON_COMMAND(ID_RUN_PAUSE, OnRunPause)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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

void CMainFrame::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}

void CMainFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}
/*
void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}
*/
void CMainFrame::OnScene() 
{
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

void CMainFrame::OnRunForward() 
{
	grClock.forward();
}

void CMainFrame::OnRunBack() 
{
	grClock.back();
}

void CMainFrame::OnRunPause() 
{
	grClock.pause();
}
