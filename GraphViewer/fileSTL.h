
#ifndef FILE_STL_H
#define FILE_STL_H

#include "typedef.h"

class Vector3f;
class IReadFile;
class IReadSeekFile;
class IWriteFile;

class PutSTL
{
public:
    virtual void start ( nat ntext, const char * text, nat nfacet ) = 0;
    virtual void putFacet ( nat i, const Vector3f & vn, const Vector3f & v1, 
                            const Vector3f & v2, const Vector3f & v3 ) = 0;
    virtual void finish () = 0;
};

bool loadSTLA ( IReadSeekFile & file, PutSTL & stl );
bool loadSTLB ( IReadFile     & file, PutSTL & stl );

class GetSTL
{
public:
    virtual void start ( nat & ntext, const char * & text, nat & nfacet ) const = 0;
    virtual void getFacet ( nat i, Vector3f & vn, Vector3f & v1, 
                            Vector3f & v2, Vector3f & v3 ) const = 0;
};

bool saveSTLA ( IWriteFile & file, GetSTL & stl, nat prec );
bool saveSTLB ( IWriteFile & file, GetSTL & stl );

////////////////////////////////////////////////////////////////////////////////////

template <class T, class I> class Poly3gon;

bool loadSTLA ( IReadSeekFile & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );
bool loadSTLB ( IReadFile     & file, nat & ntext, char * text, Poly3gon<Vector3f, Vector3f> & poly );

bool saveSTLA ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly, nat prec );
bool saveSTLB ( IWriteFile & file, nat ntext, const char * text, const Poly3gon<Vector3f, Vector3f> & poly );

#endif