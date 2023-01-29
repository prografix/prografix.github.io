
#ifndef FILEOBJPLH_H
#define FILEOBJPLH_H

#include "fileOBJ.h"
#include "ShevArray.h"

class Vector3d;

namespace Shev
{
    class Polyhedron;
}

class PutWavefrontOBJ_Polyhedron : public PutWavefrontOBJ
{
    struct IndexArray
    {
        nat nv;
        DynArray<nat> index;
    };
    Suite<Vector3d> vertex;
    Suite<IndexArray> facet;
    bool err;
public:
    PutWavefrontOBJ_Polyhedron ();
    void putVertex ( double x, double y, double z );
    void putNormal ( double x, double y, double z );
    void putFacet ( nat nv, const nat * vert, const nat * norm );
    bool give_to ( Shev::Polyhedron & poly );
};

class GetWavefrontOBJ_Polyhedron : public GetWavefrontOBJ
{
    mutable Suite<nat> buf;
    const Shev::Polyhedron & poly;
public:
    GetWavefrontOBJ_Polyhedron ( const Shev::Polyhedron & p ) : poly(p) {}
    void start ( nat & nv, nat & nn, nat & nf ) const;
    bool getVertex ( nat i, double & x, double & y, double & z ) const;
    bool getNormal ( nat i, double & x, double & y, double & z ) const;
    bool getFacet ( nat i, nat & nv, const nat * & vert, const nat * & norm ) const;
};

#endif