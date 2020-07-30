
#include "../shev/File.h"
#include "../shev/RealFile.h"
#include "../shev/Polyhedron.h"

#include "../extra/Body2.h"
#include "../extra/filePYH_Body.h"

#include "../draw.h"
#include "display.h"

bool copy ( const Body & body, Polyhedron & poly );
bool loadSTL ( const char * filename, Polyhedron & poly );

void filePYH_test ()
{
    Body body1, body2, body3;
    Polyhedron poly1, poly2;
//    if ( loadSTL ( "F:/TEST/HP_ModelRecognizedToXray.stl", poly1 ) )
//    if ( loadSTL ( "F:/TEST/modelWithCavitiesShev.stl", poly1 ) )
    if ( loadSTL ( "F:/TEST/simplifiedXray.stl", poly1 ) )
    {
//        body1 -= body1.centerOfMass ();
//        body1 *= 0.1;
    }
    else
    {
        display << "no load file 1" << NL;
        return;
    }
	nat i;
    for ( i = 955; i < _min ( 956u, poly1.facet.size() ); ++i )
    {
//        if ( i != 311 ) continue;
//        if ( i == 311 ) continue;
        copy ( poly1, body1 );
display << i << NL;
        poly1 *= 0.999;
        body1.cut ( poly1.facet[i].plane );
    }
//    copy ( body1, poly1 );
//    copy ( body2, poly2 );
//    drawNewList2d ();
//    draw ( poly1, 0, 1, 1, 1, VM_WIRE );
//    draw ( poly2, 1, 1, 0, 1, VM_WIRE );
//    endNewList ();
    display << "end" << i << NL;
}
