// DemoOther.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DemoOther.h"
#include "Poly3gon.h"
#include "Vector3f.h"
#include "RealFile.h"
#include "fileSTL.h"
#include "func1t.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DemoOther dialog


DemoOther::DemoOther(CWnd* pParent /*=NULL*/)
	: CDialog(DemoOther::IDD, pParent)
{
	//{{AFX_DATA_INIT(DemoOther)
	m_var = 0;
	//}}AFX_DATA_INIT
}


void DemoOther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DemoOther)
	DDX_CBIndex(pDX, IDC_COMBO1, m_var);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DemoOther, CDialog)
	//{{AFX_MSG_MAP(DemoOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DemoOther message handlers

nat markSides ( CArrRef< Trigon<Vector3f> > sides, DynArrRef<nat> & mark )
{
    const nat n = sides.size();
    mark.resize ( n );
    if ( n < 2 )
    {
        mark.fill ( 1 );
        return n;
    }
    mark.fill ( 0 );
    Suite< SortItem <Set2<nat>, nat> > suite12(n+n);
    Suite< SortItem <Set2<nat>, nat> > suite21(n+n);
    nat i, i1 = 0, i2 = 0;
    for ( i = 0; i < sides.size(); ++i )
    {
        const Trigon<Vector3f> & t = sides[i];
        SortItem < Set2<nat>, nat> e;
        e.tail = i;
        if ( t.a < t.b )
        {
            e.head.a = t.a;
            e.head.b = t.b;
            suite12.inc() = e;
        }
        else
        {
            e.head.a = t.b;
            e.head.b = t.a;
            suite21.inc() = e;
        }
        if ( t.b < t.c )
        {
            e.head.a = t.b;
            e.head.b = t.c;
            suite12.inc() = e;
        }
        else
        {
            e.head.a = t.c;
            e.head.b = t.b;
            suite21.inc() = e;
        }
        if ( t.c < t.a )
        {
            e.head.a = t.c;
            e.head.b = t.a;
            suite12.inc() = e;
        }
        else
        {
            e.head.a = t.a;
            e.head.b = t.c;
            suite21.inc() = e;
        }
    }
    quickSort123 ( suite12 );
    quickSort123 ( suite21 );
    DynArray<CmbSuite<nat, 3> > another ( n );
    while ( i1 < suite12.size() && i2 < suite21.size() )
    {
        SortItem < Set2<nat>, nat> & e1 = suite12[i1];
        SortItem < Set2<nat>, nat> & e2 = suite21[i2];
        if ( e1.head.a == e2.head.a )
        {
            if ( e1.head.b == e2.head.b )
            {
                const nat j1 = i1++;
                const nat j2 = i2++;
                if ( i1 < suite12.size() && i2 < suite21.size() )
                {
                    SortItem < Set2<nat>, nat> & b1 = suite12[i1];
                    SortItem < Set2<nat>, nat> & b2 = suite21[i2];
                    if ( e1.head == b1.head && e2.head == b2.head )
                    {
                        if ( _min ( sides[e1.tail].info * sides[e2.tail].info, sides[b1.tail].info * sides[b2.tail].info ) <
                             _min ( sides[e1.tail].info * sides[b2.tail].info, sides[b1.tail].info * sides[e2.tail].info ) )
                             _swap ( e2.tail, b2.tail );
                    }
                }
                another[e1.tail].inc() = e2.tail;
                another[e2.tail].inc() = e1.tail;
                continue;
            }
            if ( e1.head.b < e2.head.b )
                ++i1;
            else
                ++i2;
        }
        else
        {
            if ( e1.head.a < e2.head.a )
                ++i1;
            else
                ++i2;
        }
    }
    Suite<nat> suite;
    nat count = 0;
    for ( i = 0; i < n; ++i )
    {
        if ( mark[i] ) continue;
        mark[i] = ++count;
        suite.inc() = i;
        while ( suite.size() > 0 )
        {
            const CmbSuite<nat, 3> & s = another[suite[0]];
            for ( nat j = 0; j < s.size(); ++j )
            {
                const nat a = s[j];
                if ( mark[a] != 0 ) continue;
                mark[a] = count;
                suite.inc() = a;
            }
            suite.del ( 0 );
        }
    }
    return count;
}

void divideIntoParts ( const char * filename, Poly3gon<Vector3f, Vector3f> & poly )
{
    DynArray<nat> mark;
    nat n = markSides ( poly.side, mark );
    Suite< Trigon<Vector3f> > sides;
    CString temp;
    for ( nat i = 1; i <= n; ++i )
    {
        sides.resize();
        for ( nat j = 0; j < mark.size(); ++j )
        {
            if ( mark[j] == i ) sides.inc() = poly.side[j];
        }
        temp.Format ( n > 9 ? n > 99 ? "%spart%03d.stl" : "%spart%02d.stl" : "%spart%d.stl", filename, i );
        saveSTLB ( RealFile ( temp, "wb" ), 0, 0, poly.vertex, sides );
    }
}

void DemoOther::OnOK() 
{
	CFileDialog temp ( TRUE, 0, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "*.stl|*.stl||" );
	if ( temp.DoModal() == IDOK )
	{
        RealFile file ( temp.GetPathName(), "rb" );
        if ( file.isValid() )
        {
            Poly3gon<Vector3f, Vector3f> poly ( 0, 0 );
            nat ntext;
            char text[80];
            if ( ! loadSTLA ( file, ntext, text, poly ) )
            {
                file.rewind();
                if ( ! loadSTLB ( file, ntext, text, poly ) )
                {
                    AfxMessageBox ( "Не читается файл       " );
                    return;
                }
            }
            divideIntoParts ( temp.GetFileTitle(), poly );
            AfxMessageBox ( "Готово !       " );
        }
    }
	//CDialog::OnOK();
}
