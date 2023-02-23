// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "StlFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// StlFileDlg dialog


StlFileDlg::StlFileDlg() : m_iSurface(0),
CFileDialog ( 1, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "STL files(*.stl)|*.stl||", 0 )
{
    //{{AFX_DATA_INIT(CSpecialFileDialog)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_ofn.Flags |= OFN_ENABLETEMPLATE;
    m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_STLFILEDLG);
}


void StlFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StlFileDlg)
	DDX_Control(pDX, IDC_STL_SMOOTH, m_cbSurface);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StlFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(StlFileDlg)
	ON_CBN_SELCHANGE(IDC_STL_SMOOTH, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StlFileDlg message handlers

BOOL StlFileDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	m_cbSurface.AddString("Smooth");
	m_cbSurface.AddString("Flat");
	m_cbSurface.SetCurSel( 0 );
	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void StlFileDlg::OnSelchange() 
{
	UpdateData( TRUE );
    m_iSurface = m_cbSurface.GetCurSel();
}

BOOL StlFileDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CFileDialog::OnNotify(wParam, lParam, pResult);
}

