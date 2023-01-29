
#include <stdio.h>

#include "RealFile.h"
#include "filePYH.h"
#include "Polyhedron.h"

bool loadPYH ( const char * filename, Polyhedron & poly ) 
{
    RealFile file ( filename, "rb" );
    return loadPYH ( file, poly );
}
