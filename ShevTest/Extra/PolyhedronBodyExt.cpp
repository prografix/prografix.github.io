
#include "math.h"
// Shev
#include "../shev/Vector3d.h"
#include "../shev/func1t.h"
#include "../shev/Polyhedron.h"
#include "BodyExt.h"

BodyExt & copy ( const Polyhedron & poly, BodyExtStor & stor, BodyExt & body )
{
    nat i;
    body.makeVoid ( stor );
    DynArray<SideExt *> sarr ( poly.facet.size() );
    DynArray<Vert *> varr ( poly.vertex.size() );

    body.verts.resize ( varr.size(), stor.verts );
    body.verts.top();
    for ( i = 0; i < poly.vertex.size(); ++i )
    {
        ( varr[i] = body.verts.cur() )->point = poly.vertex[i];
        body.verts.next();
    }

    body.sides.resize ( sarr.size(), stor.sides );
    body.sides.top();
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        ( sarr[i] = body.sides.cur() )->plane = poly.facet[i].plane;
        body.sides.next();
    }

    for ( i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        sarr[i]->info = facet.info;
        for ( nat j = 0; j < facet.nv; ++j )
        {
            const nat g = facet.nv + 1 + j;
            const nat l = facet.index[g];
            const nat m = facet.index[g+facet.nv];
            if ( l >= poly.facet.size() || m >= poly.facet[l].nv )
            {
                return body.makeVoid ( stor );
            }
            if ( l > i || ( l == i && j < m ) )
            {
                BoneExt * b = body.bones.addNewAftLas ( stor.bones );
                b->v0 = varr[facet.index[j]];
                b->v1 = varr[facet.index[j+1]];
                b->c0 = & sarr[i]->blist;
                b->c1 = & sarr[l]->blist;
                sarr[i]->blist.addAftCur ( & b->b0 );
                sarr[l]->blist.addAftCur ( & b->b1 );
                b->b0.info = j;
                b->b1.info = m;
            }
        }
    }

    for ( i = 0; i < sarr.size(); ++i ) sarr[i]->blist.sort123();

    return body;
}

BodyExt & copy ( const Polyhedron & poly, BodyExt & body )
{
    return copy ( poly, BodyExtStor(), body );
}

bool copy ( const BodyExt & body, Polyhedron & poly )
{
    Show<Vert> verts ( body.verts );
    Show<SideExt> sides ( body.sides );
    Polyhedron temp ( verts.size(), sides.size() );

    typedef SortItem<const void *, nat> PtrNat;
    DynArray<PtrNat> pna ( verts.size() );
    nat i;
    verts.top();
    for ( i = 0; i < temp.vertex.size(); ++i )
    {
        temp.vertex[i] = verts.cur()->point;
        pna[i].head = verts.cur();
        pna[i].tail = i;
        verts.next();
    }
    quickSort123 ( pna );

    sides.top();
    for ( i = 0; i < temp.facet.size(); ++i )
    {
        Facet & facet = temp.facet[i];
        const SideExt * s = sides.cur();
        facet.plane = s->plane;
        facet.info = s->info;
        Show<BoneExtPtr> side ( s->blist );
        if ( side.top() )
        {
            facet.resize ( side.size() );
            for ( nat j = 0; j < facet.nv; ++j )
            {
                const nat k = lasEqu123 ( pna, PtrNat ( side.cur()->vert0() ) );
                if ( k >= temp.vertex.size() ) return false;
                facet.index[j] = pna[k].tail;
                side.next();
            }
        }
        sides.next ();
    }

    temp.linkFacets();
    _swap ( temp, poly );
    return true;
}
