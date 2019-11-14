
#include "ExtraBody.h"
#include "../shev/File.h"
#include "../shev/func1t.h"

typedef SortItem<const void *, nat32> PtrNat;

static const nat64
ver1 = *(const nat64 *) "POLYHED1",
ver2 = *(const nat64 *) "POLYHED2",
ver3 = *(const nat64 *) "POLYHED3";

bool savePYH ( IWriteFile & file, const ExtraBody & body, bool writeBones, const char * text )
{
    try
    {
        // Вначале запишем первую версию ( без рёбер )
        if ( file.write ( &ver3, 1, 8 ) != 8 ) return false;
        static const char buf[80];
        if ( file.write ( text == 0 ? buf : text, 1, 80 ) != 80 ) return false;
        Show<Vert> verts ( body.verts );
        Show<ExtraSide> sides ( body.sides );
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
            const ExtraSide * side = sides.cur();
            if ( file.write ( & side->info, 1, 4 ) != 4 ) return false;
            if ( file.write ( & side->plane, 1, 32 ) != 32 ) return false;
            const nat32 nv = side->blist.size();
            if ( file.write ( & nv, 1, 4 ) != 4 ) return false;
            Show<ExtraBonePtr> list ( side->blist );
            if ( list.top() )
            do
            {
                const ExtraBone * b = list.cur()->bone;
                const Vert * v = & b->b0 == list.cur() ? b->v0 : b->v1;
                const nat k = lasEqu123 ( varr, PtrNat ( v ) );
                if ( k >= nverts ) return false;
                if ( file.write ( & varr[k].tail, 4, 1 ) != 1 ) return false;
            }
            while ( list.next() );
            sides.next();
        }
        if ( writeBones )
        {
            // Теперь запишем вторую версию ( информация о рёбрах )
            if ( file.write ( &ver2, 1, 8 ) != 8 ) return false;
            Show<ExtraBone> bones ( body.bones );
            const nat32 nbones = bones.size();
            if ( file.write ( & nbones, 1, 4 ) != 4 ) return false;
            DynArray<PtrNat> barr ( nbones + nbones );
            j = 0;
            if ( bones.top() )
            do
            {
                const ExtraBone * b = bones.cur();
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
                Show<ExtraBonePtr> list ( sides.cur()->blist );
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
        }
        file.write ( "DATA END", 1, 8 );
    }
    catch(...)
    {
        return false;
    }
    return true;
}

// Функция loadPYH возвращает:
// 0 - ошибка
// 1 - прочитан файл 1-ой версии
// 2 - прочитан файл 2-ой версии

nat loadPYH ( IReadFile & file, ExtraBody & body, char * text )
{
    nat res = 0;
    try
    {
        nat64 vers;
        if ( file.read ( &vers, 8, 1 ) != 1 ) return 0;
        if ( vers != ver1 ) return 0; // "POLYHED1"
        char buf[80];
        if ( file.read ( text == 0 ? buf : text, 1, 80 ) != 80 ) return 0;
        ExtraBody temp;
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
            ExtraSide * side = new ExtraSide;
            temp.sides.addAftLas ( side );
            if ( file.read ( & side->info, 1, 4 ) != 4 ) return 0;
            if ( file.read ( & side->plane, 1, 32 ) != 32 ) return 0;
            if ( file.read ( & nv, 1, 4 ) != 4 ) return 0;
            if ( nv > nverts ) return 0;
            if ( nv == 0 ) continue;
            index[i].resize ( nv );
            if ( file.read ( index[i](), 4, nv ) != nv ) return 0;
        }
        if ( file.read ( &vers, 8, 1 ) == 1 && vers == ver2 ) // "POLYHED2"
        {
            // Вторая версия PYH-файла
            nat32 nbones;
            if ( file.read ( & nbones, 4, 1 ) != 1 ) return 0;
            DynArray<ExtraBone *> barr ( nbones );
            for ( i = 0; i < nbones; ++i )
            {
                nat32 arr[2];
                if ( file.read ( arr, 4, 2 ) != 2 ) return 0;
                temp.bones.addAftCur ( barr[i] = new ExtraBone ( varr[arr[0]], varr[arr[1]], 0, 0 ) );
            }
            Suite<nat32> suite;
            temp.sides.top();
            for ( i = 0; i < nsides; ++i )
            {
                nv = index[i].size();
                suite.resize ( nv );
                if ( file.read ( suite(), 4, nv ) != nv ) return 0;
                ExtraBonePtrList & blist = temp.sides.cur()->blist;
                for ( j = 0; j < nv; ++j )
                {
                    const nat32 k = suite[j];
                    ExtraBone * b = barr[k>>1];
                    if ( k & 1 )
                    {
                        b->c1 = & blist;
                        blist.addAftLas ( & b->b1 );
                    }
                    else
                    {
                        b->c0 = & blist;
                        blist.addAftLas ( & b->b0 );
                    }
                }
                temp.sides.next();
            }
            res = 2;
        }
        else
        {
            // Первая версия PYH-файла
            DynArray<ExtraBoneList> bl ( nverts );
            temp.sides.top();
            for ( i = 0; i < nsides; ++i )
            {
                ExtraBonePtrList & bplist = temp.sides.cur()->blist;
                nv = index[i].size();
                for ( j = 0; j < nv; ++j )
                {
                    const nat32 u0 = index[i][j];
                    const nat32 u1 = index[i][j==nv-1?0:j+1];
                    Vert * v0 = varr[u0];
                    Vert * v1 = varr[u1];
                    ExtraBoneList & blist = bl[u1];
                    if ( blist.top () )
                    do
                    {
                        ExtraBone * b = blist.cur ();
                        if ( b->v1 == v0 )
                        {
                            b->c1 = & bplist;
                            bplist.addAftCur ( & b->b1 );
                            blist.movCurAftLas ( temp.bones );
                            goto m1;
                        }
                    }
                    while ( blist.next () );
                    {
                        ExtraBone * b = new ExtraBone ( v0, v1, & bplist, 0 );
                        bplist.addAftCur ( & b->b0 );
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
