// CTrian3d.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "tune.h"
#include "DEMO.h"
#include "CTrian3d.h"
#include "trian3d.h"
#include "draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Trian3d dialog


Trian3d::Trian3d(CWnd* pParent /*=NULL*/)
	: CDialog(Trian3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(Trian3d)
	m_N_vertex = 75;
	m_var = 0;
	//}}AFX_DATA_INIT
}


void Trian3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Trian3d)
	DDX_Text(pDX, IDC_N_VERTEX, m_N_vertex);
	DDV_MinMaxInt(pDX, m_N_vertex, 2, 200);
	DDX_CBIndex(pDX, IDC_COMBO_VAR, m_var);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Trian3d, CDialog)
	//{{AFX_MSG_MAP(Trian3d)
	ON_BN_CLICKED(ID_RUN, OnRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Trian3d message handlers

void Trian3d::OnRun() 
{
	UpdateData();
    const nat n = m_N_vertex;
    if ( n < 2 || n > 200 ) return;
    drawNewList2d ();
    if ( m_var == 0 )
    {
        CmbArray<Vector3d, 128> vert ( n );
        CmbArray<Set3<nat>, 126> res ( n - 2 );
        nat i;
        for ( i = 0; i < n; ++i )
        {
            const double a = i * M_2PI / n;
            Vector3d & v = vert[i];
            v.x = sin(a);
            v.y = cos(a);
            v.z = v.x * v.x - 0.5;
        }
        trian ( vert, res );
        for ( i = 0; i < res.size(); ++i )
        {
            const Set3<nat> & t = res[i];
            draw ( Segment3d ( vert[t.a], vert[t.b] ), 0, 1, 1 );
            draw ( Segment3d ( vert[t.b], vert[t.c] ), 0, 1, 1 );
            draw ( Segment3d ( vert[t.c], vert[t.a] ), 0, 1, 1 );
        }
    }
    else
    {
        CmbArray<Vector3d, 128> vert1 ( n ), vert2 ( n );
        CmbArray<Set3<nat>, 126> res ( n + n - 2 );
        nat i;
        for ( i = 0; i < n; ++i )
        {
            const double a = i * M_2PI / ( n - 1 );
            Vector3d & v1 = vert1[i];
            v1.x = ( a - M_PI ) / 3;
            v1.y = 2*(0.5 - sin(0.5*a));
            v1.z = -0.6;
            Vector3d & v2 = vert2[i];
            v2.x = 0.5*v1.x;
            v2.y = 0.5*v1.y;
            v2.z = -v1.z;
        }
        trian ( vert1, vert2, false, res );
        for ( i = 0; i < res.size(); ++i )
        {
            const Set3<nat> & t = res[i];
            const Vector3d & va = t.a >= n ? vert2[t.a-n] : vert1[t.a];
            const Vector3d & vb = t.b >= n ? vert2[t.b-n] : vert1[t.b];
            const Vector3d & vc = t.c >= n ? vert2[t.c-n] : vert1[t.c];
            draw ( Segment3d ( va, vb ), 0, 1, 1 );
            draw ( Segment3d ( vb, vc ), 0, 1, 1 );
            draw ( Segment3d ( vc, va ), 0, 1, 1 );
        }
        for ( i = 1; i < n; ++i )
        {
            draw ( Segment3d ( vert1[i], vert1[i-1] ), 1, 1, 0 );
            draw ( Segment3d ( vert2[i], vert2[i-1] ), 1, 0, 0 );
        }
    }
    endNewList ();
}
