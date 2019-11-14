
#ifndef POLYHEDRON_ERR_H
#define POLYHEDRON_ERR_H

#include "../shev/Polyhedron.h"

class PolyhedronErrorRecipient : public IPolyhedronErrorRecipient
{
    nat nVertexNumIsTooSmall, nVertexIndexIsTooBig, nVertexIndexIsEqual, nVertexIndexIsDiffer,
        nFacetIndexIsUndef, nFacetIndexIsTooBig, nIndexIndexIsUndef, nIndexIndexIsTooBig, 
        nNeighbourIsBad, nTrianTestBad, nMinArea;
    nat nvertex, nfacet;
    double maxDev, minArea;
    const double tol, minAdmArea;
    bool bVolumeIsNeg;
public:
    PolyhedronErrorRecipient ( double d, double area ) : tol(d), minAdmArea(area) {}
    void start ( nat nv, nat nf )
    {
        bVolumeIsNeg = false;
        nVertexNumIsTooSmall = nVertexIndexIsTooBig = nVertexIndexIsEqual = nVertexIndexIsDiffer =
        nFacetIndexIsUndef = nFacetIndexIsTooBig = nIndexIndexIsUndef = nIndexIndexIsTooBig =
        nNeighbourIsBad = nTrianTestBad = nMinArea = 0;
        minArea = minAdmArea;
        nvertex = nv;
        nfacet = nf;
        maxDev = 0.;
    }
    double  getTolerance     () { return tol; }
    double  getMinAdmArea    () { return minAdmArea; }
    void    putSmallArea     ( nat i, double a );
    void    putDeviation     ( nat i, nat j, double d );
    void vertexNumIsTooSmall ( nat i, nat n );
    void vertexIndexIsTooBig ( nat i, nat j );
    void vertexIndexIsEqual  ( nat i, nat j );
    void vertexIndexIsDiffer ( nat i, nat j, nat l );
    void  facetIndexIsUndef  ( nat i, nat j );
    void  facetIndexIsTooBig ( nat i, nat j, nat n );
    void  indexIndexIsUndef  ( nat i, nat j );
    void  indexIndexIsTooBig ( nat i, nat j, nat n, nat m );
    void    neighbourIsBad   ( nat i, nat n, nat m );
    void        trianTest    ( nat i );
    void       volumeIsNeg   ( double vol );
    void        finish       ();
};

#endif