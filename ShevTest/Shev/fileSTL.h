
#ifndef FILE_STL_H
#define FILE_STL_H

#include "typedef.h"

template <class T> class PutSTL
{
public:
    virtual void start ( nat ntext, const char * text, nat nfacet ) = 0;
    virtual void putFacet ( nat i, const T & vn, const T & v1, const T & v2, const T & v3 ) = 0;
    virtual void finish () = 0;
};

template <class T> class GetSTL
{
public:
    virtual void start ( nat & ntext, const char * & text, nat & nfacet ) const = 0;
    virtual void getFacet ( nat i, T & vn, T & v1, T & v2, T & v3 ) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////

class IReadFile;
class IReadSeekFile;
class IWriteFile;

#include "Poly3gon.h"

#ifdef VECTOR3F_H

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );
bool loadSTLB ( IReadFile     & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );

bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec );
bool saveSTLB ( IWriteFile & file, nat ntext, const char * text, CArrRef<Vector3f> vertex, CArrRef< Trigon<Vector3f> > facet );
inline
bool saveSTLB ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly )
{
    return saveSTLB ( file, ntext, text, poly.vertex, poly.side );
}

////////////////////////////////////////////////////////////////////////////////////

bool loadSTLA ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );
bool loadSTLB ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );
bool loadSTL  ( const char * filename, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );

bool saveSTLA ( const char * filename, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec );
bool saveSTLB ( const char * filename, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly );

#endif // VECTOR3F_H

#ifdef VECTOR3D_H

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3d, Vector3d> & poly );
bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3d, Vector3d> & poly, nat prec );

#endif // VECTOR3D_H

#endif // FILE_STL_H