// RandomVec.cpp : implementation file
//

#include "stdafx.h"
#include "DEMO.h"
#include "RandomVec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RandomVec dialog


RandomVec::RandomVec(CWnd* pParent /*=NULL*/)
	: CDialog(RandomVec::IDD, pParent)
{
	//{{AFX_DATA_INIT(RandomVec)
	m_nVec = 1000;
	m_rand_type = 0;
	m_area_min = _T("");
	m_area_max = _T("");
	m_edge_min = _T("");
	m_edge_max = _T("");
	m_area_sigma = _T("");
	m_edge_sigma = _T("");
	//}}AFX_DATA_INIT
}


void RandomVec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RandomVec)
	DDX_Text(pDX, IDC_NVEC, m_nVec);
	DDX_CBIndex(pDX, IDC_COMBO_RAND, m_rand_type);
	DDX_Text(pDX, IDC_AREA_MIN, m_area_min);
	DDX_Text(pDX, IDC_AREA_MAX, m_area_max);
	DDX_Text(pDX, IDC_MIN, m_edge_min);
	DDX_Text(pDX, IDC_MAX, m_edge_max);
	DDX_Text(pDX, IDC_AREA_SIGMA, m_area_sigma);
	DDX_Text(pDX, IDC_EDGE_SIGMA, m_edge_sigma);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RandomVec, CDialog)
	//{{AFX_MSG_MAP(RandomVec)
	ON_BN_CLICKED(ID_RUN, OnRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RandomVec message handlers

#include "math.h"
#include "rand.h"
#include "func3d.h"
#include "DEMODoc.h"
#include "DEMOView.h"
#include "ShevArray.h"

double getSigma ( CCArrRef<double> & data )
{
    const nat n = data.size();
    double avg = amean ( data ), sigma = 0;
    for ( nat i = 0; i < n; ++i )
    {
        sigma += _pow2 ( data[i] - avg );
    }
    return sqrt ( sigma / n );
}

void RandomVec::OnRun() 
{
	UpdateData();
    if ( m_nVec < 10 )
    {
        m_nVec = 10;
	    UpdateData(FALSE);
    }
    else
    if ( m_nVec > 90000 )
    {
        m_nVec = 90000;
	    UpdateData(FALSE);
    }
    DynArray<Vector3d> v ( m_nVec );
    switch ( m_rand_type )
    {
    case 0:
        v << PRandVector3d(); break;
    case 1:
        v << QRandVector3d(); break;
    case 2:
        v << QRand2Vector3d(); break;
    }
    convexHull ( v, poly );

    double max_facet, min_facet, max_edge, min_edge;
	max_edge = max_facet = 0.0;
	min_edge = min_facet = 1e300;
    Suite<double> edge, area;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & f = poly.facet[i];
        CArrRef<nat> p = f.index;
        for ( nat j = 0; j < f.nv; ++j )
        {
            double t = sqrt ( qmod ( poly.vertex[p[j]] - poly.vertex[p[j+1]] ) );
            if ( max_edge < t ) max_edge = t;
            if ( min_edge > t ) min_edge = t;
            edge.inc() = t;
        }
        double t = f.getArea ( poly.vertex );
        if ( max_facet < t ) max_facet = t;
        if ( min_facet > t ) min_facet = t;
        area.inc() = t;
    }
    if ( min_edge == 1e300 ) min_edge = 0.;
    m_edge_min.Format ( "%f", min_edge );
    m_edge_max.Format ( "%f", max_edge );
    m_edge_sigma.Format ( "%f", getSigma ( edge ) );
    if ( min_facet == 1e300 ) min_facet = 0.;
    m_area_min.Format ( "%f", min_facet );
    m_area_max.Format ( "%f", max_facet );
    m_area_sigma.Format ( "%f", getSigma ( area ) );
	UpdateData(FALSE);

    CDEMODoc * doc = view->GetDocument();
    _swap ( doc->poly, poly );
    doc->drawPolyhedron();
    view->Invalidate(TRUE);
}
