
#include "tune.h"
#include "File.h"
#include "fileSTL.h"
#include "Vector3f.h"

///////////////////////////////////////////////////////////////////////////////
///////////// Запись и чтение текстовых ( ASCII ) STL-файлов //////////////////
///////////////////////////////////////////////////////////////////////////////

/*
Примерный вид текстового ( ASCII ) STL-файла:

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

template<class T> bool getFacet ( IReadFile & file, char & c, T & norm, T vert[3] )
{
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'c' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово normal
    if ( c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'r' ) return false;
    if ( ! file.getc ( &c ) || c != 'm' ) return false;
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
// Читаем координаты нормали
	if ( ! readFltDec ( file, c, norm.x ) || 
         ! readFltDec ( file, c, norm.y ) || 
         ! readFltDec ( file, c, norm.z ) ) return false;
// Доходим до конца строки
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово outer
    if ( c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'u' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'r' ) return false;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово loop
    if ( c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'p' ) return false;
// Доходим до конца строки
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
    for ( nat i = 0; i < 3; ++i )
    {
// Пропускаем пробелы
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// Читаем ключевое слово vertex
        if ( c != 'v' ) return false;
        if ( ! file.getc ( &c ) || c != 'e' ) return false;
        if ( ! file.getc ( &c ) || c != 'r' ) return false;
        if ( ! file.getc ( &c ) || c != 't' ) return false;
        if ( ! file.getc ( &c ) || c != 'e' ) return false;
        if ( ! file.getc ( &c ) || c != 'x' ) return false;
// Читаем координаты вершины
		if ( ! readFltDec ( file, c, vert[i].x ) || 
             ! readFltDec ( file, c, vert[i].y ) || 
             ! readFltDec ( file, c, vert[i].z ) ) return false;
// Доходим до конца строки
		while ( c != '\n' && file.getc ( &c ) ) continue;
        if ( c != '\n' ) return false;
    }
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово endloop
    if ( c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'p' ) return false;
// Доходим до конца строки
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово endfacet
    if ( c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 'f' ) return false;
    if ( ! file.getc ( &c ) || c != 'a' ) return false;
    if ( ! file.getc ( &c ) || c != 'c' ) return false;
    if ( ! file.getc ( &c ) || c != 'e' ) return false;
    if ( ! file.getc ( &c ) || c != 't' ) return false;
// Доходим до конца строки
	while ( c != '\n' && file.getc ( &c ) ) continue;
    if ( c != '\n' ) return false;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
    return true;
}

// Посчитать к-во граней

static bool countFacets ( IReadFile & file, nat & nfacet )
{
    nfacet = 0;
    char c;
// Пропускаем пробелы
    do
    {
        if ( ! file.getc ( &c ) ) return false;
    }
    while ( c == '\t' || c == ' ' );
// Читаем ключевое слово facet
    if ( c != 'f' ) return false;
    Vector3f norm, vert[3];
m1: if ( ! getFacet ( file, c, norm, vert ) ) return false;
    ++nfacet;
// Проверяем дальше идёт facet или endsolid?
    if ( c == 'f' ) goto m1;
    if ( c != 'e' ) return false;
// Читаем ключевое слово endfacet
    if ( ! file.getc ( &c ) || c != 'n' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    if ( ! file.getc ( &c ) || c != 's' ) return false;
    if ( ! file.getc ( &c ) || c != 'o' ) return false;
    if ( ! file.getc ( &c ) || c != 'l' ) return false;
    if ( ! file.getc ( &c ) || c != 'i' ) return false;
    if ( ! file.getc ( &c ) || c != 'd' ) return false;
    return true;
}

template <class T> bool loadSTLA ( IReadSeekFile & file, PutSTL<T> & stl )
{
    bool isSolid = false;
    for(;;)
    {
        char c;
// Пропускаем пробелы
        do
        {
            if ( ! file.getc ( &c ) ) return isSolid;
        }
        while ( c == '\t' || c == ' ' );
// Читаем ключевое слово solid
        if ( c != 's' ) return false;
        if ( ! file.getc ( &c ) || c != 'o' ) return false;
        if ( ! file.getc ( &c ) || c != 'l' ) return false;
        if ( ! file.getc ( &c ) || c != 'i' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
// Пропускаем пробелы
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// Читаем комментарий на этой же строке
        nat n = 0;
        char text[80];
        while ( c != '\n' )
        {
           if ( n < 80 ) text[n++] = c;
           if ( ! file.getc ( &c ) ) return false;
        }
// Считаем к-во граней и возвращаемся на то же место
        nat nfacet;
        const long pos = file.tell();
        if ( ! countFacets ( file, nfacet ) ) return false;
        file.seek_set ( pos );
        stl.start ( n, text, nfacet );
        nat count = 0;
// Пропускаем пробелы
        do
        {
            if ( ! file.getc ( &c ) ) return false;
        }
        while ( c == '\t' || c == ' ' );
// Читаем ключевое слово facet
        if ( c != 'f' ) return false;
        T norm, vert[3];
m1:     if ( ! getFacet ( file, c, norm, vert ) ) return false;
        stl.putFacet ( count++, norm, vert[0], vert[1], vert[2] );
// Проверяем дальше идёт facet или endsolid?
        if ( c == 'f' ) goto m1;
        if ( c != 'e' ) return false;
// Читаем ключевое слово endfacet
        if ( ! file.getc ( &c ) || c != 'n' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
        if ( ! file.getc ( &c ) || c != 's' ) return false;
        if ( ! file.getc ( &c ) || c != 'o' ) return false;
        if ( ! file.getc ( &c ) || c != 'l' ) return false;
        if ( ! file.getc ( &c ) || c != 'i' ) return false;
        if ( ! file.getc ( &c ) || c != 'd' ) return false;
// Доходим до конца строки
		while ( c != '\n' && file.getc ( &c ) ) continue;
        isSolid = true;
        stl.finish();
    }
}

template <class T> bool saveSTLA ( IWriteFile & file, GetSTL<T> & stl, nat prec )
{
    nat ntext, nf;
    const char * text;
    stl.start ( ntext, text, nf );
    if ( file.write ( "solid ", 6, 1 ) != 1 ) return false;
    if ( file.write ( text, 1, ntext ) != (nat) ntext ) return false;
    if ( file.write ( "\n", 1, 1 ) != 1 ) return false;
    T norm, v1, v2, v3;
    for ( nat i = 0; i < nf; ++i )
    {
        stl.getFacet ( i, norm, v1, v2, v3 );
        double x = norm.x;
        double y = norm.y;
        double z = norm.z;
        if ( ! printf ( file, "facet normal %.*f %.*f %.*f\nouter loop\n", prec, x, prec, y, prec, z ) ) return false;
        x = v1.x; y = v1.y; z = v1.z;
        if ( ! printf ( file, "vertex %.*f %.*f %.*f\n", prec, x, prec, y, prec, z ) ) return false;
        x = v2.x; y = v2.y; z = v2.z;
        if ( ! printf ( file, "vertex %.*f %.*f %.*f\n", prec, x, prec, y, prec, z ) ) return false;
        x = v3.x; y = v3.y; z = v3.z;
        if ( ! printf ( file, "vertex %.*f %.*f %.*f\n", prec, x, prec, y, prec, z ) ) return false;
		if ( ! printf ( file, "endloop\nendfacet\n" ) ) return false;
    }
	return file.write ( "endsolid", 8, 1 ) != 1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////// Запись и чтение бинарных STL-файлов /////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
struct facet
{
    float normal[3];     // 12 байт
    float vertex[3][3];  // 36 байт
    nat16 attr;          // 2 байта
};

struct stl_file
{
    char head[80]; // комментарий   - 80 байт
    nat32     n;   // к-во граней   - 4  байта
    facet arr[n];  // массив граней - n * 50 байт
};
*/

bool loadSTLB ( IReadFile & file, PutSTL<Vector3f> & stl )
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

bool saveSTLB ( IWriteFile & file, GetSTL<Vector3f> & stl )
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

namespace {

struct Node
{
    nat next, index;
};

template < class T >
class Set3HashTable
{
    nat nkey;
    Suite<Node> narr;
public:
    Suite< Set3<T> > varr;

    void init ( nat s )
    {
        if ( s >= 32 )
        {
            nkey = s / 8;
            narr.resize ( nkey );
            for ( nat j = 0; j < nkey; ++j ) narr[j].next = 0;
        }
    }
 
    nat index ( const Set3<T> & v )
    {
        if ( narr.size() == 0 )
        {
            for ( nat j = 0; j < varr.size(); ++j )
                if ( v == varr[j] ) return j;
        }
        else
        {
            nat s = nat ( v.a + v.b + v.c ) % nkey;
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
};

////////////////////////////////////////////////////////////////////////////////////

class PutSTL_Poly3gon3f : public PutSTL<Vector3f>
{
    nat & ntext;
    char * text;
    Poly3gon<Vector3f, Vector3f> & poly;
    Set3HashTable<nat32> table;
public:
    PutSTL_Poly3gon3f ( nat & n, char * t, Poly3gon<Vector3f, Vector3f> & p ) : ntext(n), text(t), poly ( p ) {}
    virtual void start ( nat n, const char * text, nat nfacet );
    virtual void putFacet ( nat i, const Vector3f & vn, const Vector3f & v1, 
                            const Vector3f & v2, const Vector3f & v3 );
    virtual void finish ();
};

void PutSTL_Poly3gon3f::start ( nat n, const char * t, nat nfacet )
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

void PutSTL_Poly3gon3f::putFacet ( nat i, const Vector3f & vn, const Vector3f & v1, 
                                          const Vector3f & v2, const Vector3f & v3 )
{
    Trigon<Vector3f> & t = poly.side[i];
    t.a = table.index ( (const Set3<nat32>&) v1 );
    t.b = table.index ( (const Set3<nat32>&) v2 );
    t.c = table.index ( (const Set3<nat32>&) v3 );
    t.info = vn;
}

void PutSTL_Poly3gon3f::finish()
{
    (DynArray<Set3<nat32> > &) poly.vertex = table.varr;
}

////////////////////////////////////////////////////////////////////////////////////

#ifdef VECTOR3D_H_IN_FILESTL_CPP

#include "Vector3d.h"

class PutSTL_Poly3gon3d : public PutSTL<Vector3d>
{
    nat & ntext;
    char * text;
    Poly3gon<Vector3d, Vector3d> & poly;
    Set3HashTable<nat64> table;
public:
    PutSTL_Poly3gon3d ( nat & n, char * t, Poly3gon<Vector3d, Vector3d> & p ) : ntext(n), text(t), poly ( p ) {}
    virtual void start ( nat n, const char * text, nat nfacet );
    virtual void putFacet ( nat i, const Vector3d & vn, const Vector3d & v1, 
                            const Vector3d & v2, const Vector3d & v3 );
    virtual void finish ();
};

void PutSTL_Poly3gon3d::start ( nat n, const char * t, nat nfacet )
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

void PutSTL_Poly3gon3d::putFacet ( nat i, const Vector3d & vn, const Vector3d & v1, 
                                          const Vector3d & v2, const Vector3d & v3 )
{
    Trigon<Vector3d> & t = poly.side[i];
    t.a = table.index ( (const Set3<nat64>&) v1 );
    t.b = table.index ( (const Set3<nat64>&) v2 );
    t.c = table.index ( (const Set3<nat64>&) v3 );
    t.info = vn;
}

void PutSTL_Poly3gon3d::finish()
{
    (DynArray<Set3<nat64> > &) poly.vertex = table.varr;
}

#endif // VECTOR3D_H_IN_FILESTL_CPP

////////////////////////////////////////////////////////////////////////////////////

template <class T> class GetSTL_Poly3gon : public GetSTL<T>
{
    const nat ntext;
    const char * text;
    CArrRef<T> vertex;
    CArrRef< Trigon<T> > facet;
public:
    GetSTL_Poly3gon ( const nat n, const char * t, CArrRef<T> v, CArrRef< Trigon<T> > f ) : ntext(n), text(t), vertex(v), facet(f) {}

    void start ( nat & nt, const char * & txt, nat & nfacet ) const
    {
        nt = ntext;
        txt = text;
        nfacet = facet.size();
    }

    void getFacet ( nat i, T & vn, T & v1, T & v2, T & v3 ) const
    {
        const Trigon<T> & t = facet[i];
        v1 = vertex[t.a];
        v2 = vertex[t.b];
        v3 = vertex[t.c];
        vn = t.info;
    }
};

} // namespace

////////////////////////////////////////////////////////////////////////////////////

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    PutSTL_Poly3gon3f stl ( ntext, text, poly );
    return loadSTLA ( file, stl );
}

bool loadSTLB ( IReadFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    PutSTL_Poly3gon3f stl ( ntext, text, poly );
    return loadSTLB ( file, stl );
}

bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec )
{
    GetSTL_Poly3gon<Vector3f> stl ( ntext, text, poly.vertex, poly.side );
    return saveSTLA ( file, stl, prec );
}

bool saveSTLB ( IWriteFile & file, nat ntext, const char * text, CArrRef<Vector3f> vertex, CArrRef< Trigon<Vector3f> > facet )
{
    GetSTL_Poly3gon<Vector3f> stl ( ntext, text, vertex, facet );
    return saveSTLB ( file, stl ) ;
}

////////////////////////////////////////////////////////////////////////////////////

#include "RealFile.h"

bool loadSTLA ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    return loadSTLA ( RealFile ( filename, "rb" ), ntext, text, poly );
}

bool loadSTLB ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    return loadSTLB ( RealFile ( filename, "rb" ), ntext, text, poly );
}

bool loadSTL ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly )
{
    RealFile file ( filename, "rb" );
    return loadSTLA ( file, ntext, text, poly ) || loadSTLB ( file, ntext, text, poly );
}

bool saveSTLA ( const char * filename, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec )
{
    return saveSTLA ( RealFile ( filename, "wb" ), ntext, text, poly, prec );
}

bool saveSTLB ( const char * filename, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly )
{
    return saveSTLB ( RealFile ( filename, "wb" ), ntext, text, poly.vertex, poly.side );
}

////////////////////////////////////////////////////////////////////////////////////

#ifdef VECTOR3D_H_IN_FILESTL_CPP

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3d, Vector3d> & poly )
{
    PutSTL_Poly3gon3d stl ( ntext, text, poly );
    return loadSTLA ( file, stl );
}

bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3d, Vector3d> & poly, nat prec )
{
    GetSTL_Poly3gon<Vector3d> stl ( ntext, text, poly );
    return saveSTLA ( file, stl, prec );
}

#endif // VECTOR3D_H_IN_FILESTL_CPP
