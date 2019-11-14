
#pragma once
#include "octMath/octPolyhedron.h"
#include "ExtraBody.h"


bool copy ( const oct::OctPolyhedron & poly, ExtraBodyStor & stor, ExtraBody & body );

bool copy ( const ExtraBody & body, oct::OctPolyhedron & poly );