
#include "math.h"
#include "PolyhedronErrorRecipient.h"
#include "Display.h"

static void dummy ()
{
}

void PolyhedronErrorRecipient::putSmallArea ( nat i, double a )
{
    ++nMinArea;
    dummy();
    if ( minArea > a )
    {
        minArea = a;
    }
}

void PolyhedronErrorRecipient::putDeviation ( nat i, nat j, double d )
{
    if ( fabs ( maxDev ) < fabs ( d ) )
    {
        maxDev = d;
    }
}

void PolyhedronErrorRecipient::vertexNumIsTooSmall ( nat i, nat n )
{
    ++nVertexNumIsTooSmall;
    dummy();
}

void PolyhedronErrorRecipient::vertexIndexIsTooBig ( nat i, nat j )
{
     ++nVertexIndexIsTooBig;
    dummy();
}

void PolyhedronErrorRecipient::vertexIndexIsEqual ( nat i, nat j )
{
    ++nVertexIndexIsEqual;
    dummy();
}

void PolyhedronErrorRecipient::vertexIndexIsDiffer ( nat i, nat j, nat n )
{
     ++nVertexIndexIsDiffer;
    dummy();

}

void PolyhedronErrorRecipient::facetIndexIsUndef ( nat i, nat j )
{
    ++nFacetIndexIsUndef;
    dummy();
}

void PolyhedronErrorRecipient::facetIndexIsTooBig ( nat i, nat j, nat n )
{
    ++nFacetIndexIsTooBig;
    dummy();
}

void PolyhedronErrorRecipient::indexIndexIsUndef  ( nat i, nat j )
{
    ++nIndexIndexIsUndef;
    dummy();
}

void PolyhedronErrorRecipient::indexIndexIsTooBig ( nat i, nat j, nat n, nat m )
{
    ++nIndexIndexIsTooBig;
    dummy();
}

void PolyhedronErrorRecipient::neighbourIsBad   ( nat i, nat n, nat m )
{
    ++nNeighbourIsBad;
    dummy();
}

void PolyhedronErrorRecipient::trianTest ( nat i )
{
    ++nTrianTestBad;
    dummy();
}

void PolyhedronErrorRecipient::volumeIsNeg ( double vol )
{
    bVolumeIsNeg = true;
    dummy();
}

void PolyhedronErrorRecipient::finish ()
{
    if ( nVertexNumIsTooSmall > 0 ) display << "nVertexNumIsTooSmall" << nVertexNumIsTooSmall << NL;
    if ( nVertexIndexIsTooBig > 0 ) display << "nVertexIndexIsTooBig" << nVertexIndexIsTooBig << NL;
    if ( nVertexIndexIsEqual > 0 ) display << "nVertexIndexIsEqual" << nVertexIndexIsEqual << NL;
    if ( nVertexIndexIsDiffer > 0 ) display << "nVertexIndexIsDiffer" << nVertexIndexIsDiffer << NL;
    if ( nFacetIndexIsUndef > 0 ) display << "nFacetIndexIsUndef" << nFacetIndexIsUndef << NL;
    if ( nFacetIndexIsTooBig > 0 ) display << "nFacetIndexIsTooBig" << nFacetIndexIsTooBig << NL;
    if ( nIndexIndexIsUndef > 0 ) display << "nIndexIndexIsUndef" << nIndexIndexIsUndef << NL;
    if ( nIndexIndexIsTooBig > 0 ) display << "nIndexIndexIsTooBig" << nIndexIndexIsTooBig << NL;
    if ( nNeighbourIsBad > 0 ) display << "nNeighbourIsBad" << nNeighbourIsBad << NL;
    if ( nTrianTestBad > 0 ) display << "nTrianTestBad" << nTrianTestBad << NL;
    if ( nMinArea > 0 ) display << "nMinArea" << nMinArea << minArea << NL;
    if ( bVolumeIsNeg ) display << "volume < 0" << NL;
    if ( maxDev != 0 )
    {
        display << "maxDev = " << maxDev << NL;
    }
}