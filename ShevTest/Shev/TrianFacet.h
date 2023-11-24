
#pragma once

#include "Vector3d.h"
#include "ShevArray.h"
#include "Lists.h"


class TrianFacet
{
public:
    nat vertex[3], facet[3], edge[3], index;
    Plane3d plane;
    List1d list;
};

bool convexHull ( CCArrRef<Vector3d> & point, nat & nv, ArrRef<nat> & iv, nat & nf, ArrRef<TrianFacet> & facet );
