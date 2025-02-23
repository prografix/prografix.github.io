
#include "math.h"
#include "../shev/Lists.h"
#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "vert.h"
#include "body2.h"
#include "../shev/trans.h"
#include "../shev/func1t.h"
#include "../shev/func2d.h"
#include "../shev/Polyhedron.h"
#include "../shev/intersect2d.h"

BonePtrList::~BonePtrList()
{
    outAll();
}

double Bone::getDistance ( const Vector3d & p ) const // Расстояние до точки
{
    if ( !v0 || !v1 ) return 1e300;
    return sqrt ( Segment3d ( v0->point, v1->point ).qdis ( p ) );
}

Def<Vector3d> Side::centerOfMass() const
{
    Show<BonePtr> show ( *this );
    if ( show.top() )
    {
        const Bone * b = show.cur()->bone;
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
            const Bone * b = show.cur()->bone;
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

double Side::area() const
{
    if ( size() < 3 ) return 0.;
    Vector3d v ( 0., 0., 0. );
    Show<BonePtr> show ( *this );
    show.top();
    do
    {
        const Bone * b = show.cur()->bone;
        v += & b->b0 == show.cur() ? b->v0->point % b->v1->point : b->v1->point % b->v0->point;
    }
    while ( show.next() );
    return 0.5 * ( v * plane.norm );
}

double Side::perimeter() const // Периметр
{
    if ( ! size() ) return 0.;
    double p = 0.;
    Show<BonePtr> show ( *this );
    show.top();
    do
    {
        const Bone * b = show.cur()->bone;
        p += norm2 ( b->v1->point - b->v0->point );
    }
    while ( show.next() );
    return p;
}

double Side::getDistance ( const Vector3d & p ) const  // Расстояние до точки
{
    if ( ! size() ) return 1e300;
    DynArray<Vector2d> poly;
//    copy ( *this, poly );
    if ( isIntersect ( poly, trans ( plane.project ( p ), plane.norm ) ) ) return fabs ( plane % p );
    Show<BonePtr> show ( *this );
    double min = show.top()->bone->getDistance ( p );
    while ( show.next() )
    {
        const double t = show.cur()->bone->getDistance ( p );
        if ( min > t ) min = t;
    }
    return min;
}

void Side::initPlane()
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

double Body::volume() const // Объём
{
    double v = 0.;
    Show<Side> show ( sides );
    if ( show.top() )
    do
    {
        const Side * side = show.cur ();
        v -= side->area() * side->plane.dist;
    }
    while ( show.next() );
    return v / 3.;
}

double Body::getArea() const // Площадь поверхности
{
    double a = 0.;
    Show<Side> show ( sides );
    if ( show.top() )
    do
    {
        a += show.cur()->area();
    }
    while ( show.next() );
    return a;
}

Def<Vector3d> Body::centerOfMass() const // Центр масс
{
    double m = 0.;
    Vector3d c ( 0, 0, 0 );
    Show<Side> sides ( sides );
    if ( sides.top() )
    do
    {
        Show<BonePtr> side ( * sides.cur() );
        if ( !side.top() ) continue;
        const Bone * b = side.cur()->bone;
        const Vector3d v1 = & b->b0 == side.cur() ? b->v0->point : b->v1->point;
        while ( side.next() )
        {
            if ( side.curIsLas() ) break;
            const Bone * b = side.cur()->bone;
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

namespace ////////////////////////////////////////////////////////////////////////////
{

void divideSide ( Side * s, BoneList & link, const Vector3d & v, SideList & sides, BoneList & bones )
{
    while ( link.size() > 0 )
    {
        if ( link.size() == 1 )
        {
            link.movAllAftLas ( bones );
            break;
        }
        Bone * a = link.top();
        double da = v * a->v0->point;
        while ( link.next() )
        {
            Bone * p = link.cur();
            double d = v * p->v0->point;
            if ( da > d ) a = p, da = d;
        }
        link.makeFir ( a );
        Bone * b = link.top();
        double db = v * b->v1->point;
        while ( link.next() )
        {
            Bone * p = link.cur();
            double d = v * p->v1->point;
            if ( db > d ) b = p, db = d;
        }
        if ( a == b )
        {
            link.movFirAftLas ( bones );
            continue;
        }
        _swap ( a->v1, b->v1 );
        s->makeLas ( & b->b0 );
        link.makeLas ( b );
        s->jump ( & a->b0 );
        link.jump ( a );
        Side * t = new Side ( s->plane );
        t->info = s->info;
        sides.addBefFir ( t );
        s->movAftCurAftCur ( *t );
        List<Bone> temp;
        link.movAftCurAftCur ( temp );
        if ( s->size() < t->size() )
        {// Для скорости поменяем грани местами
            swap ( *s, *t );
            swap ( link, temp );
        }
        t->setAllSelf();
        divideSide ( t, temp, v, sides, bones );
    }
}

void recon ( SideList & sides, BoneList & bones, BonePtrList & side, const Plane3d & plane, List<Bone> & bstor )
{
    // Закрываем пробелы рёбрами
    List<Bone> link;
    Side * s = sides.cur();
    Vert * v0 = s->las()->vert1();
    s->top();
    do
    {
        Vert * v1 = s->vert0();
        if ( v0 != v1 )
        {
            Bone * b = link.addNewAftLas ( bstor );
            b->v0 = v0;
            b->v1 = v1;
            b->s0 = s;
            b->s1 = 0;
            s->addBefCur ( & b->b0 ).next();
            side.addAftLas ( & b->b1 );
        }
        v0 = s->vert1();
    }
    while ( s->next() );
    // Если нужно - делим грань на части
    const Vector3d v = s->plane.norm % plane.norm;
    divideSide ( s, link, v, sides, bones );
}

double qdis ( const Bone & bone, const Vector3d & p, bool & isVert )
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
    BonePtrList * list;
    BonePtr * o1, * o2;
    double d;
    bool used;
    int boneNum;
};

double calcArea ( const Bone & bone, const Vector3d & c, const Vector3d & norm )
{
    const Vector3d & a = bone.v0->point;
    const Vector3d & b = bone.v1->point;
    return ( ( c - a ) % ( b - a ) ) * norm;
}

// Найти наименьшее расстояние между многоугольниками и записать результат в info

void findDist ( const BonePtrList & cplus, LinkInfo & info, const Vector3d & norm )
{
    if ( info.used ) return;
    Show<BonePtr> plus ( cplus );
    List<BonePtr> * list = info.list;
    bool isVert = 0;
    plus.top();
    do
    {
        const Bone * b1 = plus.cur()->bone;
        list->top();
        do
        {
            const Bone * b2 = list->cur()->bone;
            if ( b1->v0 == b2->v0 )
            {
                info.d = 0.;
                info.o1 = (BonePtr *) plus.cur();
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
                                info.o1 = (BonePtr *) plus.cur();
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
                                info.o1 = (BonePtr *) plus.cur();
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
                        const BonePtr * pn = plus.cnext();
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
                                info.o1 = (BonePtr *) ( a2 > a1 ? pn : plus.cur() );
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

bool layOnBone ( const Bone & bone, const Vector3d & c )
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

void link ( BonePtrList & plus, List< ListItem<BonePtrList> > & minus, const Plane3d & plane, BoneList & bones, SideList & sides )
{
    if ( plus.size() == 0 || minus.size() == 0 ) return;
// Находим наименьшие расстояния между положительным и отрицательными многоугольниками
    const nat n = minus.size();
    LinkInfo * info = new LinkInfo[n];
    nat i = 0;
    minus.top();
    do
    {
        BonePtrList * list = minus.cur();
        LinkInfo & cinfo = info[i++];
        cinfo.d = 1e300;
        cinfo.list = list;
        cinfo.used = false;
    }
    while ( minus.next() );
#ifdef PARALLEL
    Concurrency::parallel_for(0u, n, [&](nat i)
    {
        findDist ( plus, info[i], plane.norm ); 
    });
#else
    for ( i = 0; i < n; ++i )
    {
        findDist ( plus, info[i], plane.norm );
    }
#endif
// Связываем отрицательные многоугольники с положительным
    for(;;)
    {
        nat imin = n;
        for ( i = 0; i < n; ++i )
        {
            const LinkInfo & cinfo = info[i];
            if ( cinfo.used ) continue;
            if ( imin == n || info[imin].d > cinfo.d ) imin = i;
        }
        if ( imin == n ) break;
        LinkInfo & cinfo = info[imin];
        cinfo.used = true;
        BonePtrList * list = cinfo.list;
        list->jump ( cinfo.o2 ).makeCurLas();
        plus.jump ( cinfo.o1 );
        if ( cinfo.boneNum == 1 )
        {
            if ( layOnBone ( *cinfo.o1->bone, cinfo.o2->bone->v0->point ) )
            {
                Bone * b1 = cinfo.o1->bone;
                Bone * b2 = new Bone ( cinfo.o2->bone->v0, b1->v1, b1->s0, 0 );
                b1->v1 = b2->v0;
                bones.addAftLas ( b2 );
                b1->s0->jump ( & b1->b0 ).addAftCur ( & b2->b0 );
                list->addBefFir ( & b2->b1 );
                plus.cprev();
            }
            else
            {
                Side * side = new Side ( plane );
                sides.addAftLas ( side );
                Bone * b1 = new Bone ( cinfo.o1->bone->v0, cinfo.o2->bone->v0, side, 0 );
                bones.addAftLas ( b1 );
                side->addAftLas ( & b1->b0 );
                list->addAftLas ( & b1->b1 );
                Bone * b2 = new Bone ( cinfo.o2->bone->v0, cinfo.o1->bone->v1, side, 0 );
                bones.addAftLas ( b2 );
                side->addAftLas ( & b2->b0 );
                list->addBefFir ( & b2->b1 );
                plus.makeCurLas();
                plus.movCurAftLas ( *side );
                cinfo.o1->bone->s1 = side;
                for ( i = 0; i < n; ++i )
                {
                    if ( info[i].o1 == cinfo.o1 ) info[i].d = 1e300;
                }
            }
        }
        else
        if ( cinfo.boneNum == 2 )
        {
            Side * side = new Side ( plane );
            sides.addAftLas ( side );
            list->movCurAftCur ( *side );
            cinfo.o2->bone->s1 = side;
            Bone * b1 = new Bone ( cinfo.o2->bone->v0, cinfo.o1->bone->v0, side, 0 );
            bones.addAftLas ( b1 );
            side->addAftLas ( & b1->b0 );
            list->addBefFir ( & b1->b1 );
            Bone * b2 = new Bone ( cinfo.o1->bone->v0, cinfo.o2->bone->v1, side, 0 );
            bones.addAftLas ( b2 );
            side->addAftLas ( & b2->b0 );
            list->addAftLas ( & b2->b1 );
        }
// Считаем наименьшие расстояния для новых рёбер
#ifdef PARALLEL
        Concurrency::parallel_for(0u, n, [&](nat i)
        {
            findDist ( *list, info[i], plane.norm ); 
        });
#else
        for ( i = 0; i < n; ++i )
        {
            findDist ( *list, info[i], plane.norm );
        }
#endif
        list->movAllAftCur ( plus );
    }
    delete[] info;
    minus.delAll();
}

void makeClosed ( BonePtrList & list, List< ListItem<BonePtrList> > & itog )
{
    BonePtrList temp;
    typedef SortItem<Set2<const Vert *, int>, BonePtr *> SItem;
    Suite<SItem> vert ( list.size() );
    if ( list.top() )
    for(;;)
    {
        BonePtr * t = list.cur();
        if ( t->bone->v0 == t->bone->v1 )
        {
            if ( list.delCur_() ) break;
            continue;
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
                    ListItem<BonePtrList> * p = new ListItem<BonePtrList>;
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
                                ListItem<BonePtrList> * p = new ListItem<BonePtrList>;
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

void makeSides ( BonePtrList & list, const Plane3d & plane, SideList & sides, BoneList & bones, BodyStor & stor )
{
// Получение многоугольников
    List< ListItem<BonePtrList> > itog;
    makeClosed ( list, itog );
    if ( itog.size() == 0 ) return;
// Сортировка многоугольников по площади
    DynArray<double> area ( itog.size() );
    List1p plus, minus;
    nat i = 0;
    if ( itog.top() )
    do
    {
        ListItem<BonePtrList> * lp = itog.cur();
        if ( lp->size() == 0 ) continue;
        if ( lp->size() == 2 )
        {
            BonePtr * f = lp->top();
            BonePtr * l = lp->las();
            Bone * b = l->bone;
            Side * s = f->bone->s1 = b->s0;
            s->jump ( & b->b0 ).swapCurAndCur ( *lp );
            lp->delAll ();
            bones.del ( b );
            continue;
        }
        Vector3d v ( 0., 0., 0. );
        lp->invert();
        lp->top();
        do
        {
            const Bone * b = lp->cur()->bone;
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
    Double123Sorter a123 ( &area[0] );
    Double321Sorter a321 ( &area[0] );
    plus .sort ( a123 );
    minus.sort ( a321 );
// Слияние отрицательных многоугольников с положительными
    Suite<Vector2d> buf2;
    Suite<Vector3d> buf3;
    OrthoFunc3to2 trans ( plane.norm );
    if ( plus.top() )
    do
    {
        BonePtrList & lp = *(ListItem<BonePtrList> *) plus.cur()->a;
        if ( minus.top() )
        {
            const nat nv = lp.size();
            buf3.resize ( nv );
            buf2.resize ( nv );
            nat count = 0;
            lp.top();
            do
            {
                buf3[count++] = lp.cur()->bone->v0->point;
            }
            while ( lp.next() );
            ortho_trans ( buf3, plane.norm, buf2 );
            List< ListItem<BonePtrList> > temp;
            do
            {
                if ( area[plus.cur()->info] + area[minus.cur()->info] < 0 ) break;
                ListItem<BonePtrList> & mlp = *(ListItem<BonePtrList> *) minus.cur()->a;
                if ( mlp.top() )
                do
                {
                    const Bone * b = mlp.cur()->bone;
                    const Vector2d p = trans ( 0.5 * ( b->v0->point + b->v1->point ) );
                    if ( isIntersect ( p, buf2 ) )
                    {
                        itog.jump ( & mlp );
                        itog.movCurAftCur ( temp );
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
                    if ( minus.delCur_() ) break;
                    continue;
                }
                if ( ! minus.next() ) break;
            }
            link ( lp, temp, plane, bones, sides );
        }
        sides.addNewAftLas ( stor.sides )->plane = plane;
        Side * s = sides.las();
        swap ( lp, *s );
        s->top();
        do
        {
            s->cur()->bone->s1 = s;
        }
        while ( s->next() );
    }
    while ( plus.next() );
// Эта часть никогда не должна выполняться, а сделана на всякий случай
    if ( minus.top() )
    do
    {
        BonePtrList & lp = *(ListItem<BonePtrList> *) minus.cur()->a;
        if ( lp.size() == 0 ) continue;
        Side * s = new Side ( plane );
        swap ( lp, *s );
        s->top();
        do
        {
            s->cur()->bone->s1 = s;
        }
        while ( s->next() );
        sides.addAftCur ( s );
    }
    while ( minus.next() );
}

} // namespace ///////////////////////////////////////////////////////////////////////

#include "PolyhedronDoctor.h"

Body & Body::cut ( const Plane3d & plane, BodyStor & stor, int mark )
{
    if ( ! plane.norm ) return *this;
    if ( ! verts.size() || ! bones.size() || ! sides.size() ) return makeVoid ( stor );

    // Проверка на корректность 13.11.2017
    bool isErr = false;
    bones.top();
    do
    {
        const Bone * b = bones.cur();
        if ( ! b->v0 || ! b->v1 || ! b->s0 || ! b->s1 )
        {
            isErr = true;
            break;
        }
    }
    while ( bones.next() );
    if ( isErr )
    {
        doctorPolyhedron ( *this, *this, PolyhedronDoctorReportDummy() );
        if ( ! verts.size() || ! bones.size() || ! sides.size() ) return makeVoid ( stor );
    }

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
        sides.cur()->flag = 0;
    }
    while ( sides.next() );

    bones.top();
    for (;;)
    {
        Bone & b = * bones.cur();
        if ( b.v0->d > 0 )
        {
            b.s0->flag = b.s1->flag = 1;
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
            b.s0->del ( & b.b0 );
            b.s1->del ( & b.b1 );
            if ( bones.movCurAftLas_ ( stor.bones ) ) break;
        }
        else
        {
            if ( b.v0->d < 0 )
            {
                if ( b.v1->d > 0 )
                {
                    b.s0->flag = b.s1->flag = 1;
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
                        b.s0->del ( & b.b0 );
                        b.s1->del ( & b.b1 );
                        if ( bones.movCurAftLas_ ( stor.bones ) ) break;
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
                    b.s0->flag = b.s1->flag = 1;
                    b.s0->del ( & b.b0 );
                    b.s1->del ( & b.b1 );
                    if ( bones.movCurAftLas_ ( stor.bones ) ) break;
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
            if ( verts.movCurAftLas_ ( stor.verts ) ) break;
        }
        else
            if ( ! verts.next() ) break;
    }
    vtmp.movAllAftLas ( verts );

    BonePtrList side;
    for ( sides.top();; )
    {
        if ( sides.cur()->size() > 0 )
        {
            if ( sides.cur()->flag )
                recon ( sides, bones, side, plane, stor.bones );
            if ( ! sides.next() ) break;
        }
        else
        {
            if ( sides.movCurAftLas_ ( stor.sides ) ) break;
        }
    }

    SideList temp;
    makeSides ( side, plane, temp, bones, stor );
    temp.setAllInfo ( mark );
    temp.movAllAftLas ( sides );
    delSmallBones ( *this, 1e-9 );
    return *this;
}

Body & Body::operator= ( const Body & body )
{
    return copy ( body, BodyStor(), *this );
}

//**************************** 23.04.2012 ****************************
//
//      Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
//
//**************************** 23.04.2012 ****************************

Body & Body::makeVoid ()
{
	verts.delAll(); bones.delAll(); sides.delAll(); return *this;
}

Body & Body::makeVoid ( BodyStor & stor )
{
    if ( sides.top() )
    do
    {
        sides.cur()->delAll();
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

Body & Body::makeParallelepiped ( double x, double y, double z )
{
    return copy ( Polyhedron().makeCuboid ( x, y, z ), *this );
}

//**************************** 31.03.2014 ****************************
//
//              Смещение многогранника на вектор v
//
//**************************** 31.03.2014 ****************************

Body & Body::operator += ( const Vector3d & v )
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

Body & Body::operator -= ( const Vector3d & v )
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

bool split ( const Body & body, const Plane3d & plane, Body & bm, Body & bp, BodyStor & stor, int mark )
{
    if ( ! plane.norm ) return false;
    if ( ! body.verts.size() || ! body.bones.size() || ! body.sides.size() ) return false;

    bool isErr = false;
    Show<Bone> bones ( body.bones );
    bones.top();
    do
    {
        const Bone * b = bones.cur();
        if ( ! b->v0 || ! b->v1 || ! b->s0 || ! b->s1 )
        {
            isErr = true;
            break;
        }
    }
    while ( bones.next() );
    if ( isErr ) return false;

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

    VertList vnull;
    DynArray<SortItem<const Vert *, Vert *>> dist ( body.verts.size() );
    Show<Vert> verts ( body.verts );
    verts.top();
    for ( nat i = 0; i < dist.size(); ++i )
    {
        SortItem<const Vert *, Vert *> & si = dist[i];
        si.head = verts.cur();
        const Vector3d & v = si.head->point; 
        double d = 0; // для оптимизатора
        switch ( way )
        {
            case 0: d = bx * v.x + by * v.y + bz * v.z + bd; break;
            case 1: d = by * v.y + bz * v.z + bx * v.x + bd; break;
            case 2: d = bz * v.z + bx * v.x + by * v.y + bd; break;
        }

        if ( d > 0 )
        {
            si.tail = bp.verts.addNewAftLas ( stor.verts );
        }
        else
        if ( d < 0 )
        {
            si.tail = bm.verts.addNewAftLas ( stor.verts );
        }
        else
        {
            si.tail = vnull.addNewAftLas ( stor.verts );
        }
        si.tail->point = v;
        si.tail->info = si.head->info;
        si.tail->d = d;

        verts.next();
    }
    if ( ! bp.verts.size() )
    {
        copy ( body, stor, bm );
        return true;
    }
    if ( ! bm.verts.size() )
    {
        copy ( body, stor, bp );
        return true;
    }
    quickSort123 ( dist );
    
    VertList vtmp;
    SortItem<const Vert *, Vert *> si;
    bones.top();
    do
    {
        const Bone & b = * bones.cur();
        si.head = b.v0;
        nat i = lasEqu123 ( dist, si );
        if ( i == dist.size() )
            return false; // такого не должно быть
        Vert * v0 = dist[i].tail;
        si.head = b.v1;
        i = lasEqu123 ( dist, si );
        if ( i == dist.size() )
            return false; // такого не должно быть
        Vert * v1 = dist[i].tail;
        if ( ! v0->d )
        {
            if ( ! v1->d )
                continue;
            Bone * p;
            if ( v1->d > 0 )
            {
                p = bp.bones.addNewAftLas ( stor.bones );
                p->v0 = bp.verts.addNewAftLas ( stor.verts );
            }
            else
            {
                p = bm.bones.addNewAftLas ( stor.bones );
                p->v0 = bm.verts.addNewAftLas ( stor.verts );
            }
            p->v0->point = b.v0->point;
            p->v0->info = b.v0->info;
            p->v1 = v1;
            p->info = b.info;
            continue;
        }
        if ( v0->d > 0 )
        {
//               b.s0->flag = b.s1->flag = 1;
            if ( v1->d < 0 )
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
                    // b.v0 = vtmp.addNewAftLas ( stor.verts );
                    // b.v0->point = point;
                }
            }
            // b.s0->del ( & b.b0 );
            // b.s1->del ( & b.b1 );
        }
        else
        {/*
            if ( b.v0->d < 0 )
            {
                if ( b.v1->d > 0 )
                {
                    b.s0->flag = b.s1->flag = 1;
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
                        b.s0->del ( & b.b0 );
                        b.s1->del ( & b.b1 );
                        continue;
                    }
                    b.v1 = vtmp.addNewAftLas ( stor.verts );
                    b.v1->point = point;
                }
            }
            else
            {
                if ( b.v1->d >= 0 )
                {
                    b.s0->flag = b.s1->flag = 1;
                    b.s0->del ( & b.b0 );
                    b.s1->del ( & b.b1 );
                }
            }*/
        }
    }
    while ( bones.next() );

    return false;
}
