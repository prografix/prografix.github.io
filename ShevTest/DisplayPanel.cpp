// Display.cpp : implementation file
//

#include "Shev/File.h"
#include "Test/display.h"

#include "stdafx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Display dialog

class DisplayPanel : public CDialog
{
// Construction
public:
	DisplayPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DisplayPanel)
	enum { IDD = IDD_DISPLAY };
	CEdit	m_Message;
	CString	m_Buffer;
	BOOL	m_flag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DisplayPanel)
	protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DisplayPanel)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

DisplayPanel::DisplayPanel(CWnd* pParent /*=NULL*/)
	: CDialog(DisplayPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(DisplayPanel)
	m_Buffer = _T("");
	m_flag = FALSE;
	//}}AFX_DATA_INIT
}

void DisplayPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DisplayPanel)
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_MESSAGE, m_flag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DisplayPanel, CDialog)
	//{{AFX_MSG_MAP(DisplayPanel)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void DisplayPanel::OnClose() 
{
	CDialog::OnClose();
	m_Buffer.Empty();
	m_flag = 0;
}

void DisplayPanel::OnSize(UINT nType, int cx, int cy) 
{
    if ( ::IsWindow(m_Message.m_hWnd) )
    {
        m_Message.MoveWindow ( 0, 0, cx, cy );
    }
}

/////////////////////////////////////////////////////////////////////////////
// DisplayPanel message handlers

static DisplayPanel displayPanel;

#include "Shev/ShevArray.h"

class DisplayFile : public IWriteFile
{
    Suite<char> suite;
public:
    virtual unsigned int write(const void * p, const unsigned int size, const unsigned int count);
    virtual bool putc(const void * p);
    virtual void flush();
};

unsigned int DisplayFile::write(const void * ptr, const unsigned int size, const unsigned int count)
{
    if ( size <= 0 || count <= 0 ) return 0;
    const unsigned int n = size * count;
    const char * p = (const char *) ptr;
    for ( unsigned int i = 0; i < n; ++i )
    {
        putc ( p + i );
    }
    return count;
}

bool DisplayFile::putc(const void * ptr)
{
    const char c = *(char *) ptr;
    if ( c == '\n' )
    {
        if ( suite.size() == 0 || suite.las() != '\r' )
        {
            suite.inc() = '\r';
        }
    }
    suite.inc() = c;
    return true;
}

void DisplayFile::flush()
{
    if ( displayPanel.m_flag == 0 )
	{
		displayPanel.DestroyWindow ();
		displayPanel.Create ( DisplayPanel::IDD );
		displayPanel.m_flag = 1;
	}
    suite.inc() = 0;
	displayPanel.m_Buffer += suite();
	displayPanel.m_Message.SetWindowText ( displayPanel.m_Buffer );
	displayPanel.m_Message.LineScroll ( displayPanel.m_Message.GetLineCount() );
    suite.resize ( 0 );
}

static DisplayFile displayFile;
Display display ( displayFile );