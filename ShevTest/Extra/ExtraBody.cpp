
#include "math.h"
#include "../shev/Lists.h"
#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "vert.h"
#include "ExtraBody.h"
#include "../shev/trans.h"
#include "../shev/func1t.h"
#include "../shev/func2d.h"
#include "../shev/Polyhedron.h"
#include "../shev/intersect2d.h"
#include "../test/display.h"

ExtraBonePtrList & ExtraBonePtrList::setAllSelf ()
{
    if ( top() ) do setSelf(); while ( next() );
    return *this;
}
/*
Def<Vector3d> ExtraSide::centerOfMass() const
{
    Show<ExtraBonePtr> show ( *this );
    if ( show.top() )
    {
        const ExtraBone * b = show.cur()->bone;
        if ( show.size() < 3 )
        {
            return 0.5 * ( b->v0->point + b->v1->point );
        }
        const Vector3d v = & b->b0 == show.cur() ? b->v0->point : b->v1->point;
        double s = 0.;
        Vector3d o ( 0.,0.,0. );
        while ( show.next() )
        {
            if ( show.curIsLas() ) break;
            const ExtraBone * b = show.cur()->bone;
            const Vert * v0, * v1;
            if ( & b->b0 == show.cur() )
            {
                v0 = b->v0;
                v1 = b->v1;
            }
            else
            {
                v0 = b->v1;
                v1 = b->v0;
            }
            const double t = plane.norm * ( ( v0->point - v ) % ( v1->point - v ) );
            s += t;
            o += t * ( v0->point + v1->point + v );
        }
        if ( s )
        {
            o /= 3;
            o /= s;
            return o;
        }
    }
    return Def<Vector3d>();
}

double ExtraSide::area() const
{
    if ( size() < 3 ) return 0.;
    Vector3d v ( 0., 0., 0. );
    Show<ExtraBonePtr> show ( *this );
    show.top();
    do
    {
        const ExtraBone * b = show.cur()->bone;
        v += & b->b0 == show.cur() ? b->v0->point % b->v1->point : b->v1->point % b->v0->point;
    }
    while ( show.next() );
    return 0.5 * ( v * plane.norm );
}

double ExtraSide::perimeter() const // Периметр
{
    if ( ! size() ) return 0.;
    double p = 0.;
    Show<ExtraBonePtr> show ( *this );
    show.top();
    do
    {
        const ExtraBone * b = show.cur()->bone;
        p += norm2 ( b->v1->point - b->v0->point );
    }
    while ( show.next() );
    return p;
}

void ExtraSide::initPlane()
{
    plane.norm.fill();
    plane.dist = 0.;
    if ( ! top() ) return;
    do
    {
        Vert * v0 = vert0();
        Vert * v1 = vert1();
        plane.norm += v0->point % v1->point;
    }
    while ( next() );
    plane.norm.setNorm2();
    plane.dist = - ( plane.norm * cur()->bone->v0->point );
}

// Свойства многогранника

double ExtraBody::volume() const // Объём
{
    double v = 0.;
    Show<ExtraSide> show ( sides );
    if ( show.top () )
    do
    {
        const ExtraSide * side = show.cur ();
        v -= side->area() * side->plane.dist;
    }
    while ( show.next () );
    return v / 3.;
}

Def<Vector3d> ExtraBody::centerOfMass() const // Центр масс
{
    double m = 0.;
    Vector3d c ( 0, 0, 0 );
    Show<ExtraSide> sides ( sides );
    if ( sides.top() )
    do
    {
        Show<ExtraBonePtr> side ( * sides.cur() );
        if ( !side.top() ) continue;
        const ExtraBone * b = side.cur()->bone;
        const Vector3d v1 = & b->b0 == side.cur() ? b->v0->point : b->v1->point;
        while ( side.next() )
        {
            if ( side.curIsLas() ) break;
            const ExtraBone * b = side.cur()->bone;
            Vector3d v2, v3;
            if ( & b->b0 == side.cur() )
            {
                v2 = b->v0->point;
                v3 = b->v1->point;
            }
            else
            {
                v2 = b->v1->point;
                v3 = b->v0->point;
            }
            const double p = ( ( v1 % v2 ) * v3 ) / 6.;
            c += ( v1 + v2 + v3 ) * ( p / 4.);
            m += p;
        }
    }
    while ( sides.next() );
    return m == 0 ? Def<Vector3d>() : Def<Vector3d> ( c /= m );
}
*/
namespace ////////////////////////////////////////////////////////////////////////////
{

void divideSide ( ExtraSide * s, ExtraBoneList & link, const Vector3d & v, ExtraSideList & sides, ExtraBoneList & bones )
{
    while ( link.size() > 0 )
    {
        if ( link.size() == 1 )
        {
            link.movAllAftLas ( bones );
            break;
        }
        ExtraBone * a = link.top();
        double da = v * a->v0->point;
        while ( link.next() )
        {
            ExtraBone * p = link.cur();
            double d = v * p->v0->point;
            if ( da > d ) a = p, da = d;
        }
        link.makeFir ( a );
        ExtraBone * b = link.top();
        double db = v * b->v1->point;
        while ( link.next() )
        {
            ExtraBone * p = link.cur();
            double d = v * p->v1->point;
            if ( db > d ) b = p, db = d;
        }
        if ( a == b )
        {
            link.movFirAftLas ( bones );
            continue;
        }
        _swap ( a->v1, b->v1 );
        s->blist.makeLas ( & b->b0 );
        link.makeLas ( b );
        s->blist.jump ( & a->b0 );
        link.jump ( a );
        ExtraSide * t = new ExtraSide ( s->plane );
        t->info = s->info;
        sides.addBefFir ( t );
        s->blist.movAftCurAftCur ( t->blist );
        List<ExtraBone> temp;
        link.movAftCurAftCur ( temp );
        if ( s->blist.size() < t->blist.size() )
        {// Для скорости поменяем грани местами
            swap ( s->blist, t->blist );
            swap ( link, temp );
        }
        t->blist.setAllSelf();
        divideSide ( t, temp, v, sides, bones );
    }
}

void recon ( ExtraSideList & sides, ExtraBoneList & bones, ExtraBonePtrList & side, const Plane3d & plane, ExtraBodyStor & stor )
{
    // Закрываем пробелы рёбрами
    List<ExtraBone> link;
    ExtraSide * s = sides.cur();
    Vert * v0 = s->blist.las()->vert1();
    s->blist.top();
    do
    {
        Vert * v1 = s->blist.cur()->vert0();
        if ( v0 != v1 )
        {
            ExtraBone * b = link.addNewAftLas ( stor.bones );
            b->v0 = v0;
            b->v1 = v1;
            b->c0 = & s->blist;
            b->c1 = 0;
            s->blist.addBefCur ( & b->b0 ).next();
            side.addAftLas ( & b->b1 );
        }
        v0 = s->blist.cur()->vert1();
    }
    while ( s->blist.next() );
    if ( s->holes.top() )
    for(;;)
    {
        ExtraBonePtrList * slist = s->holes.cur();
        if ( slist->size() > 0 )
        {
            if ( slist->flag )
            {
                Vert * v0 = slist->las()->vert1();
                slist->top();
                do
                {
                    Vert * v1 = slist->cur()->vert0();
                    if ( v0 != v1 )
                    {
                        ExtraBone * b = link.addNewAftLas ( stor.bones );
                        b->v0 = v0;
                        b->v1 = v1;
                        b->c0 = slist;
                        b->c1 = 0;
                        slist->addBefCur ( & b->b0 ).next();
                        side.addAftLas ( & b->b1 );
                    }
                    v0 = slist->cur()->vert1();
                }
                while ( slist->next() );
            }
            if ( ! s->holes.next() ) break;
        }
        else
        {
            const bool isLas = s->holes.curIsLas();
            s->holes.movCurAftLas ( stor.bplist );
            if ( isLas ) break;
        }
    }
    // Если нужно - делим грань на части
    const Vector3d v = s->plane.norm % plane.norm;
    divideSide ( s, link, v, sides, bones );
}

double qdis ( const ExtraBone & bone, const Vector3d & p, bool & isVert )
{
    const Vector3d & a = bone.v0->point;
    const Vector3d & b = bone.v1->point;
    const Vector3d u = b - a;
    const Vector3d w = p - a;
    const double s = u * w;
    if ( s <= 0 )
    {
        isVert = 1;
        return w * w;
    }
    isVert = 0;
    const double t = u * u;
    if ( s >= t ) return 1e300;
    return fabs ( w * w - ( s * s ) / t );
}

struct LinkInfo
{
    ExtraBonePtrList * list;
    ExtraBonePtr * o1, * o2;
    double d;
    bool used;
    int boneNum;
};

double calcArea ( const ExtraBone & bone, const Vector3d & c, const Vector3d & norm )
{
    const Vector3d & a = bone.v0->point;
    const Vector3d & b = bone.v1->point;
    return ( ( c - a ) % ( b - a ) ) * norm;
}

// Найти наименьшее расстояние между многоугольниками и записать результат в info

void findDist ( const List<ExtraBonePtr> & cplus, LinkInfo & info, const Vector3d & norm )
{
    if ( info.used ) return;
    Show<ExtraBonePtr> plus ( cplus );
    List<ExtraBonePtr> * list = info.list;
    bool isVert = 0;
    plus.top();
    do
    {
        const ExtraBone * b1 = plus.cur()->bone;
        list->top();
        do
        {
            const ExtraBone * b2 = list->cur()->bone;
            if ( b1->v0 == b2->v0 )
            {
                info.d = 0.;
                info.o1 = (ExtraBonePtr *) plus.cur();
                info.o2 = list->cur();
                info.boneNum = 0;
                return;
            }
            {
                const Vector3d & a = b2->v0->point;
                const Vector3d & b = b2->v1->point;
                const Vector3d & p = b1->v0->point;
                const Vector3d u = b - a;
                const Vector3d w = p - a;
                const double s = u * w;
                if ( s >= 0 )
                {
                    const double t = u * u;
                    if ( s <= t )
                    {
                        const double q =  _max ( 0., w * w - ( s * s ) / t );
                        if ( info.d > q )
                        {
                            const double a1 = calcArea ( *b2, p, norm );
                            if ( a1 > 0 )
                            {
                                info.d = q;
                                info.o1 = (ExtraBonePtr *) plus.cur();
                                info.o2 = list->cur();
                                info.boneNum = 2;
                            }
                        }
                    }
                }
            }
            {
                const Vector3d & a = b1->v0->point;
                const Vector3d & b = b1->v1->point;
                const Vector3d & p = b2->v0->point;
                const Vector3d u = b - a;
                const Vector3d w = p - a;
                const double s = u * w;
                if ( s >= 0 )
                {
                    const double t = u * u;
                    if ( s <= t )
                    {
                        const double q = _max ( 0., w * w - ( s * s ) / t );
                        if ( info.d > q )
                        {
                            const double a1 = calcArea ( *b1, p, norm );
                            if ( a1 > 0 )
                            {
                                info.d = q;
                                info.o1 = (ExtraBonePtr *) plus.cur();
                                info.o2 = list->cur();
                                info.boneNum = 1;
                            }
                        }
                    }
                }
                else
                {
                    const double q = w * w;
                    if ( info.d > q )
                    {
                        const ExtraBonePtr * pn = plus.cnext();
                        const Vector3d & c = pn->bone->v0->point;
                        plus.cprev();
                        if ( ( ( c - a ) % ( b - a ) ) * norm < 0 )
                        {
                            const double a1 = calcArea ( *b1, p, norm );
                            const double a2 = calcArea ( *pn->bone, p, norm );
                            double aa = _max ( a1, a2 );
                            if ( aa > 0 )
                            {
                                info.d = q;
                                info.o1 = (ExtraBonePtr *) ( a2 > a1 ? pn : plus.cur() );
                                info.o2 = list->cur();
                                info.boneNum = 1;
                            }
                        }
                    }
                }
            }
        }
        while ( list->next() );
    }
    while ( plus.next() );
}

bool layOnBone ( const ExtraBone & bone, const Vector3d & c )
{
    const Vector3d & a = bone.v0->point;
    const Vector3d & b = bone.v1->point;
    const Vector3d ab = a - b;
    const Vector3d bc = b - c;
    const Vector3d ca = c - a;
    const double dc = qmod ( ab );
    const double da = qmod ( bc );
    const double db = qmod ( ca );
    if ( dc >= db && dc >= da )
    {
         return qmod ( ca % bc ) < 1e-16 * dc;
    }
    return false;
}

void makeClosed ( ExtraBonePtrList & list, List< ListItem<ExtraBonePtrList> > & itog )
{
    ExtraBonePtrList temp;
    typedef SortItem<Set2<const Vert *, int>, ExtraBonePtr *> SItem;
    Suite<SItem> vert ( list.size() );
    if ( list.top() )
    for(;;)
    {
        ExtraBonePtr * t = list.cur();
        if ( t->bone->v0 == t->bone->v1 )
        {
            if ( list.curIsLas() )
            {
                list.delCur();
                break;
            }
            else
            {
                list.delCur();
                continue;
            }
        }
        SItem & si = vert.inc();
        si.head.a = t->bone->v0;
        si.head.b = 0;
        si.tail = t;
        if ( ! list.next() ) break;
    }
    quickSort123 ( vert );

    SItem si0, si1;
    si0.head.b = 0;
    si1.head.b = vert.size();
    while ( list.size() > 0 )
    {
        if ( temp.size() > 0 )
        {
            si0.head.a = temp.las()->bone->v1;
            const nat i = lasEqu123 ( vert, si0 );
            if ( i < vert.size() )
            {
                SItem & si = vert[i];
                si.head.b = si1.head.b;
                list.jump ( si.tail ).movCurAftLas ( temp );
                if ( temp.fir()->bone->v0 == temp.las()->bone->v1 )
                {
                    ListItem<ExtraBonePtrList> * p = new ListItem<ExtraBonePtrList>;
                    itog.addAftLas ( p );
                    temp.movAllAftCur ( *p ); // цепочка закрыта, начинаем сначала
                }
                else
                {
                    si1.head.a = temp.las()->bone->v1;
                    if ( hasEqu123 ( vert, si1 ) )
                    {
                        temp.top();
                        while ( temp.next() )
                        {
                            if ( temp.cur()->bone->v0 == temp.las()->bone->v1 )
                            {
                                ListItem<ExtraBonePtrList> * p = new ListItem<ExtraBonePtrList>;
                                itog.addAftLas ( p );
                                temp.movBefCurBefCur ( *p ); // цепочка разделена
                                swap ( *p, temp );
                            }
                        }
                    }
                }
            }
            else
            {
                temp.delLas(); // удаляем последнее звено, т.к. у него нет продолжения
           }
        }
        else
        {
            si0.head.a = list.las()->bone->v0;
            const nat i = lasEqu123 ( vert, si0 );
            if ( i < vert.size() )
            {
                // начинаем новую цепочку
                SItem & si = vert[i];
                si.head.b = --si1.head.b;
                list.jump ( si.tail ).movCurAftLas ( temp );
            }
            else
            {
                list.delLas(); // такого не должно быть
            }
        }
    }
}

// Собираем из рёбер грани

void makeSides ( ExtraBonePtrList & list, const Plane3d & plane, ExtraSideList & sides, ExtraBoneList & bones, ExtraBodyStor & stor )
{
// Получение многоугольников
    List< ListItem<ExtraBonePtrList> > itog;
    makeClosed ( list, itog );
    if ( itog.size() == 0 ) return;
// Сортировка многоугольников по площади
    DynArray<double> area ( itog.size() );
    List1p plus, minus;
    nat i = 0;
    if ( itog.top() )
    do
    {
        ListItem<ExtraBonePtrList> * lp = itog.cur();
        if ( lp->size() == 2 )
        {
            ExtraBonePtr * f = lp->top();
            ExtraBonePtr * l = lp->las();
            ExtraBone * b = l->bone;
            ExtraBonePtrList * c = f->bone->c1 = b->c0;
            c->jump ( & b->b0 ).swapCurAndCur ( *lp );
            lp->delAll ();
            bones.del ( b );
            continue;
        }
        lp->setAllSelf();
        Vector3d v ( 0., 0., 0. );
        lp->invert();
        lp->top();
        do
        {
            const ExtraBone * b = lp->cur()->bone;
            v -= b->v0->point % b->v1->point;
        }
        while ( lp->next() );
        const double a = area[i] = v * plane.norm;
        Item1p * item = new Item1p ( lp, i++ );
        if ( a < 0 )
        {
            minus.addAftCur ( item );
        }
        else
        {
            plus.addAftCur ( item );
        }
    }
    while ( itog.next() );
//if ( minus.size() ) display << minus.size() << NL;
    Double123Sorter a123 ( area() );
    Double321Sorter a321 ( area() );
    plus .sort ( a123 );
    minus.sort ( a321 );
// Слияние отрицательных многоугольников с положительными
    Suite<Vector2d> buf2;
    Suite<Vector3d> buf3;
    OrthoFunc3to2 trans ( plane.norm );
    if ( plus.top() )
    do
    {
        sides.addNewAftLas ( stor.sides )->plane = plane;
        ExtraSide * s = sides.las();
        s->blist.side = s;
        swap ( *(ListItem<ExtraBonePtrList> *) plus.cur()->a, s->blist );
        s->blist.setAllSelf();
        if ( minus.top() )
        {
            const nat nv = s->blist.size();
            buf3.resize ( nv );
            buf2.resize ( nv );
            nat count = 0;
            s->blist.top();
            do
            {
                buf3[count++] = s->blist.cur()->bone->v0->point;
            }
            while ( s->blist.next() );
            ortho_trans ( buf3, plane.norm, buf2 );
            List< ListItem<ExtraBonePtrList> > temp;
            do
            {
                if ( area[(nat)plus.cur()->info] + area[(nat)minus.cur()->info] < 0 ) break;
                ListItem<ExtraBonePtrList> & mlp = *(ListItem<ExtraBonePtrList> *) minus.cur()->a;
                if ( mlp.top() )
                do
                {
                    const ExtraBone * b = mlp.cur()->bone;
                    const Vector2d p = trans ( 0.5 * ( b->v0->point + b->v1->point ) );
                    if ( isIntersect ( p, buf2 ) )
                    {
                        itog.jump ( & mlp );
                        itog.movCurAftCur ( s->holes );
                        s->holes.cur()->side = s;
                        minus.cur()->a = 0;
                        break;
                    }
                }
                while ( mlp.next() );
            }
            while ( minus.next() );
            minus.top();
            for(;;)
            {
                if ( minus.cur()->a == 0 )
                {
                    if ( minus.curIsLas() )
                    {
                        minus.delCur();
                        break;
                    }
                    else
                    {
                        minus.delCur();
                        continue;
                    }
                }
                if ( ! minus.next() ) break;
            }
        }
        if(0)
        {
            if ( s->blist.top() )
            do
            {
                if ( ! s->blist.cur()->bone->c1 ) display << "s->blist" << NL;
            }
            while ( s->blist.next() );
            if ( s->holes.top() )
            do
            {
                if ( s->holes.cur()->top() )
                do
                {
                    if ( ! s->holes.cur()->cur()->bone->c1 ) display << "s->holes" << NL;
                }
                while ( s->holes.cur()->next() );
            }
            while ( s->holes.next() );
        }
    }
    while ( plus.next() );
// Эта часть никогда не должна выполняться, а сделана на всякий случай
    if ( minus.top() )
    do
    {
display << "no" << NL;
        sides.addNewAftLas ( stor.sides )->plane = -plane;
        ExtraSide * s = sides.las();
        s->blist.side = s;
        swap ( *(ListItem<ExtraBonePtrList> *) minus.cur()->a, s->blist );
        s->blist.setAllSelf();
    }
    while ( minus.next() );
}

} // namespace ///////////////////////////////////////////////////////////////////////

ExtraBody & ExtraBody::cut ( const Plane3d & plane, ExtraBodyStor & stor, int mark )
{
    if ( ! verts.size() || ! bones.size() || ! sides.size() || ! plane.norm ) return *this;
    bones.top();
    do
    {
        const ExtraBone * b = bones.cur();
        if ( ! b->v0 || ! b->v1 || ! b->c0 || ! b->c1 )
        {
if ( ! b->v0 ) display << "! b->v0" << NL;
if ( ! b->v1 ) display << "! b->v1" << NL;
if ( ! b->c0 ) display << "! b->c0" << NL;
if ( ! b->c1 ) display << "! b->c1" << NL;
            return *this;
        }
    }
    while ( bones.next() );

    VertList vtmp;
    double bx = plane.norm.x;
    double by = plane.norm.y;
    double bz = plane.norm.z;
    double bd = plane.dist;
    double ax = fabs ( bx );
    double ay = fabs ( by );
    double az = fabs ( bz );
    nat way = ay > az ? ( ay > ax ? 1 : 0 ) : ( az > ax ? 2 : 0 );
    switch ( way )
    {
        case 0: bx /= ax; by /= ax; bz /= ax; bd /= ax; break;
        case 1: bx /= ay; by /= ay; bz /= ay; bd /= ay; break;
        case 2: bx /= az; by /= az; bz /= az; bd /= az; break;
    }
    verts.top();
    do
    {
        double d = 0; // для оптимизатора 21.01.2014
        Vert & v = * verts.cur ();
        switch ( way )
        {
            case 0: v.d = bx*v.point.x;
                      d = by*v.point.y + bz*v.point.z + bd; break;
            case 1: v.d = by*v.point.y;
                      d = bz*v.point.z + bx*v.point.x + bd; break;
            case 2: v.d = bz*v.point.z;
                      d = bx*v.point.x + by*v.point.y + bd; break;
        }
        v.d += d;
    }
    while ( verts.next () );

    sides.top();
    do
    {
        sides.cur()->blist.flag = 0;
    }
    while ( sides.next() );
    
    bones.top();
    for (;;)
    {
        ExtraBone & b = * bones.cur();
        if ( b.v0->d > 0 )
        {
            b.c0->flag = b.c1->flag = 1;
            if ( b.v1->d < 0 )
            {
                Vector3d point;
                double a = b.v0->d / ( b.v0->d - b.v1->d );
                double c = b.v1->d / ( b.v1->d - b.v0->d );
                switch ( way )
                {
                    case 0: point.y = a * b.v1->point.y + c * b.v0->point.y;
                            point.z = a * b.v1->point.z + c * b.v0->point.z;
                            point.x = by * point.y + bz * point.z + bd;
                            if ( bx > 0 ) point.x = - point.x;
                            break;
                    case 1: point.z = a * b.v1->point.z + c * b.v0->point.z;
                            point.x = a * b.v1->point.x + c * b.v0->point.x;
                            point.y = bz * point.z + bx * point.x + bd;
                            if ( by > 0 ) point.y = - point.y;
                            break;
                    case 2: point.x = a * b.v1->point.x + c * b.v0->point.x;
                            point.y = a * b.v1->point.y + c * b.v0->point.y;
                            point.z = bx * point.x + by * point.y + bd;
                            if ( bz > 0 ) point.z = - point.z;
                            break;
                }
                if ( point != b.v1->point )
                {
                    b.v0 = vtmp.addNewAftLas ( stor.verts );
                    b.v0->point = point;
                    if ( ! bones.next () ) break;
                    continue;
                }
            }
            b.c0->del ( & b.b0 );
            b.c1->del ( & b.b1 );
            if ( bones.curIsLas() )
            {
                bones.movCurAftLas ( stor.bones );
                break;
            }
            bones.movCurAftLas ( stor.bones );
        }
        else
        {
            if ( b.v0->d < 0 )
            {
                if ( b.v1->d > 0 )
                {
                    b.c0->flag = b.c1->flag = 1;
                    Vector3d point;
                    double a = b.v0->d / ( b.v0->d - b.v1->d );
                    double c = b.v1->d / ( b.v1->d - b.v0->d );
                    switch ( way )
                    {
                        case 0: point.y = a * b.v1->point.y + c * b.v0->point.y;
                                point.z = a * b.v1->point.z + c * b.v0->point.z;
                                point.x = by * point.y + bz * point.z + bd;
                                if ( bx > 0 ) point.x = - point.x;
                                break;
                        case 1: point.z = a * b.v1->point.z + c * b.v0->point.z;
                                point.x = a * b.v1->point.x + c * b.v0->point.x;
                                point.y = bz * point.z + bx * point.x + bd;
                                if ( by > 0 ) point.y = - point.y;
                                break;
                        case 2: point.x = a * b.v1->point.x + c * b.v0->point.x;
                                point.y = a * b.v1->point.y + c * b.v0->point.y;
                                point.z = bx * point.x + by * point.y + bd;
                                if ( bz > 0 ) point.z = - point.z;
                                break;
                    }
                    if ( point == b.v0->point )
                    {
                        b.c0->del ( & b.b0 );
                        b.c1->del ( & b.b1 );
                        if ( bones.curIsLas() )
                        {
                            bones.movCurAftLas ( stor.bones );
                            break;
                        }
                        bones.movCurAftLas ( stor.bones );
                        continue;
                    }
                    b.v1 = vtmp.addNewAftLas ( stor.verts );
                    b.v1->point = point;
                }
                if ( ! bones.next () ) break;
            }
            else
            {
                if ( b.v1->d >= 0 )
                {
                    b.c0->flag = b.c1->flag = 1;
                    b.c0->del ( & b.b0 );
                    b.c1->del ( & b.b1 );
                    if ( bones.curIsLas() )
                    {
                        bones.movCurAftLas ( stor.bones );
                        break;
                    }
                    bones.movCurAftLas ( stor.bones );
                }
                else
                    if ( ! bones.next () ) break;
            }
        }
    }
    if ( ! bones.cur() ) return makeVoid ( stor );

    verts.top();
    for (;;)
    {
        if ( verts.cur()->d > 0 )
        {
            if ( verts.curIsLas() ) { verts.movCurAftLas ( stor.verts ); break; }
            verts.movCurAftLas ( stor.verts );
        }
        else
            if ( ! verts.next() ) break;
    }
    vtmp.movAllAftLas ( verts );

    if ( ! sides.top() ) return *this;

    ExtraBonePtrList clist;
    for (;;)
    {
        if ( sides.cur()->blist.flag )
        {
            if ( sides.cur()->blist.size() > 0 )
            {
                recon ( sides, bones, clist, plane, stor );
                if ( ! sides.next() ) break;
            }
            else
            {
                if ( sides.curIsLas() )
                {
                    sides.movCurAftLas ( stor.sides );
                    break;
                }
                else
                {
                    sides.movCurAftLas ( stor.sides );
                }
            }
        }
        else
        {
            if ( ! sides.next() ) break;
        }
    }

    ExtraSideList temp;
    makeSides ( clist, plane, temp, bones, stor );
    /*
    if ( sides.top() )
    do
    {
        ExtraSide * s = sides.cur();
        if ( s->blist.top() )
        do
        {
            if ( ! s->blist.cur()->bone->c1 ) display << "s->blist" << NL;
        }
        while ( s->blist.next() );
        if ( s->holes.top() )
        do
        {
            if ( s->holes.cur()->top() )
            do
            {
                if ( ! s->holes.cur()->cur()->bone->c1 ) display << "s->holes" << NL;
            }
            while ( s->holes.cur()->next() );
        }
        while ( s->holes.next() );
    }
    while ( sides.next() );
    //*/
    temp.setAllInfo ( mark );
    temp.movAllAftLas ( sides );
//    delSmallBones ( *this, 1e-9 );
    return *this;
}
/*
ExtraBody & ExtraBody::operator= ( const ExtraBody & body )
{
    return copy ( body, ExtraBodyStor(), *this );
}
*/
//**************************** 23.04.2012 ****************************
//
//      Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
//
//**************************** 23.04.2012 ****************************

ExtraBody & ExtraBody::makeVoid ()
{
	verts.delAll(); bones.delAll(); sides.delAll(); return *this;
}

ExtraBody & ExtraBody::makeVoid ( ExtraBodyStor & stor )
{
    if ( sides.top() )
    do
    {
        sides.cur()->blist.delAll();
    }
    while ( sides.next() );
    sides.movAllAftLas ( stor.sides );
    bones.movAllAftLas ( stor.bones );
    verts.movAllAftLas ( stor.verts );
    return *this;
}

//**************************** 12.10.2009 ****************************
//
//      Создание параллелепипеда ( x, y, z - половины сторон )
//
//**************************** 12.10.2009 ****************************
/*
ExtraBody & ExtraBody::makeCuboid ( double x, double y, double z )
{
    return copy ( Polyhedron().makeCuboid ( x, y, z ), *this );
}
*/
//**************************** 31.03.2014 ****************************
//
//              Смещение многогранника на вектор v
//
//**************************** 31.03.2014 ****************************

ExtraBody & ExtraBody::operator += ( const Vector3d & v )
{
    if ( verts.top() )
    do
    {
        verts.cur()->point += v;
    }
    while ( verts.next() );
    if ( sides.top() )
    do
    {
        sides.cur()->plane += v;
    }
    while ( sides.next() );
    return *this;
}

ExtraBody & ExtraBody::operator -= ( const Vector3d & v )
{
    if ( verts.top() )
    do
    {
        verts.cur()->point -= v;
    }
    while ( verts.next() );
    if ( sides.top() )
    do
    {
        sides.cur()->plane -= v;
    }
    while ( sides.next() );
    return *this;
}

void swap ( ExtraBody & body1, ExtraBody & body2 )
{
    swap ( body1.sides, body2.sides );
    swap ( body1.bones, body2.bones );
    swap ( body1.verts, body2.verts );
}