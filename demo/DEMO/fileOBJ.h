
#ifndef FILEOBJ_H
#define FILEOBJ_H

#include "typedef.h"

class PutWavefrontOBJ
{
protected:
    virtual ~PutWavefrontOBJ();
public:
    virtual void putVertex ( double x, double y, double z ) = 0;
    virtual void putNormal ( double x, double y, double z ) = 0;
    virtual void putFacet ( nat nv, const nat * vert, const nat * norm ) = 0;
};

bool loadOBJ ( class IReadFile & file, PutWavefrontOBJ & obj );

class GetWavefrontOBJ
{
protected:
    virtual ~GetWavefrontOBJ();
public:
    virtual void start ( nat & nv, nat & nn, nat & nf ) const = 0;
    virtual bool getVertex ( nat i, double & x, double & y, double & z ) const = 0;
    virtual bool getNormal ( nat i, double & x, double & y, double & z ) const = 0;
    virtual bool getFacet ( nat i, nat & nv, const nat * & vert, const nat * & norm ) const = 0;
};

bool saveOBJ ( class IWriteFile & file, const GetWavefrontOBJ & obj, nat prec );

#endif