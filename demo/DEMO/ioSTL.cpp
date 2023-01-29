
#include <stdio.h>

#include "Trans.h"
#include "RealFile.h"
#include "Poly3gon.h"
#include "Vector3f.h"
#include "fileSTL.h"
#include "Polyhedron.h"
#include "Polyhedron_Poly3gon.h"

bool loadSTL ( const char * filename, Polyhedron & poly ) 
{
    RealFile file ( filename, "rb" );
    if ( ! file.isValid() ) return false;
    Poly3gon<Vector3f, Vector3f> temp ( 0, 0 );
    nat ntext;
    char text[80];
    if ( ! loadSTLA ( file, ntext, text, temp ) )
    {
        file.rewind();
        if ( ! loadSTLB ( file, ntext, text, temp ) ) return false;
    }
    copy ( temp, poly );
    return true;
}

bool saveSTLA ( const char * filename, const Polyhedron & poly ) 
{
    RealFile file ( filename, "w" );
    if ( ! file.isValid() ) return false;
    Poly3gon<Vector3f, Vector3f> temp ( 0, 0 );
    copy ( poly, temp );
    const bool ok = saveSTLA ( file, 0, 0, temp, 6 );
    if ( ! ok )
	{
		remove ( filename );
	}
    return ok;
}

bool saveSTLB ( const char * filename, const Polyhedron & poly ) 
{
    RealFile file ( filename, "wb" );
    if ( ! file.isValid() ) return false;
    Poly3gon<Vector3f, Vector3f> temp ( 0, 0 );
    copy ( poly, temp );
    const bool ok = saveSTLB ( file, 0, 0, temp );
    if ( ! ok )
	{
		remove ( filename );
	}
    return ok;
}