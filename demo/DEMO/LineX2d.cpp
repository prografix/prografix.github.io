// LineX2d.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "LineX2d.h"
#include "math.h"
#include "approx2d.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LineX2d dialog


LineX2d::LineX2d(CWnd* pParent /*=NULL*/)
	: CDialog(LineX2d::IDD, pParent)
{
	//{{AFX_DATA_INIT(LineX2d)
	m_line_num = 1;
	//}}AFX_DATA_INIT
}


void LineX2d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LineX2d)
	DDX_CBIndex(pDX, IDC_LINES_NUM, m_line_num);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LineX2d, CDialog)
	//{{AFX_MSG_MAP(LineX2d)
	ON_BN_CLICKED(IDC_NEXT_LINES, OnNextLines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LineX2d message handlers

void LineX2d::OnNextLines() 
{
	UpdateData();
    FixArray<Line2d, 7> line;
    const int n = m_line_num + 3;
    drawNewList2d ();
    for ( int i = 0; i < n; ++i )
    {
        double t = rand();
        line[i].dist = 0.25 * t * t;
        double a = 2. * 3.14159265358979323846 * rand();
        Vector2d & v = line[i].norm;
        v.x = cos(a);
        v.y = sin(a);
        const Vector2d o ( - line[i].dist * v );
        const Vector2d p = 8. * v.leftPerpendicular();
        draw ( Segment2d ( o - p, o + p ), 0, 0, 1 );
    }
    CArrRef<Line2d> ref ( line, 0, n );
    drawCross ( getNearPoint1 ( ref ), 1, 1, 0 );
    drawCross ( getNearPoint2 ( ref ), 1, 0, 0 );
    drawCross ( getNearPointU ( ref ), 0, 1, 0 );
    endNewList ();
}

