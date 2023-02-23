// ScenePar.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "ScenePar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ScenePar dialog


ScenePar::ScenePar(CWnd* pParent /*=NULL*/)
	: CDialog(ScenePar::IDD, pParent)
{
	//{{AFX_DATA_INIT(ScenePar)
	m_dAngle = 0.0;
	m_dDistance = 0.0;
	m_dMagnify = 0.0;
	//}}AFX_DATA_INIT
}


void ScenePar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ScenePar)
	DDX_Control(pDX, IDC_SDISTANCE, m_slDistance);
	DDX_Control(pDX, IDC_SMAGNIFY, m_slMagnify);
	DDX_Control(pDX, IDC_SANGLE, m_slAngle);
	DDX_Text(pDX, IDC_EANGLE, m_dAngle);
	DDX_Text(pDX, IDC_EDISTANCE, m_dDistance);
	DDX_Text(pDX, IDC_EMAGNIFY, m_dMagnify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ScenePar, CDialog)
	//{{AFX_MSG_MAP(ScenePar)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ScenePar message handlers

void ScenePar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar == (CScrollBar*)&m_slDistance )
    {
		UpdateData( TRUE );
		m_dDistance = 0.01 * m_slDistance.GetPos();
		UpdateData( FALSE );
	}
    else
	if( pScrollBar == (CScrollBar*)&m_slAngle )
    {
		UpdateData( TRUE );
		m_dAngle = 1.8 * m_slAngle.GetPos();
		UpdateData( FALSE );
	}
    else
	if( pScrollBar == (CScrollBar*)&m_slMagnify )
    {
		UpdateData( TRUE );
		m_dMagnify = 1. + 0.01 * m_slMagnify.GetPos();
		UpdateData( FALSE );
	}
	UpdateData( FALSE );
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL ScenePar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_slAngle.SetPos(int(m_dAngle/1.8+0.5));
	m_slMagnify.SetPos(int((m_dMagnify-1.)*100.+0.5));
	m_slDistance.SetPos(int(m_dDistance*100.+0.5));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
