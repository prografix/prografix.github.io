// MaxSphere.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "DEMOView.h"
#include "MaxSphere.h"
#include "gl\gl.h"
#include "draw.h"
#include "rand.h"
#include "func2d.h"
#include "func3d.h"
#include "opti3d.h"
#include "opti2d_3d.h"
#include "Polyhedron.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// MaxSphere dialog

template<class T> inline 
void _draw ( const T & inner, const Polyhedron & outer, bool flat = false )
{
    glNewList ( 1, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, VM_WIRE );
    draw ( outer, 0, 1, 1, 1, VM_WIRE );
    glEndList();
    glNewList ( 4, GL_COMPILE );
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    draw ( outer, 0, 1, 1, 0.4f, VM_FLAT );
    glDisable ( GL_BLEND );
    glEndList();
    glNewList ( 2, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, VM_FLAT );
    glCallList ( 4 );
    glEndList();
    glNewList ( 3, GL_COMPILE );
    draw ( inner, 1, 0.7f, 0.3f, 1, flat ? VM_FLAT : VM_SMOOTH );
    glCallList ( 4 );
    glEndList();
}

class Max3d_data
{
    Polyhedron outer;
    QRand2Vector3d vrand;
    Suite<Vector3d> point;
public:
    Max3d_data ( nat np )
    {
        nextOuter ( np );
    }
    void nextOuter ( nat np )
    {
        point.resize(np) << vrand;
        convexHull ( point, outer );
    }
    void drawAll ( nat type )
    {
        switch ( type )
        {
        case 0:
            _draw ( maxSphereInConvexPolyhedron ( outer ), outer ); break;
        case 1:
            _draw ( maxFixCylinderInConvexPolyhedron ( outer, 1.5 ), outer ); break;
        case 2:
            _draw ( maxFixConeInConvexPolyhedron ( outer, 1.5 ), outer ); break;
        case 3:
            _draw ( maxFixSpheroidInConvexPolyhedron ( outer, 0.5 ), outer ); break;
        case 4:
            _draw ( maxCylinderInConvexPolyhedronV ( outer ), outer ); break;
        case 5:
            _draw ( maxConeInConvexPolyhedronV ( outer ), outer ); break;
        case 6:
            _draw ( maxSpheroidInConvexPolyhedronV ( outer ), outer ); break;
        case 7:
            _draw ( maxEllipsoidInConvexPolyhedronV ( outer ), outer ); break;
        case 8:
            _draw ( maxTetrahedronInConvexPolyhedronV ( outer ), outer ); break;
        case 9:
            _draw ( maxCuboidInConvexPolyhedronNR ( outer ), outer ); break;
        case 10:
            {
                Polyhedron inner;
                inner.makePrism ( regularPolygon ( FixArray<Vector2d, 6>() ), 0.4 );
                inner *= maxPolyhedronInConvexPolyhedronNR ( inner, outer );
                _draw ( inner, outer, true ); break;
            }
        case 11:
            {
                Polyhedron inner;
                inner.makePrism ( regularPolygon ( FixArray<Vector2d, 6>() ), 0.4 );
                inner *= maxPolyhedronInConvexPolyhedron1R ( inner, Vector3d(0,0,1), 6, outer );
                _draw ( inner, outer, true ); break;
            }
        case 12:
            {
                Polyhedron inner;
                inner.makePrism ( regularPolygon ( FixArray<Vector2d, 6>() ), 0.4 );
                inner *= maxPolyhedronInConvexPolyhedron ( inner, outer );
                _draw ( inner, outer, true ); break;
            }
        }
        view->Invalidate(TRUE);
    }
};

MaxSphere::MaxSphere(CWnd* pParent /*=NULL*/)
	: CDialog(MaxSphere::IDD, pParent)
{
	//{{AFX_DATA_INIT(MaxSphere)
	m_nvert = 5;
	m_inner = 0;
	//}}AFX_DATA_INIT
    data = new Max3d_data ( 4 + m_nvert );
}

MaxSphere::~MaxSphere()
{
    delete data;
}

void MaxSphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MaxSphere)
	DDX_CBIndex(pDX, IDC_COMBO_OUTER_NV, m_nvert);
	DDX_CBIndex(pDX, IDC_COMBO_INNER, m_inner);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MaxSphere, CDialog)
	//{{AFX_MSG_MAP(MaxSphere)
	ON_BN_CLICKED(ID_NEXT_OUTER, OnNextPoly)
	ON_CBN_SELCHANGE(IDC_COMBO_INNER, OnSelchangeComboInner)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTER_NV, OnSelchangeComboOuter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MaxSphere message handlers

void MaxSphere::OnNextPoly() 
{
	UpdateData();
    data->nextOuter ( 4 + m_nvert );
	data->drawAll ( m_inner );
}

void MaxSphere::OnSelchangeComboInner() 
{
	UpdateData();
	data->drawAll ( m_inner );
}

void MaxSphere::OnSelchangeComboOuter() 
{
	UpdateData();
    data->nextOuter ( 4 + m_nvert );
	data->drawAll ( m_inner );
}
