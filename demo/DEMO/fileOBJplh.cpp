
#include "FileOBJplh.h"
#include "Polyhedron.h"

PutWavefrontOBJ_Polyhedron::PutWavefrontOBJ_Polyhedron () : err(false) {}

void PutWavefrontOBJ_Polyhedron::putVertex ( double x, double y, double z )
{
    if ( err ) return;
    vertex.inc() = Vector3d ( x, y, z );
}

void PutWavefrontOBJ_Polyhedron::putNormal ( double x, double y, double z )
{
}

void PutWavefrontOBJ_Polyhedron::putFacet ( nat nv, const nat * ndx, const nat * norm )
{
    if ( err ) return;
    if ( nv < 3 )
    {
        err = true;
        return;
    }
    IndexArray & ia = facet.inc();
    ia.index.resize ( 3 * nv + 1 );
    ia.nv = nv;
    for ( nat i = 0; i < nv; ++i )
    {
        if ( ndx[i] > 0 )
        {
            const nat v = ndx[i] - 1;
            if ( v < vertex.size() )
            {
                ia.index[i] = v;
                continue;
            }
        }
        err = true;
        return;
    }
    ia.index[nv] = ia.index[0];
}

bool PutWavefrontOBJ_Polyhedron::give_to ( Polyhedron & poly )
{
    if ( err ) return false;
    poly.vertex = vertex;
    poly.facet.resize ( facet.size() );
    for ( nat i = 0; i < facet.size(); ++i )
    {
        _swap ( poly.facet[i].index, facet[i].index );
        poly.facet[i].nv = facet[i].nv;
    }
    poly.initPlanes();
    poly.linkFacets();
    err = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GetWavefrontOBJ_Polyhedron::start ( nat & nv, nat & nn, nat & nf ) const
{
    nv = poly.vertex.size();
    nn = 0;
    nf = poly.facet.size();
}

bool GetWavefrontOBJ_Polyhedron::getVertex ( nat i, double & x, double & y, double & z ) const
{
    if ( i >= poly.vertex.size() ) return false;
    const Vector3d & v = poly.vertex[i];
    x = v.x;
    y = v.y;
    z = v.z;
    return true;
}

bool GetWavefrontOBJ_Polyhedron::getNormal ( nat i, double & x, double & y, double & z ) const
{
    return false;
}

bool GetWavefrontOBJ_Polyhedron::getFacet ( nat i, nat & nv, const nat * & vert, const nat * & norm ) const
{
    if ( i >= poly.facet.size() ) return false;
    const Facet & f = poly.facet[i];
    nv = f.nv;
    buf.resize ( nv );
    for ( nat j = 0; j < nv; ++j )
    {
        buf[j] = f.index[j] + 1;
    }
    vert = & buf[0];
    norm = 0;
    return true;
}
