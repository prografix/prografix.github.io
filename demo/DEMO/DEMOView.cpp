// DEMOView.cpp : implementation of the CDEMOView class
//

#include "stdafx.h"
#include "DEMO.h"
#include "math.h"
#include "gl\gl.h"

#include "DEMODoc.h"
#include "DEMOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEMOView

IMPLEMENT_DYNCREATE(CDEMOView, CView)

BEGIN_MESSAGE_MAP(CDEMOView, CView)
	//{{AFX_MSG_MAP(CDEMOView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_SAVE_VIEW, OnFileSaveView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEMOView construction/destruction

CDEMOView * view;

CDEMOView::CDEMOView()
{
    m_pDC = 0;
    view = this;
}

CDEMOView::~CDEMOView()
{
    delete m_pDC;
}

BOOL CDEMOView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

int CDEMOView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

/////////////////////////////////////////////////////////////////////////////
// CDEMOView drawing

void CDEMOView::OnDraw(CDC* pDC)
{
	CDEMODoc* pDoc = GetDocument();
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    switch ( pDoc->m_model_view )
    {
    case 0:
        glDisable ( GL_LIGHT0 );
        glDisable ( GL_LIGHTING );
        glEnable  ( GL_CULL_FACE );
        glDisable ( GL_NORMALIZE );
        glEnable  ( GL_DEPTH_TEST );
        glDepthFunc ( GL_LEQUAL );
        glPolygonMode ( GL_FRONT, GL_LINE );
        glCallList ( 1 );
        break;
    case 1:
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glDisable( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );
        glCallList ( 2 );
        break;
    case 2:
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glDisable( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );
        glCallList ( 3 );
        break;
    }
    glFlush();
    SwapBuffers(wglGetCurrentDC());
}

/////////////////////////////////////////////////////////////////////////////
// CDEMOView diagnostics

#ifdef _DEBUG
void CDEMOView::AssertValid() const
{
	CView::AssertValid();
}

void CDEMOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDEMODoc* CDEMOView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDEMODoc)));
	return (CDEMODoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDEMOView message handlers

void CDEMOView::OnSize(UINT nType, int cx, int cy) 
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

BOOL CDEMOView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDEMOView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnLButtonDown(nFlags, point);
}

void CDEMOView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnLButtonUp(nFlags, point);
}

void CDEMOView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_point = point;
    button_is_down = true;
	CView::OnRButtonDown(nFlags, point);
}

void CDEMOView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	button_is_down = false;
	CView::OnRButtonUp(nFlags, point);
}

void CDEMOView::OnMouseMove(UINT nFlags, CPoint point) 
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

void CDEMOView::reset()
{
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    Invalidate(TRUE);
}

void CDEMOView::scale ( double s )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity();
    glScaled ( s, s, s );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CDEMOView::rotate ( double a, double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glRotated ( a, x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}

void CDEMOView::move ( double x, double y, double z )
{
    glMatrixMode ( GL_MODELVIEW );
    double data[16];
    glGetDoublev ( GL_MODELVIEW_MATRIX, data );
    glLoadIdentity ();
    glTranslated ( x, y, z );
    glMultMatrixd ( data );
    Invalidate(TRUE);
}


#include "io.h"
#include "fcntl.h"
#include "sys/stat.h"

bool saveBMP ( int nx, int ny, const unsigned char * map, const char * filename )
{
	const int h = _open ( filename, _O_TRUNC | _O_CREAT | _O_WRONLY | _O_BINARY, _S_IWRITE );
    if ( h == -1 ) return false;

	BITMAPINFOHEADER bih;
    bih.biSize = sizeof(BITMAPINFOHEADER); 
    bih.biWidth = nx; 
    bih.biHeight = ny; 
    bih.biPlanes = 1; 
    bih.biBitCount = 24;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 3 * nx * ny;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0; 
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

	BITMAPFILEHEADER hdr;
    hdr.bfType = 0x4d42;        /* 0x42 = "B" 0x4d = "M" */
    hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + bih.biSize;
    hdr.bfSize = hdr.bfOffBits + bih.biSizeImage;
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

	if ( _write ( h, &hdr, sizeof(BITMAPFILEHEADER) ) < 0 ||
	     _write ( h, &bih, sizeof(BITMAPINFOHEADER) ) < 0 ||
	     _write ( h, map, bih.biSizeImage ) < 0 )
	{
		_close ( h );
		remove ( filename );
		return false;
	}
	_close ( h );
	return true;
}

void CDEMOView::OnFileSaveView() 
{
	CFileDialog temp(FALSE,"bmp",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"BMP files (*.bmp)|*.bmp||");
	int ok = temp.DoModal();
	if ( ok == IDOK )
	{
	    CString filename = temp.GetPathName();
	    const int ny = sy;
   	    const int nx = ( sx / 4 ) * 4;
        unsigned char * p = new unsigned char[3*nx*ny];
	    glReadPixels( 0, 0, nx, ny, GL_BGR_EXT, GL_UNSIGNED_BYTE, p );
        if ( ! saveBMP ( nx, ny, p, filename ) )
        {
	        AfxMessageBox ( "Failed save view    " );
	    }
   	    delete[] p;
	}
}
