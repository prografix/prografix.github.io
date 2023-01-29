// DemoApprox3d.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "DemoApprox3d.h"

#include "rand.h"
#include "approx3d.h"
#include "gl\gl.h"
#include "draw.h"
#include "DEMOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DemoApprox3d dialog


DemoApprox3d::DemoApprox3d(CWnd* pParent /*=NULL*/)
	: CDialog(DemoApprox3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(DemoApprox3d)
	m_appr_var = 0;
	m_points_num = 3;
	//}}AFX_DATA_INIT
}


void DemoApprox3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DemoApprox3d)
	DDX_CBIndex(pDX, IDC_COMBO_VAR, m_appr_var);
	DDX_CBIndex(pDX, IDC_POINTS_NUM, m_points_num);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DemoApprox3d, CDialog)
	//{{AFX_MSG_MAP(DemoApprox3d)
	ON_BN_CLICKED(IDC_NEXT_POINTS, OnNextPoints)
	ON_CBN_SELCHANGE(IDC_COMBO_VAR, OnSelchangeComboVar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DemoApprox3d message handlers

void DemoApprox3d::OnNextPoints() 
{
	UpdateData();
    static QRand2Vector3d qrv;
    poly.makeEllipsoid ( 4+m_points_num, 1, 1, 1, qrv );
    OnSelchangeComboVar();
}

void DemoApprox3d::OnSelchangeComboVar() 
{
	UpdateData();
    switch ( m_appr_var )
    {
    case 0: // Ўар
        {
            const Sphere3d fig = getSpherePlg ( poly );
            glNewList ( 1, GL_COMPILE );
            draw ( fig, 1, 0.7f, 0.3f, 1, VM_WIRE );
            draw ( poly, 0, 1, 1, 1, VM_WIRE );
            glEndList();
            glNewList ( 2, GL_COMPILE );
            draw ( fig, 1, 0.7f, 0.3f, 1, VM_SMOOTH );
            draw ( poly, 0, 1, 1, 1, VM_FLAT );
            glEndList();
            glNewList ( 3, GL_COMPILE );
            glCallList ( 2 );
            glEndList();
            break;
        }
    case 1: // Ёллипсоид
        {
            const Ellipsoid3d elli = getEllipsoidPlg ( poly );
            glNewList ( 1, GL_COMPILE );
            draw ( elli, 1, 0.7f, 0.3f, 1, VM_WIRE );
            draw ( poly, 0, 1, 1, 1, VM_WIRE );
            glEndList();
            glNewList ( 2, GL_COMPILE );
            draw ( elli, 1, 0.7f, 0.3f, 1, VM_SMOOTH );
            draw ( poly, 0, 1, 1, 1, VM_FLAT );
            glEndList();
            glNewList ( 3, GL_COMPILE );
            glCallList ( 2 );
            glEndList();
            break;
        }
    case 2: // ѕараллелепипед
        {
            const Cuboid3d para = getCuboid ( poly );
            glNewList ( 1, GL_COMPILE );
            draw ( para, 1, 0.7f, 0.3f, 1, VM_WIRE );
            draw ( poly, 0, 1, 1, 1, VM_WIRE );
            glEndList();
            glNewList ( 2, GL_COMPILE );
            draw ( para, 1, 0.7f, 0.3f, 1, VM_FLAT );
            draw ( poly, 0, 1, 1, 1, VM_FLAT );
            glEndList();
            glNewList ( 3, GL_COMPILE );
            glCallList ( 2 );
            glEndList();
            break;
        }
    }

    view->Invalidate(TRUE);
}
