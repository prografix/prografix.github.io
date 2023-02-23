// GraphViewerView.cpp : implementation of the CGraphViewerView class
//

#include "stdafx.h"
#include "math.h"
#include "gl\gl.h"
#include "GraphViewer.h"
#include "GraphViewerDoc.h"
#include "GraphViewerView.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// CGraphViewerView

IMPLEMENT_DYNCREATE(CGraphViewerView, CView)

BEGIN_MESSAGE_MAP(CGraphViewerView, CView)
	//{{AFX_MSG_MAP(CGraphViewerView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerView construction/destruction

CGraphViewerView * view;

CGraphViewerView::CGraphViewerView()
{
    m_pDC = 0;
    view = this;
    button_is_down = false;
}

CGraphViewerView::~CGraphViewerView()
{
    delete m_pDC;
}

void SetTimer ( int uElapse )
{
    view->SetTimer ( 1, uElapse, 0 );
}

BOOL CGraphViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

int CGraphViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;
    m_pDC = new CClientDC(this);
    PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
          PFD_SUPPORT_OPENGL |          // support OpenGL
          PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // 8-bit alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        0,                              // 32-bit z-buffer
        0,                              // 8-bit stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixelformat;
    if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
    {
        MessageBox("ChoosePixelFormat failed");
        return -1;
    }
    if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
    {
        MessageBox("SetPixelFormat failed");
        return -1;
    }
    int n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
    m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	if ( wglMakeCurrent ( m_pDC->GetSafeHdc(), m_hrc ) == FALSE ) return -1;
	return 0;
}

void CGraphViewerView::refresh_glContext()
{
    KillTimer ( 1 );
    wglMakeCurrent ( 0, 0 );
    wglDeleteContext ( m_hrc );
    m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent ( m_pDC->GetSafeHdc(), m_hrc );
    glViewport(0, 0, sx, sy);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double dy = 1.;
    double dx = double(sx)/sy;
    glOrtho(-dx, dx, -dy, dy, -10., 10.);
    glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerView drawing

void CGraphViewerView::OnDraw(CDC* pDC)
{
	CGraphViewerDoc* pDoc = GetDocument();
    pDoc->draw();
    glFlush();
    SwapBuffers(wglGetCurrentDC());
}

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerView diagnostics

#ifdef _DEBUG
void CGraphViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphViewerDoc* CGraphViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphViewerDoc)));
	return (CGraphViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphViewerView message handlers

BOOL CGraphViewerView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CGraphViewerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
    if ( cy > 0 && cx > 0 )
    {
        sx = cx;
        sy = cy;
        glViewport(0, 0, cx, cy);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double dy = 1.;
        double dx = double(cx)/cy;
        glOrtho(-dx, dx, -dy, dy, -10., 10.);
        glMatrixMode(GL_MODELVIEW);
    }	
}

void CGraphViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnLButtonDown(nFlags, point);
}

void CGraphViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnLButtonUp(nFlags, point);
}

void CGraphViewerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnRButtonDown(nFlags, point);
}

void CGraphViewerView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnRButtonUp(nFlags, point);
}

void CGraphViewerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( button_is_down && point != m_point )
    {
    	double x = point.x - m_point.x;
	    double y = point.y - m_point.y;
    	if ( nFlags & MK_LBUTTON )
        {
            double t = sqrt ( x*x + y*y );
            rotate ( t, y/t, x/t, 0. );
        }
        else
    	if ( nFlags & MK_RBUTTON && sy > 0 )
        {
            double t = 2./sy;
            move ( x*t, -y*t, 0. );
        }
        m_point = point;
    }
	CView::OnMouseMove(nFlags, point);
}

void CGraphViewerView::OnTimer(UINT nIDEvent) 
{
	Invalidate(TRUE);
	CView::OnTimer(nIDEvent);
}

void CGraphViewerView::reset()
{
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    Invalidate(TRUE);
}

void CGraphViewerView::scale ( double s )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity();
    glScaled ( s, s, s );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CGraphViewerView::rotate ( double a, double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glRotated ( a, x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CGraphViewerView::move ( double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glTranslated ( x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}