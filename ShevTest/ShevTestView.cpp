// ShevTestView.cpp : implementation of the CShevTestView class
//

#include "stdafx.h"
#include "math.h"
#include "ShevTest.h"
#include "gl\gl.h"

#include "ShevTestDoc.h"
#include "ShevTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShevTestView

IMPLEMENT_DYNCREATE(CShevTestView, CView)

BEGIN_MESSAGE_MAP(CShevTestView, CView)
	//{{AFX_MSG_MAP(CShevTestView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShevTestView construction/destruction

CShevTestView * view;

CShevTestView::CShevTestView()
{
    m_pDC = 0;
    view = this;
}

CShevTestView::~CShevTestView()
{
    delete m_pDC;
}

BOOL CShevTestView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

int CShevTestView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
//	CShevTestDoc* pDoc = GetDocument();
//    pDoc->makePicture();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CShevTestView drawing

void CShevTestView::OnDraw(CDC* pDC)
{
//	CShevTestDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glCallList(1);
    glFlush();
    SwapBuffers(wglGetCurrentDC());
}

/////////////////////////////////////////////////////////////////////////////
// CShevTestView printing

BOOL CShevTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShevTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CShevTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CShevTestView diagnostics

#ifdef _DEBUG
void CShevTestView::AssertValid() const
{
	CView::AssertValid();
}

void CShevTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShevTestDoc* CShevTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShevTestDoc)));
	return (CShevTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShevTestView message handlers

void CShevTestView::OnSize(UINT nType, int cx, int cy) 
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

BOOL CShevTestView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CShevTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnLButtonDown(nFlags, point);
}

void CShevTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnLButtonUp(nFlags, point);
}

void CShevTestView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnRButtonDown(nFlags, point);
}

void CShevTestView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnRButtonUp(nFlags, point);
}

void CShevTestView::OnMouseMove(UINT nFlags, CPoint point) 
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

void CShevTestView::reset()
{
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    Invalidate(TRUE);
}

void CShevTestView::scale ( double s )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity();
    glScaled ( s, s, s );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CShevTestView::rotate ( double a, double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glRotated ( a, x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CShevTestView::move ( double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glTranslated ( x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}
