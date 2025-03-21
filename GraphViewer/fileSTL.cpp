
#include "File.h"
#include "fileSTL.h"
#include "Vector3f.h"

///////////////////////////////////////////////////////////////////////////////
///////////// ������ � ������ ��������� ( ASCII ) STL-������ //////////////////
///////////////////////////////////////////////////////////////////////////////

/*
��������� ��� ���������� ( ASCII ) STL-�����:

solid [part name]
facet normal 0.576759 -0.415057 -0.703617
outer loop
vertex 2.22934 -0.992723 -0.862826
vertex 2.29245 -0.871852 -0.8824
vertex 2.41037 -0.777999 -0.841105
endloop
endfacet
...
endsolid [part name]
*/

static bool getFacet ( IReadFile & file, char & c, Vector3f & norm, Vector3f vert[3] )
{
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'c' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� normal
    if ( c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'r' ) return false;
    if ( ! file.getc ( &c ) || c != 'm' ) return false;
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
// ������ ���������� �������
	if ( ! readFltDec ( file, c, norm.x ) || 
         ! readFltDec ( file, c, norm.y ) || 
         ! readFltDec ( file, c, norm.z ) ) return false;
// ������� �� ����� ������
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� outer
    if ( c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'u' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'r' ) return false;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� loop
    if ( c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'p' ) return false;
// ������� �� ����� ������
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
    for ( nat i = 0; i < 3; ++i )
    {
// ���������� �������
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// ������ �������� ����� vertex
        if ( c != 'v' ) return false;
        if ( ! file.getc ( &c ) || c != 'e' ) return false;
        if ( ! file.getc ( &c ) || c != 'r' ) return false;
        if ( ! file.getc ( &c ) || c != 't' ) return false;
        if ( ! file.getc ( &c ) || c != 'e' ) return false;
        if ( ! file.getc ( &c ) || c != 'x' ) return false;
// ������ ���������� �������
		if ( ! readFltDec ( file, c, vert[i].x ) || 
             ! readFltDec ( file, c, vert[i].y ) || 
             ! readFltDec ( file, c, vert[i].z ) ) return false;
// ������� �� ����� ������
		while ( c != '\n' && file.getc ( &c ) ) continue;
        if ( c != '\n' ) return false;
    }
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� endloop
    if ( c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'p' ) return false;
// ������� �� ����� ������
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� endfacet
    if ( c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 'f' ) return false;
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'c' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
// ������� �� ����� ������
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
    return true;
}

// ��������� �-�� ������

static bool countFacets ( IReadFile & file, nat & nfacet )
{
    nfacet = 0;
    char c;
// ���������� �������
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// ������ �������� ����� facet
    if ( c != 'f' ) return false;
    Vector3f norm, vert[3];
m1: if ( ! getFacet ( file, c, norm, vert ) ) return false;
    ++nfacet;
// ��������� ������ ��� facet ��� endsolid?
    if ( c == 'f' ) goto m1;
    if ( c != 'e' ) return false;
// ������ �������� ����� endfacet
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 's' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'i' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    return true;
}

bool loadSTLA ( IReadSeekFile & file, PutSTL & stl )
{
    bool isSolid = false;
    for(;;)
    {
        char c;
// ���������� �������
        do
        {
            if ( ! file.getc ( &c ) ) return isSolid;
        }
        while ( c == '\t' || c == ' ' );
// ������ �������� ����� solid
        if ( c != 's' ) return false;
        if ( ! file.getc ( &c ) || c != 'o' ) return false;
        if ( ! file.getc ( &c ) || c != 'l' ) return false;
        if ( ! file.getc ( &c ) || c != 'i' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
// ���������� �������
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// ������ ����������� �� ���� �� ������
        nat n = 0;
        char text[80];
        while ( c != '\n' )
        {
           if ( n < 80 ) text[n++] = c;
           if ( ! file.getc ( &c ) ) return false;
        }
// ������� �-�� ������ � ������������ �� �� �� �����
        nat nfacet;
        const long pos = file.tell();
        if ( ! countFacets ( file, nfacet ) ) return false;
        file.seek_set ( pos );
        stl.start ( n, text, nfacet );
        nat count = 0;
// ���������� �������
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// ������ �������� ����� facet
        if ( c != 'f' ) return false;
        Vector3f norm, vert[3];
m1:     if ( ! getFacet ( file, c, norm, vert ) ) return false;
        stl.putFacet ( count++, norm, vert[0], vert[1], vert[2] );
// ��������� ������ ��� facet ��� endsolid?
        if ( c == 'f' ) goto m1;
        if ( c != 'e' ) return false;
// ������ �������� ����� endfacet
        if ( ! file.getc ( &c ) || c != 'n' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
        if ( ! file.getc ( &c ) || c != 's' ) return false;
        if ( ! file.getc ( &c ) || c != 'o' ) return false;
        if ( ! file.getc ( &c ) || c != 'l' ) return false;
        if ( ! file.getc ( &c ) || c != 'i' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
// ������� �� ����� ������
		while ( c != '\n' && file.getc ( &c ) ) continue;
        isSolid = true;
        stl.finish();
    }
}

bool saveSTLA ( IWriteFile & file, GetSTL & stl, nat prec )
{
    nat ntext, nf;
    const char * text;
    stl.start ( ntext, text, nf );
    if ( file.write ( "solid ", 6, 1 ) != 1 ) return false;
    if ( file.write ( text, 1, ntext ) != ntext ) return false;
    if ( file.write ( "\n", 1, 1 ) != 1 ) return false;

    Vector3f norm, v1, v2, v3;
    for ( nat i = 0; i < nf; ++i )
    {
        stl.getFacet ( i, norm, v1, v2, v3 );
        if ( ! writeStr ( file, "facet normal " ) ) return false;
		if ( ! writeFltDec ( file, norm.x, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, norm.y, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, norm.z, prec ) ) return false;
        if ( ! writeStr ( file, "\nouter loop\nvertex " ) ) return false;
		if ( ! writeFltDec ( file, v1.x, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v1.y, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v1.z, prec ) ) return false;
        if ( ! writeStr ( file, "\nvertex " ) ) return false;
		if ( ! writeFltDec ( file, v2.x, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v2.y, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v2.z, prec ) ) return false;
        if ( ! writeStr ( file, "\nvertex " ) ) return false;
		if ( ! writeFltDec ( file, v3.x, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v3.y, prec ) ) return false;
		if ( ! file.putc ( " " ) ) return false;
		if ( ! writeFltDec ( file, v3.z, prec ) ) return false;
		if ( ! writeStr ( file, "\nendloop\nendfacet\n" ) ) return false;
    }
	return writeStr ( file, "endsolid" );
}

///////////////////////////////////////////////////////////////////////////////
///////////////// ������ � ������ �������� STL-������ /////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
struct facet
{
    float normal[3];     // 12 ����
    float vertex[3][3];  // 36 ����
    nat16 attr;          // 2 �����
};

struct stl_file
{
    char head[80]; // �����������   - 80 ����
    nat32     n;   // �-�� ������   - 4  �����
    facet arr[n];  // ������ ������ - n * 50 ����
};
*/

bool loadSTLB ( IReadFile & file, PutSTL & stl )
{
    char text[80];
    if ( file.read ( text, 80, 1 ) != 1 ) return false;
    nat32 nfacet;
    if ( file.read ( &nfacet, 4, 1 ) != 1 ) return false;
    stl.start ( 80, text, nfacet );
    Vector3f vn, v1, v2, v3;
    for ( nat32 i = 0; i < nfacet; ++i )
    {
        if ( file.read ( &vn, 12, 1 ) != 1 ) return false;
        if ( file.read ( &v1, 12, 1 ) != 1 ) return false;
        if ( file.read ( &v2, 12, 1 ) != 1 ) return false;
        if ( file.read ( &v3, 12, 1 ) != 1 ) return false;
        if ( file.read ( text, 2, 1 ) != 1 ) return false;
        stl.putFacet ( i, vn, v1, v2, v3 );
    }
    stl.finish();
	return true;
}

bool saveSTLB ( IWriteFile & file, GetSTL & stl )
{
    nat ntext, nf;
    const char * text;
    stl.start ( ntext, text, nf );
    char buf[80];
    for ( nat j = 0; j < 80; ++j )
    {
        buf[j] = j < ntext ? text[j] : 0;
    }
    if ( file.write ( buf, 80, 1 ) != 1 ) return false;
    if ( file.write ( & nf, 4, 1 ) != 1 ) return false;
    buf[0] = buf[1] = 0;
    Vector3f v[4];
    for ( nat i = 0; i < nf; ++i )
    {
        stl.getFacet ( i, v[0], v[1], v[2], v[3] );
        if ( file.write ( v, 12, 4 ) != 4 ) return false;
        if ( file.write ( buf, 2, 1 ) != 1 ) return false;
    }
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

#include "Poly3gon.h"

namespace {

struct Node
{
    nat next, index;
};

class Nat3HashTable
{
    nat nkey;
    Suite<Node> narr;
public:
    Suite< Set3<nat32> > varr;

    nat index ( const Set3<nat32> & v );
    void init ( nat s );
};

void Nat3HashTable::init ( nat s )
{
    if ( s >= 32 )
    {
        nkey = s / 8;
        narr.resize ( nkey );
        for ( nat j = 0; j < nkey; ++j ) narr[j].next = 0;
    }
}
 
nat Nat3HashTable::index ( const Set3<nat32> & v )
{
    if ( narr.size() == 0 )
    {
        for ( nat j = 0; j < varr.size(); ++j )
            if ( v == varr[j] ) return j;
    }
    else
    {
        nat s = ( v.a + v.b + v.c ) % nkey;
        for (;;)
        {
            const Node & node = narr[s];
            if ( node.next == 0 ) break;
            if ( v == varr[node.index] ) return node.index;
            s = node.next;
        }
        Node & node = narr[s];
        node.next = narr.size();
        node.index = varr.size();
        narr.inc().next = 0;
    }
    varr.inc() = v;
    return varr.size() - 1;
}

class PutSTL_Poly3gonVector3f : public PutSTL
{
    nat & ntext;
    char * text;
    Poly3gon<Vector3f, Vector3f> & poly;
    Nat3HashTable table;
public:
    PutSTL_Poly3gonVector3f ( nat & n, char * t, Poly3gon<Vector3f, Vector3f> & p ) : ntext(n), text(t), poly ( p ) {}
    virtual void start ( nat n, const char * text, nat nfacet );
    virtual void putFacet ( nat i, const Vector3f & vn, const Vector3f & v1, 
                            const Vector3f & v2, const Vector3f & v3 );
    virtual void finish ();
};

void PutSTL_Poly3gonVector3f::start ( nat n, const char * t, nat nfacet )
{
    if ( text == 0 )
    {
        ntext = 0;
    }
    else
    {
        ntext = n;
        for ( nat i = 0; i < n; ++i ) text[i] = t[i];
    }
    poly.resize ( 0, nfacet );
    table.init ( nfacet/2 + 2 );
}

void PutSTL_Poly3gonVector3f::putFacet ( nat i, const Vector3f & vn, const Vector3f & v1, 
                                         const Vector3f & v2, const Vector3f & v3 )
{
    Trigon<Vector3f> & t = poly.side[i];
    t.a = table.index ( *(const Set3<nat32>*)& v1 );
    t.b = table.index ( *(const Set3<nat32>*)& v2 );
    t.c = table.index ( *(const Set3<nat32>*)& v3 );
    t.info = vn;
}

void PutSTL_Poly3gonVector3f::finish()
{
    *(DynArray<Set3<nat32> >*)& poly.vertex = table.varr;
}

class GetSTL_Poly3gonVector3f : public GetSTL
{
    const nat ntext;
    const char * text;
    const Poly3gon<Vector3f, Vector3f> & poly;
public:
    GetSTL_Poly3gonVector3f ( const nat n, const char * t, const Poly3gon<Vector3f, Vector3f> & p ) : ntext(n), text(t), poly ( p ) {}
    virtual void start ( nat & ntext, const char * & text, nat & nfacet ) const;
    virtual void getFacet ( nat i, Vector3f & vn, Vector3f & v1, Vector3f & v2, Vector3f & v3 ) const;
};

void GetSTL_Poly3gonVector3f::start ( nat & nt, const char * & txt, nat & nfacet ) const
{
    nt = ntext;
    txt = text;
    nfacet = poly.side.size();
}

void GetSTL_Poly3gonVector3f::getFacet ( nat i, Vector3f & vn, Vector3f & v1, Vector3f & v2, Vector3f & v3 ) const
{
    const Trigon<Vector3f> & t = poly.side[i];
    v1 = poly.vertex[t.a];
    v2 = poly.vertex[t.b];
    v3 = poly.vertex[t.c];
    vn = poly.side[i].info;
}

} // namespace

////////////////////////////////////////////////////////////////////////////////////

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    PutSTL_Poly3gonVector3f stl ( ntext, text, poly );
    return loadSTLA ( file, stl );
}

bool loadSTLB ( IReadFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    PutSTL_Poly3gonVector3f stl ( ntext, text, poly );
    return loadSTLB ( file, stl );
}

bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec )
{
    GetSTL_Poly3gonVector3f stl ( ntext, text, poly );
    return saveSTLA ( file, stl, prec );
}

bool saveSTLB ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly )
{
    GetSTL_Poly3gonVector3f stl ( ntext, text, poly );
    return saveSTLB ( file, stl ) ;
}

////////////////////////////////////////////////////////////////////////////////////
