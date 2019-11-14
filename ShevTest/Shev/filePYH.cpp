
#include "File.h"
#include "Polyhedron.h"

bool savePYH ( IWriteFile & file, const Polyhedron & poly, const char * text )
{
    try
    {
        const char * vers = "POLYHED1";
        if ( file.write ( vers, 1, 8 ) != 8 ) return false;
        static const char buf[80];
        if ( file.write ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        const nat32 nverts = poly.vertex.size();
        if ( file.write ( & nverts, 1, 4 ) != 4 ) return false;
        if ( file.write ( poly.vertex(), 24, nverts ) != nverts ) return false;
        const nat32 nfacets = poly.facet.size();
        if ( file.write ( & nfacets, 1, 4 ) != 4 ) return false;
        for ( nat32 i = 0; i < nfacets; ++i )
        {
            const Facet & faceti = poly.facet[i];
            if ( file.write ( & faceti.info, 1, 4 ) != 4 ) return false;
            if ( file.write ( & faceti.plane, 1, 32 ) != 32 ) return false;
            if ( file.write ( & faceti.nv, 1, 4 ) != 4 ) return false;
            if ( file.write ( faceti.index(), 4, faceti.nv ) != faceti.nv) return false;
        }
        file.write ( "DATA END", 1, 8 );
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool loadPYH ( IReadFile & file, Polyhedron & poly, char * text )
{
    try
    {
        nat32 vers[2];
        if ( file.read ( vers, 4, 2 ) != 2 ) return false;
        if ( vers[0] != 1498173264 || vers[1] != 826557768 ) return false;
        char buf[80];
        if ( file.read ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        Polyhedron temp;
        nat32 nverts, nfacets;
        if ( file.read ( & nverts, 1, 4 ) != 4 ) return false;
        temp.vertex.resize ( nverts );
        if ( file.read ( temp.vertex(), 24, nverts ) != nverts ) return false;
        if ( file.read ( & nfacets, 1, 4 ) != 4 ) return false;
        temp.facet.resize ( nfacets );
        for ( nat32 i = 0; i < nfacets; ++i )
        {
            Facet & faceti = temp.facet[i];
            if ( file.read ( & faceti.info, 1, 4 ) != 4 ) return false;
            if ( file.read ( & faceti.plane, 1, 32 ) != 32 ) return false;
            if ( file.read ( & faceti.nv, 1, 4 ) != 4 ) return false;
            if ( faceti.nv > nverts ) return false;
            if ( faceti.nv > 0 )
            {
                faceti.index.resize ( 3 * faceti.nv + 1 );
                if ( file.read ( faceti.index(), 4, faceti.nv ) != faceti.nv ) return false;
            }
        }
        temp.linkFacets ();
        _swap ( temp, poly );
    }
    catch(...)
    {
        return false;
    }
    return true;
}