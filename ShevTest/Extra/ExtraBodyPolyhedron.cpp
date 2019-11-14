#if 0
#include <ppl.h>
#include <math.h>
#include "octMath/octPolyhedron.h"
#include "octMath/octSortingT.h"
#include "ShevArray.h"
#include "Lists.h"
#include "ExtraBody.h"

//#include "Display.h"

using namespace oct;  

bool copy ( const OctPolyhedron & poly, ExtraBodyStor & stor, ExtraBody & body )
{
    try
    {
//double t0 = timeInSec();
        nat i;
        body.makeVoid ( stor );
        DynArray<Vert *> varr ( poly.vertexCount() );
        body.verts.resize ( varr.size(), stor.verts );
        body.verts.top();
        for ( i = 0; i < varr.size(); ++i )
        {
            ( varr[i] = body.verts.cur() )->point = poly.vertex(i);
            body.verts.next();
        }
//double t1 = timeInSec();
        const nat ns = poly.faceCount();
        DynArray<ExtraBonePtrList *> sarr ( ns );
        body.sides.resize ( ns, stor.sides ).top();
        for ( i = 0; i < ns; ++i )
        {
            ExtraSide * s = body.sides.cur();
            s->blist.side = s;
            sarr[i] = & s->blist;
            s->plane = poly.face(i).plane;
            body.sides.next();
        }
//double t2 = timeInSec();
        const nat nb = poly.edgeCount();
        DynArray<SortItem<Set3<nat>, Set2<ExtraBonePtr *, ExtraBonePtrList **> > > bp ( 2*nb );
        body.bones.resize ( nb, stor.bones ).top();
        for ( i = 0; i < nb; ++i )
        {
            const Edge & b1 = poly.edge(i);
            ExtraBone * b2 = body.bones.cur();
            b2->v0 = b1.iVertex0 < varr.size() ? varr[b1.iVertex0] : 0;
            b2->v1 = b1.iVertex1 < varr.size() ? varr[b1.iVertex1] : 0;
            b2->c0 = b2->c1 = 0;
            SortItem<Set3<nat>, Set2<ExtraBonePtr *, ExtraBonePtrList **> > & si1 = bp[i+i];
            si1.head.a = b1.iVertex0;
            si1.head.b = b1.iVertex1;
            si1.head.c = 0;
            si1.tail.a = & b2->b0;
            si1.tail.b = & b2->c0;
            SortItem<Set3<nat>, Set2<ExtraBonePtr *, ExtraBonePtrList **> > & si2 = bp[i+i+1];
            si2.head.a = b1.iVertex1;
            si2.head.b = b1.iVertex0;
            si2.head.c = 0;
            si2.tail.a = & b2->b1;
            si2.tail.b = & b2->c1;
            body.bones.next();
        }
//double t3 = timeInSec();
        quickSort123 ( bp(), bp.size() );
//double t4 = timeInSec();
        /*if(1)
        {
        
            Concurrency::parallel_for ( 0u, ns, [&](nat i)
            {
                initSide ( poly.face(i), sarr[i], bp );
            } );
        }
        else*/
        {
            SortItem<Set3<nat>, Set2<ExtraBonePtr *, ExtraBonePtrList **> > si;
            si.head.c = 0;
            body.sides.top();
            for ( i = 0; i < ns; ++i )
            {
                const Face & face = poly.face(i);
                ExtraSide * s = body.sides.cur();
                for ( nat i = 0; i < face.size(); ++i )
                {
                    si.head.a = face.vertex(i);
                    si.head.b = face.nextVertex(i);
                    const nat j = lasEqu123 ( bp, si );
                    if ( j == bp.size() ) return false;
                    s->blist.addAftLas ( bp[j].tail.a );
                    *bp[j].tail.b = &s->blist;
                    bp[j].head.c = 1;
                }
                body.sides.next();
            }
        }
/*double t5 = timeInSec();
display << t1-t0 << NL;
display << t2-t1 << NL;
display << t3-t2 << NL;
display << t4-t3 << NL;
display << t5-t4 << NL;*/
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool copy ( const ExtraBody & body, OctPolyhedron & poly )
{
    Show<Vert> verts ( body.verts );
    Show<ExtraSide> sides ( body.sides );
	poly.resizeVertexes ( body.verts.size(), false );
	poly.resizeEdges ( body.bones.size(), false );
	poly.resizeFaces ( body.sides.size(), false );
    
    typedef SortItem<const void *, nat> PtrNat;
    DynArray<PtrNat> pna ( verts.size() );
    verts.top();
    for ( nat i = 0; i < verts.size(); ++i )
    {
        poly.vertex(i) = verts.cur()->point;
        pna[i].head = verts.cur();
        pna[i].tail = i;
        verts.next();
    }
    quickSort123 ( pna(), pna.size() );

    sides.top();
    for ( nat i = 0; i < sides.size(); ++i )
    {
		Face & facet = poly.face(i);
        const ExtraSide * s = sides.cur();
        facet.plane = s->plane;
        Show<ExtraBonePtr> side ( s->blist );
        if ( side.top() )
        {
		    const nat n = side.size();
            facet.resize ( n );
            for ( nat j = 0; j < n; ++j )
            {
                const nat k = lasEqu123 ( pna, PtrNat ( side.cur()->vert0() ) );
                if ( k >= pna.size() ) return false;
                facet.vertex(j) = pna[k].tail;
                side.next();
            }
        }
        sides.next ();
    }

    return true;
}

#endif