
#include "RealFile.h"
#include "FileOBJplh.h"
#include "Vector3d.h"

bool loadOBJ ( const char * filename, Shev::Polyhedron & poly )
{
    RealFile file ( filename, "rb" );
    if ( file.isValid() == false )
    {
        return false;
    }
    PutWavefrontOBJ_Polyhedron obj;
    if ( loadOBJ ( file, obj ) == false )
    {
        return 0;
    }
    return obj.give_to ( poly );
}

bool saveOBJ ( const char * filename, const Shev::Polyhedron & poly )
{
    RealFile file ( filename, "w" );
    if ( file.isValid() == false )
    {
        return false;
    }
    GetWavefrontOBJ_Polyhedron obj ( poly );
    return saveOBJ ( file, obj, 6 );
}