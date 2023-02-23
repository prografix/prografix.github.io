
#include "math.h"

#include "RealFile.h"
#include "Poly3gon.h"
#include "Vector3f.h"
#include "fileSTL.h"
#include "GraphicInterface.h"
#include "GIStorage.h"

GraphicInterface * importSTL ( const char * filename, const int surface )
{
    RealFile file ( filename, "rb" );
    if ( ! file.isValid() ) return 0;
    Poly3gon<Vector3f, Vector3f> poly ( 0, 0 );
    nat ntext;
    char text[80];
    if ( ! loadSTLA ( file, ntext, text, poly ) )
    {
        file.rewind();
        if ( ! loadSTLB ( file, ntext, text, poly ) ) return 0;
    }

    GIStorage * gi = new GIStorage;

    gi->NewListCompile(1);
    const nat nverts = poly.vertex.size();
    if ( nverts > 0 )
    {
        const Vector3f * vertex = &poly.vertex[0];
        float x0, x1, y0, y1, z0, z1;
        x0 = x1 = vertex->x;
        y0 = y1 = vertex->y;
        z0 = z1 = vertex->z;
	    for ( nat j = nverts; --j > 0; )
	    {
		    const Vector3f & v = vertex[j];
            if ( x0 > v.x ) x0 = v.x; else
            if ( x1 < v.x ) x1 = v.x;
            if ( y0 > v.y ) y0 = v.y; else
            if ( y1 < v.y ) y1 = v.y;
            if ( z0 > v.z ) z0 = v.z; else
            if ( z1 < v.z ) z1 = v.z;
        }
        const float dx = x1 - x0;
        const float dy = y1 - y0;
        const float dz = z1 - z0;
        float scale = sqrtf ( dx * dx + dy * dy + dz * dz );
        if ( scale > 0 )
        {
            scale = 2.0f / scale;
            gi->Scale ( scale, scale, scale );
        }
        gi->Translate ( -0.5f * ( x0 + x1 ), -0.5f * ( y0 + y1 ), -0.5f * ( z0 + z1 ) );
	}

	gi->BeginTriangles();
    if ( surface == 0 )
    {
        nat i, j;
	    DynArray<Vector3f> norm ( nverts );
	    for ( j = 0; j < nverts; ++j )
	    {
		    norm[j].null();
	    }
        for ( i = 0; i < poly.side.size(); ++i )
        {
		    const Trigon<Vector3f> & side = poly.side[i];
		    const Vector3f n = ( poly.vertex[side.b] - poly.vertex[side.a] ) %
				         ( poly.vertex[side.c] - poly.vertex[side.b] );
            norm[side.a] += n;
            norm[side.b] += n;
            norm[side.c] += n;
	    }
	    for ( i = 0; i < poly.side.size(); ++i )
	    {
		    const Trigon<Vector3f> & side = poly.side[i];
		    const Vector3f & pa = poly.vertex[side.a];
		    const Vector3f & pb = poly.vertex[side.b];
		    const Vector3f & pc = poly.vertex[side.c];
		    const Vector3f & na = norm[side.a];
		    const Vector3f & nb = norm[side.b];
		    const Vector3f & nc = norm[side.c];
			gi->Normal ( na.x, na.y, na.z );
			gi->Vertex ( pa.x, pa.y, pa.z );
			gi->Normal ( nb.x, nb.y, nb.z );
			gi->Vertex ( pb.x, pb.y, pb.z );
			gi->Normal ( nc.x, nc.y, nc.z );
			gi->Vertex ( pc.x, pc.y, pc.z );
	    }
    }
    else
    {
	    for ( nat i = 0; i < poly.side.size(); ++i )
	    {
		    Trigon<Vector3f> & side = poly.side[i];
		    const Vector3f & pa = poly.vertex[side.a];
		    const Vector3f & pb = poly.vertex[side.b];
		    const Vector3f & pc = poly.vertex[side.c];
		    const Vector3f & vn = side.info;
			gi->Normal ( vn.x, vn.y, vn.z );
			gi->Vertex ( pa.x, pa.y, pa.z );
			gi->Vertex ( pb.x, pb.y, pb.z );
			gi->Vertex ( pc.x, pc.y, pc.z );
	    }
    }
    gi->End();
    gi->EndList();

    gi->NextStart(0);
    gi->ClearColorBuffer();
    gi->ClearDepthBuffer();
    gi->EnableNormalize();
    gi->EnableDepthTest();
    gi->LightAmbient (LIGHT_1, 1.0, 1.0, 1.0, 1.0);
	gi->LightDiffuse (LIGHT_1, 1.0, 1.0, 1.0, 1.0);
	gi->LightSpecular(LIGHT_1, 1.0, 1.0, 1.0, 1.0);
	gi->EnableLight(LIGHT_1);
	gi->EnableLighting();
    gi->PushMatrix();
    gi->CallList(1);
    gi->PopMatrix(); 
    return gi;
}