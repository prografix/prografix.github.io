
#include "math.h"

#include "RealFile.h"
#include "fileOBJ.h"
#include "Vector3f.h"
#include "GraphicInterface.h"
#include "GIStorage.h"

class PutWavefrontOBJg : public PutWavefrontOBJ
{
    nat vsize, vcount;
    Vector3f * vert;
    nat nsize, ncount;
    Vector3f * norm;
    bool err;
    GraphicInterface & gi;
public:
    PutWavefrontOBJg ( GraphicInterface & g );
   ~PutWavefrontOBJg ();
    virtual void putVertex ( float x, float y, float z );
    virtual void putNormal ( float x, float y, float z );
    virtual void putFacet ( nat nv, const nat * vert, const nat * norm );
    void setScale() const;
};

PutWavefrontOBJg::PutWavefrontOBJg ( GraphicInterface & g ) :
vsize(256), vcount(0), vert(new Vector3f[256]),
nsize(256), ncount(0), norm(new Vector3f[256]),
err(false), gi(g)
{}

PutWavefrontOBJg::~PutWavefrontOBJg()
{
    delete[] norm;
    delete[] vert;
}

void PutWavefrontOBJg::putVertex ( float x, float y, float z )
{
    if ( err ) return;
    if ( vcount >= vsize )
    {
        nat n = vsize;
        vsize += vsize;
        Vector3f * v = new Vector3f[vsize];
        for ( nat i = 0; i < n; ++i )
        {
            v[i] = vert[i];
        }
        delete[] vert;
        vert = v;
    }
    vert[vcount++] = Vector3f ( x, y, z );
}

void PutWavefrontOBJg::putNormal ( float x, float y, float z )
{
    if ( err ) return;
    if ( ncount >= nsize )
    {
        nat n = nsize;
        nsize += nsize;
        Vector3f * v = new Vector3f[nsize];
        for ( nat i = 0; i < n; ++i )
        {
            v[i] = norm[i];
        }
        delete[] norm;
        norm = v;
    }
    norm[ncount++] = Vector3f ( x, y, z );
}

void PutWavefrontOBJg::putFacet ( nat nv, const nat * iv, const nat * in )
{
    if ( err ) return;
    if ( nv < 3 )
    {
        err = true;
        return;
    }
    switch ( nv )
    {
    case 3: gi.BeginTriangles(); break;
    case 4: gi.BeginQuads(); break;
    default:gi.BeginPolygon(); break;
    }
    if ( in == 0 )
    {
        nat i;
// Проверка данных
        for ( i = 0; i < nv; ++i )
        {
            if ( iv[i] > 0 )
            {
                const nat j = iv[i] - 1;
                if ( j < vcount ) continue;
            }
            err = true;
            return;
        }
// Вычисление нормали
        Vector3f n ( 0., 0., 0. );
        for ( i = 0; i < nv; ++i )
        {
            const nat j1 = i > 0 ? iv[i-1] - 1 : iv[nv-1] - 1;
            const nat j2 = iv[i] - 1;
            n += vert[j1] % vert[j2];
        }
        gi.Normal ( n.x, n.y, n.z );
// Передача вершин
        for ( i = 0; i < nv; ++i )
        {
            const nat j = iv[i] - 1;
            const Vector3f & v = vert[j];
            gi.Vertex ( v.x, v.y, v.z );
        }
    }
    else
    {
        for ( nat i = 0; i < nv; ++i )
        {
            if ( iv[i] > 0 && in[i] > 0 )
            {
                const nat jv = iv[i] - 1;
                const nat jn = in[i] - 1;
                if ( jv < vcount && jn < ncount )
                {
                    const Vector3f & n = norm[jn];
                    const Vector3f & v = vert[jv];
                    gi.Normal ( n.x, n.y, n.z );
                    gi.Vertex ( v.x, v.y, v.z );
                    continue;
                }
            }
            err = true;
            return;
        }
    }
    gi.End();
}

void PutWavefrontOBJg::setScale () const
{
    if ( vcount > 1 )
    {
        float x0, x1, y0, y1, z0, z1;
        x0 = x1 = vert->x;
        y0 = y1 = vert->y;
        z0 = z1 = vert->z;
	    for ( nat i = 1; i < vcount; ++i )
	    {
		    const Vector3f & v = vert[i];
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
            gi.Scale ( scale, scale, scale );
        }
        gi.Translate ( -0.5f * ( x0 + x1 ), -0.5f * ( y0 + y1 ), -0.5f * ( z0 + z1 ) );
	}
}

GraphicInterface * importOBJ ( const char * filename, const int surface )
{
    RealFile file ( filename, "rb" );
    if ( ! file.isValid() ) return 0;
    GIStorage * gi = new GIStorage;
    PutWavefrontOBJg obj ( *gi );
    gi->NewListCompile(1);
    if ( ! loadOBJ ( file, obj ) )
    {
        delete gi;
        return 0;
    }
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
    obj.setScale();
    gi->CallList(1);
    gi->PopMatrix();
    return gi;
}