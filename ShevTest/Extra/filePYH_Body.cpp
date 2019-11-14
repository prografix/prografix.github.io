
#include "Body2.h"
#include "../shev/File.h"
#include "../shev/func1t.h"

typedef SortItem<const void *, nat32> PtrNat;

bool savePYH1 ( IWriteFile & file, const Body & body, const char * text )
{
    try
    {
        const char * vers = "POLYHED1";
        if ( file.write ( vers, 1, 8 ) != 8 ) return false;
        static const char buf[80];
        if ( file.write ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        Show<Vert> verts ( body.verts );
        Show<Side> sides ( body.sides );
        const nat32 nverts = verts.size();
        const nat32 nsides = sides.size();
        if ( file.write ( & nverts, 1, 4 ) != 4 ) return false;
        DynArray<PtrNat> varr ( nverts );
        verts.top();
        for ( nat32 i = 0; i < nverts; ++i )
        {
            if ( file.write ( & verts.cur()->point, 24, 1 ) != 1 ) return false;
            varr[i].head = verts.cur();
            varr[i].tail = i;
            verts.next();
        }
        insertSort123 ( varr );
        if ( file.write ( & nsides, 1, 4 ) != 4 ) return false;
        if ( sides.top() )
        do
        {
            const Side * side = sides.cur();
            if ( file.write ( & side->info, 1, 4 ) != 4 ) return false;
            if ( file.write ( & side->plane, 1, 32 ) != 32 ) return false;
            const nat32 nv = side->size();
            if ( file.write ( & nv, 1, 4 ) != 4 ) return false;
            Show<BonePtr> list ( *side );
            if ( list.top() )
            do
            {
                const Bone * b = list.cur()->bone;
                const Vert * v = & b->b0 == list.cur() ? b->v0 : b->v1;
                const nat k = lasEqu123 ( varr, PtrNat ( v ) );
                if ( k >= nverts ) return false;
                if ( file.write ( & varr[k].tail, 4, 1 ) != 1 ) return false;
            }
            while ( list.next() );
        }
        while ( sides.next() );
        file.write ( "DATA END", 1, 8 );
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool savePYH2 ( IWriteFile & file, const Body & body, const char * text )
{
    try
    {
        // Вначале запишем первую версию ( без рёбер )
        const char * vers = "POLYHED1";
        if ( file.write ( vers, 1, 8 ) != 8 ) return false;
        static const char buf[80];
        if ( file.write ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        Show<Vert> verts ( body.verts );
        Show<Side> sides ( body.sides );
        const nat32 nverts = verts.size();
        const nat32 nsides = sides.size();
        if ( file.write ( & nverts, 1, 4 ) != 4 ) return false;
        DynArray<PtrNat> varr ( nverts );
        nat32 i, j;
        verts.top();
        for ( i = 0; i < nverts; ++i )
        {
            if ( file.write ( & verts.cur()->point, 24, 1 ) != 1 ) return false;
            varr[i].head = verts.cur();
            varr[i].tail = i;
            verts.next();
        }
        insertSort123 ( varr );
        if ( file.write ( & nsides, 1, 4 ) != 4 ) return false;
        sides.top();
        for ( i = 0; i < nsides; ++i )
        {
            const Side * side = sides.cur();
            if ( file.write ( & side->info, 1, 4 ) != 4 ) return false;
            if ( file.write ( & side->plane, 1, 32 ) != 32 ) return false;
            const nat32 nv = side->size();
            if ( file.write ( & nv, 1, 4 ) != 4 ) return false;
            Show<BonePtr> list ( *side );
            if ( list.top() )
            do
            {
                const Bone * b = list.cur()->bone;
                const Vert * v = & b->b0 == list.cur() ? b->v0 : b->v1;
                const nat k = lasEqu123 ( varr, PtrNat ( v ) );
                if ( k >= nverts ) return false;
                if ( file.write ( & varr[k].tail, 4, 1 ) != 1 ) return false;
            }
            while ( list.next() );
            sides.next();
        }
        // Теперь запишем вторую версию ( информация о рёбрах )
        const char * vers2 = "POLYHED2";
        if ( file.write ( vers2, 1, 8 ) != 8 ) return false;
        Show<Bone> bones ( body.bones );
        const nat32 nbones = bones.size();
        if ( file.write ( & nbones, 1, 4 ) != 4 ) return false;
        DynArray<PtrNat> barr ( nbones + nbones );
        j = 0;
        if ( bones.top() )
        do
        {
            const Bone * b = bones.cur();
            i = lasEqu123 ( varr, PtrNat ( b->v0 ) );
            if ( i == varr.size() ) return false;
            if ( file.write ( & varr[i].tail, 1, 4 ) != 4 ) return false;
            i = lasEqu123 ( varr, PtrNat ( b->v1 ) );
            if ( i == varr.size() ) return false;
            if ( file.write ( & varr[i].tail, 1, 4 ) != 4 ) return false;
            barr[j].head = & b->b0;
            barr[j].tail = j;
            ++j;
            barr[j].head = & b->b1;
            barr[j].tail = j;
            ++j;
        }
        while ( bones.next() );
        insertSort123 ( barr );
        sides.top();
        for ( i = 0; i < nsides; ++i )
        {
            Show<BonePtr> list ( *sides.cur() );
            if ( list.top() )
            do
            {
                j = lasEqu123 ( barr, PtrNat ( list.cur() ) );
                if ( j >= barr.size() ) return false;
                if ( file.write ( & barr[j].tail, 4, 1 ) != 1 ) return false;
            }
            while ( list.next() );
            sides.next();
        }
        file.write ( "DATA END", 1, 8 );
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool loadPYH1 ( IReadFile & file, Body & body, char * text )
{
    try
    {
        nat32 vers[2];
        if ( file.read ( vers, 4, 2 ) != 2 ) return false;
        if ( vers[0] != 1498173264 || vers[1] != 826557768 ) return false;
        char buf[80];
        if ( file.read ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        Body temp;
        nat32 i, nverts, nfacets, nv;
        if ( file.read ( & nverts, 1, 4 ) != 4 ) return false;
        DynArray<BoneList> bl ( nverts );
        DynArray<Vert *> varr ( nverts );
        for ( i = 0; i < nverts; ++i )
        {
            Vector3d v;
            if ( file.read ( & v, 24, 1 ) != 1 ) return false;
            temp.verts.addAftLas ( varr[i] = new Vert ( v ) );
        }
        if ( file.read ( & nfacets, 1, 4 ) != 4 ) return false;
        Suite<nat32> index;
        for ( i = 0; i < nfacets; ++i )
        {
            Side * side = new Side;
            temp.sides.addAftLas ( side );
            if ( file.read ( & side->info, 1, 4 ) != 4 ) return false;
            if ( file.read ( & side->plane, 1, 32 ) != 32 ) return false;
            if ( file.read ( & nv, 1, 4 ) != 4 ) return false;
            if ( nv > nverts ) return false;
            if ( nv == 0 ) continue;
            index.resize ( nv );
            if ( file.read ( index(), 4, nv ) != nv ) return false;
            for ( nat32 j = 0; j < nv; ++j )
            {
                const nat32 u0 = index[j];
                const nat32 u1 = index[j==nv-1?0:j+1];
                Vert * v0 = varr[u0];
                Vert * v1 = varr[u1];
                BoneList & blist = bl[u1];
                if ( blist.top () )
                do
                {
                    Bone * b = blist.cur ();
                    if ( b->v1 == v0 )
                    {
                        b->s1 = side;
                        side->addAftCur ( & b->b1 );
                        blist.movCurAftLas ( temp.bones );
                        goto m1;
                    }
                }
                while ( blist.next () );
                {
                    Bone * b = new Bone ( v0, v1, side, 0 );
                    side->addAftCur ( & b->b0 );
                    bl[u0].addAftCur ( b );
                }
    m1:;    }
        }
        for ( i = 0; i < nverts; ++i ) bl[i].movAllAftLas ( temp.bones );
        swap ( temp, body );
    }
    catch(...)
    {
        return false;
    }
    return true;
}

// Функция loadPYH2 возвращает:
// 0 - ошибка
// 1 - прочитан файл 1-ой версии
// 2 - прочитан файл 2-ой версии

nat loadPYH2 ( IReadFile & file, Body & body, char * text )
{
    nat res = 0;
    try
    {
        nat32 vers[2];
        if ( file.read ( vers, 4, 2 ) != 2 ) return 0;
        if ( vers[0] != 1498173264 || vers[1] != 826557768 ) return 0; // "POLYHED1"
        char buf[80];
        if ( file.read ( text == 0 ? buf : text, 1, 80 ) != 80 ) return 0;
        Body temp;
        nat32 i, j, nverts, nsides, nv;
        if ( file.read ( & nverts, 1, 4 ) != 4 ) return 0;
        DynArray<Vert *> varr ( nverts );
        for ( i = 0; i < nverts; ++i )
        {
            Vector3d v;
            if ( file.read ( & v, 24, 1 ) != 1 ) return 0;
            temp.verts.addAftLas ( varr[i] = new Vert ( v ) );
        }
        if ( file.read ( & nsides, 1, 4 ) != 4 ) return 0;
        DynArray<DynArray<nat32> > index ( nsides );
        for ( i = 0; i < nsides; ++i )
        {
            Side * side = new Side;
            temp.sides.addAftLas ( side );
            if ( file.read ( & side->info, 1, 4 ) != 4 ) return 0;
            if ( file.read ( & side->plane, 1, 32 ) != 32 ) return 0;
            if ( file.read ( & nv, 1, 4 ) != 4 ) return 0;
            if ( nv > nverts ) return 0;
            if ( nv == 0 ) continue;
            index[i].resize ( nv );
            if ( file.read ( index[i](), 4, nv ) != nv ) return 0;
        }
        if ( file.read ( vers, 4, 2 ) == 2 && vers[0] == 1498173264 || vers[1] == 843334984 ) // "POLYHED2"
        {
            // Вторая версия PYH-файла
            nat32 nbones;
            if ( file.read ( & nbones, 4, 1 ) != 1 ) return 0;
            DynArray<Bone *> barr ( nbones );
            for ( i = 0; i < nbones; ++i )
            {
                nat32 arr[2];
                if ( file.read ( arr, 4, 2 ) != 2 ) return 0;
                temp.bones.addAftCur ( barr[i] = new Bone ( varr[arr[0]], varr[arr[1]], 0, 0 ) );
            }
            Suite<nat32> suite;
            temp.sides.top();
            for ( i = 0; i < nsides; ++i )
            {
                nv = index[i].size();
                suite.resize ( nv );
                if ( file.read ( suite(), 4, nv ) != nv ) return 0;
                Side * side = temp.sides.cur();
                for ( j = 0; j < nv; ++j )
                {
                    const nat32 k = suite[j];
                    Bone * b = barr[k>>1];
                    if ( k & 1 )
                    {
                        b->s1 = side;
                        side->addAftLas ( & b->b1 );
                    }
                    else
                    {
                        b->s0 = side;
                        side->addAftLas ( & b->b0 );
                    }
                }
                temp.sides.next();
            }
            res = 2;
        }
        else
        {
            // Первая версия PYH-файла
            DynArray<BoneList> bl ( nverts );
            temp.sides.top();
            for ( i = 0; i < nsides; ++i )
            {
                Side * side = temp.sides.cur();
                nv = index[i].size();
                for ( j = 0; j < nv; ++j )
                {
                    const nat32 u0 = index[i][j];
                    const nat32 u1 = index[i][j==nv-1?0:j+1];
                    Vert * v0 = varr[u0];
                    Vert * v1 = varr[u1];
                    BoneList & blist = bl[u1];
                    if ( blist.top () )
                    do
                    {
                        Bone * b = blist.cur ();
                        if ( b->v1 == v0 )
                        {
                            b->s1 = side;
                            side->addAftCur ( & b->b1 );
                            blist.movCurAftLas ( temp.bones );
                            goto m1;
                        }
                    }
                    while ( blist.next () );
                    {
                        Bone * b = new Bone ( v0, v1, side, 0 );
                        side->addAftCur ( & b->b0 );
                        bl[u0].addAftCur ( b );
                    }
        m1:;    }
                temp.sides.next();
            }
            for ( i = 0; i < nverts; ++i ) bl[i].movAllAftLas ( temp.bones );
            res = 1;
        }
        swap ( temp, body );
    }
    catch(...)
    {
        return 0;
    }
    return res;
}

#include "../shev/RealFile.h"

nat loadPYH2 ( const char * filename, Body & body, char * text )
{
    return loadPYH2 ( RealFile ( filename, "rb" ), body, text );
}
