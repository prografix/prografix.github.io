
#include "lists.h"
#include "func1t.h"
#include "func3d.h"
#include "Poly3gon.h"
#include "Vector3d.h"
#include "../test/Display.h"

class ITrian3d
{
public:
virtual void operator() ( CArrRef<Vector3d> vert, ArrRef<double> a, ArrRef<nat> b ) const = 0;
};

double trianNearPoint ( CArrRef<Vector3d> vert, const ITrian3d & trian, DynArrRef< Set3<nat> > & res )
{
    const nat n = vert.size();
    if ( n < 3 )
    {
        res.resize();
        return 0;
    }

// Заполнение массивов a и b

    const nat n2 = (n-1) * (n-2) / 2;
    CmbArray<double, 820> a ( n2 );
    CmbArray<nat, 820+42> b ( n2 + n );
    trian ( vert, a, b );

// Заполнение массива res

    res.resize ( n - 2 );
    nat k = 0, count = 0;
    ArrRef<nat> c ( b, n2, n );
    c[count++] = 0;
    c[count++] = n - 1;
    while ( count > 0 )
    {
        const nat j2 = c[--count];
        const nat j1 = c[--count];
        const nat j = b[(j2-1)*(j2-2)/2+j1];
        res[k++] = Set3<nat> ( j1, j, j2 );
        if ( j2 - j > 1 )
        {
            c[count++] = j;
            c[count++] = j2;
        }
        if ( j - j1 > 1 )
        {
            c[count++] = j1;
            c[count++] = j;
        }
    }
    return a[(n-2)*(n-3)/2];
}

inline double getValue ( const Triangle3d & t, const Vector3d & norm, const Vector3d & p )
{
    if ( norm * ( ( t.a - t.b ) % ( t.b - t.c ) ) < 0 ) return -1;
    return getDistance2 ( t, p );
}

class Trian3dNearPoint : public ITrian3d
{
    Vector3d point;
public:
    Trian3dNearPoint ( const Vector3d & p ) : point ( p ) {}
    void operator() ( CArrRef<Vector3d> vert, ArrRef<double> a, ArrRef<nat> b ) const
    {
        Vector3d norm ( 0, 0, 0 );
        const nat n = vert.size();
        for ( nat i = 0; i < n; ++i ) norm += vert[i] % vert.cnext(i);
        Triangle3d t;
        for ( nat k = 2; k < n; ++k )
        {
            for ( int i1 = n - k; --i1 >= 0; )
            {
                const nat i2 = i1 + k;
                nat im = i1 + 1;
                t.a = vert[i1];
                t.b = vert[im];
                t.c = vert[i2];
                double d = getValue ( t, norm, point );
                const nat j = (i2-1) * (i2-2) / 2 + i1;
                if ( k == 2 )
                {
                    b[j] = im;
                    a[j] = d;
                    continue;
                }
                double dist = _min ( d, a[j+1] );
                const nat ik = i2 - 1;
                for ( nat i = i1+2; i < ik; ++i )
                {
                    t.b = vert[i];
                    d = _min ( a[(i-1)*(i-2)/2+i1], getValue ( t, norm, point ), a[j+i-i1] );
                    if ( dist < 0 || ( d >= 0 && dist > d ) ) dist = d, im = i;
                }
                t.b = vert[ik];
                d = _min ( a[(ik-1)*(ik-2)/2+i1], getValue ( t, norm, point ) );
                if ( dist < 0 || ( d >= 0 && dist > d ) ) dist = d, im = ik;
                b[j] = im;
                a[j] = dist;
            }
        }
    }
};

class Trian3dNearTriangles : public ITrian3d
{
    CArrRef<Triangle3d> triangle;
public:
    Trian3dNearTriangles ( CArrRef<Triangle3d> t ) : triangle ( t ) {}

    double getValue ( const Triangle3d & t, const Vector3d & norm ) const
    {
        if ( norm * ( ( t.a - t.b ) % ( t.b - t.c ) ) < 0 ) return -1;
        double min = 1e9;
        const Vector3d p = ( t.a + t.b + t.c ) / 3;
        for ( nat i = 0; i < triangle.size(); ++i )
        {
            double t = getDistance2 ( triangle[i], p );
            if ( min > t ) min = t;
        }
        return min;
    }

    static inline double select ( double d1, double d2 )
    {
        //return _max ( d1, d2 );
        return d1 < 0 || d2 < 0 ? -1 : _max ( d1, d2 );
    }

    static inline double select ( double d1, double d2, double d3 )
    {
        //return _max ( d1, d2, d3 );
        return d1 < 0 || d2 < 0 || d3 < 0 ? -1 : _max ( d1, d2, d3 );
    }

    void operator() ( CArrRef<Vector3d> vert, ArrRef<double> a, ArrRef<nat> b ) const
    {
        Vector3d norm ( 0, 0, 0 );
        const nat n = vert.size();
        for ( nat i = 0; i < n; ++i ) norm += vert[i] % vert.cnext(i);
        Triangle3d t;
        for ( nat k = 2; k < n; ++k )
        {
            for ( int i1 = n - k; --i1 >= 0; )
            {
                const nat i2 = i1 + k;
                nat im = i1 + 1;
                t.a = vert[i1];
                t.b = vert[im];
                t.c = vert[i2];
                double d = getValue ( t, norm );
                const nat j = (i2-1) * (i2-2) / 2 + i1;
                if ( k == 2 )
                {
                    b[j] = im;
                    a[j] = d;
                    continue;
                }
                double dist = select ( d, a[j+1] );
                const nat ik = i2 - 1;
                for ( nat i = i1+2; i < ik; ++i )
                {
                    t.b = vert[i];
                    d = select ( a[(i-1)*(i-2)/2+i1], getValue ( t, norm ), a[j+i-i1] );
                    if ( dist < 0 || ( d >= 0 && dist > d ) ) dist = d, im = i;
                    //if ( dist > d ) dist = d, im = i;
                }
                t.b = vert[ik];
                d = select ( a[(ik-1)*(ik-2)/2+i1], getValue ( t, norm ) );
                if ( dist < 0 || ( d >= 0 && dist > d ) ) dist = d, im = ik;
                //if ( dist > d ) dist = d, im = ik;
                b[j] = im;
                a[j] = dist;
            }
        }
    }
};

void delIndex ( List1n & list, nat a )
{
    if ( list.top() )
    do
    {
        if ( list.cur()->a == a )
        {
            list.delCur();
            return;
        }
    }
    while ( list.next() );
}

void delSides ( Poly3gon<Vector3d, void> & poly3gon )
{
    nat i, j, n = 0;
    for ( i = 0; i < poly3gon.side.size(); ++i )
    {
        const Trigon<void> & t = poly3gon.side[i];
        if ( t.a == t.b ) ++n;
    }
    if ( ! n ) return;
    DynArray<Trigon<void> > array ( poly3gon.side.size() - n );
    for ( i = j = 0; i < poly3gon.side.size(); ++i )
    {
        const Trigon<void> & t = poly3gon.side[i];
        if ( t.a == t.b ) continue;
        array[j] = t;
        ++j;
    }
    poly3gon.side.swap ( array );
}

void delVerts ( Poly3gon<Vector3d, void> & poly3gon )
{
    nat i, n = 0;
    DynArray<nat> index ( poly3gon.vertex.size() );
    index.fill ( 0 );
    for ( i = 0; i < poly3gon.side.size(); ++i )
    {
        const Trigon<void> & t = poly3gon.side[i];
        index[t.a] = index[t.b] = index[t.c] = 1;
    }
    for ( i = 0; i < index.size(); ++i )
        if ( index[i] ) ++n;
    DynArray<Vector3d> array ( n );
    for ( nat j = i = 0; i < index.size(); ++i )
    {
        if ( ! index[i] ) continue;
        index[i] = j;
        array[j] = poly3gon.vertex[i];
        ++j;
    }
    for ( i = 0; i < poly3gon.side.size(); ++i )
    {
        Trigon<void> & t = poly3gon.side[i];
        t.a = index[t.a];
        t.b = index[t.b];
        t.c = index[t.c];
    }
    poly3gon.vertex.swap ( array );
}

void arrangeVertex ( Poly3gon<Vector3d, void> & poly3gon )
{
    DynArray<SortItem<Set3<double>, nat> > vertex ( poly3gon.vertex.size() );
    nat i;
    for ( i = 0; i < vertex.size(); ++i )
    {
        const Vector3d & v = poly3gon.vertex[i];
        SortItem<Set3<double>, nat> & si = vertex[i];
        si.head.a = v.x;
        si.head.b = v.y;
        si.head.c = v.z;
        si.tail = i;
    }
    quickSort123 ( vertex );
    DynArray<nat> index ( vertex.size() );
    for ( i = 0; i < vertex.size(); ++i )
    {
        Vector3d & v = poly3gon.vertex[i];
        const SortItem<Set3<double>, nat> & si = vertex[i];
        v.x = si.head.a;
        v.y = si.head.b;
        v.z = si.head.c;
        index[si.tail] = i;
    }
    for ( i = 0; i < poly3gon.side.size(); ++i )
    {
        Trigon<void> & t = poly3gon.side[i];
        t.a = index[t.a];
        t.b = index[t.b];
        t.c = index[t.c];
    }
}

void testVert ( nat i, ArrRef<List1n> vert, const Poly3gon<Vector3d, void> & poly3gon )
{
    List1n & side = vert[i];
    if ( ! side.top() ) return;
    List2n list, stor;
    for ( nat j = 0; j < side.size(); ++j )
    {
        const Trigon<void> & t = poly3gon.side[side.cur()->a];
        Item2n * item = list.addNewAftLas ( stor );
        if ( t.a == i )
        {
            item->a = t.b;
            item->b = t.c;
        }
        else
        if ( t.b == i )
        {
            item->a = t.c;
            item->b = t.a;
        }
        else
        {
            item->a = t.a;
            item->b = t.b;
        }
        side.next();
    }
    List<ListItem<List2n> > itog;
    nat n = list.size();
    makeClosed ( list, itog );
    if ( itog.size() != 1 ) display << "itog.size()" << itog.size() << NL;
    else
    if ( itog.cur()->size() != n ) display << "n" << n << itog.cur()->size() << NL;
}

void simplify ( Poly3gon<Vector3d, void> & poly3gon )
{
    nat i, j;
    List2n list, stor;
    arrangeVertex ( poly3gon );
    DynArray<List1n> vert ( poly3gon.vertex.size() );
    for ( i = 0; i < poly3gon.side.size(); ++i )
    {
        const Trigon<void> & t = poly3gon.side[i];
        vert[t.a].addAftLas ( new Item1n ( i ) );
        vert[t.b].addAftLas ( new Item1n ( i ) );
        vert[t.c].addAftLas ( new Item1n ( i ) );
    }/*
    for ( i = 0; i < vert.size(); ++i )
    {
        List1n & side = vert[i];
        if ( side.top() )
        {
            const Trigon<void> & t = poly3gon.side[side.cur()->a];
            put ( Set2<nat> ( t.a, t.b ), list, stor );
            put ( Set2<nat> ( t.b, t.c ), list, stor );
            put ( Set2<nat> ( t.c, t.a ), list, stor );
        }
        while ( side.next() );
        if ( list.size() < 3 ) display << "list.size()" << list.size() << NL;
        list.movAllAftLas ( stor );
    }*
    for ( i = 0; i < vert.size(); ++i )
    {
        testVert ( i, vert, poly3gon );
    }return;*/
    Suite<nat> index;
    Suite<Vector3d> temp;
    Suite< Set3<nat> > trian;
    Suite<Triangle3d> triangle;
    for ( i = 0; i < vert.size(); ++i )
    {
        List1n side;
        swap ( side, vert[i] );
//display << side.size() << NL;
        if ( ! side.top() ) continue;
        triangle.resize ( side.size() );
        for ( j = 0; j < side.size(); ++j )
        {
            const Trigon<void> & t = poly3gon.side[side.cur()->a];
            Triangle3d & tr = triangle[j];
            tr.a = poly3gon.vertex[t.a];
            tr.b = poly3gon.vertex[t.b];
            tr.c = poly3gon.vertex[t.c];
            Item2n * item = list.addNewAftLas ( stor );
            if ( t.a == i )
            {
                item->a = t.b;
                item->b = t.c;
            }
            else
            if ( t.b == i )
            {
                item->a = t.c;
                item->b = t.a;
            }
            else
            {
                item->a = t.a;
                item->b = t.b;
            }
            side.next();
        }
        List<ListItem<List2n> > itog;
        makeClosed ( list, itog );
        if ( itog.size() != 1 )
        {/*
            if ( side.size() == 2 ) continue;
display << side.size() << NL;
            side.top();
            for ( j = 0; j < side.size(); ++j )
            {
                const Trigon<void> & t = poly3gon.side[side.cur()->a];
                Item2n * item = list.addNewAftLas ( stor );
                if ( t.a == i )
                {
                    item->a = t.b;
                    item->b = t.c;
                }
                else
                if ( t.b == i )
                {
                    item->a = t.c;
                    item->b = t.a;
                }
                else
                {
                    item->a = t.a;
                    item->b = t.b;
                }
display << item->a << item->b << NL;
                side.next();
            }
            list.movAllAftLas ( stor );*/
            if ( itog.top() )
            do
                itog.cur()->movAllBefFir ( stor );
            while ( itog.next() );
            continue;
        }
        List2n & list2 = *itog.cur();
//display << itog.cur()->size() << NL;
        temp.resize ( list2.size() );
        index.resize ( list2.size() );
        list2.top();
        for ( j = 0; j < temp.size(); ++j )
        {
            temp[j] = poly3gon.vertex[list2.cur()->a];
            index[j] = list2.cur()->a;
            list2.next();
        }
        list2.movAllBefFir ( stor );
        double d2 = trianNearPoint ( temp, Trian3dNearTriangles ( triangle ), trian );
        if ( d2 > 1e-12 )
        {
m1:
            swap ( side, vert[i] );
        }
        else
        {
//drawPoint ( poly3gon.vertex[i], 0, 1, 1 );
            double sum1 = 0;
            side.top();
            do
            {
                const Trigon<void> & t = poly3gon.side[side.cur()->a];
                const Triangle3d tr ( poly3gon.vertex[t.a], poly3gon.vertex[t.b], poly3gon.vertex[t.c] );
                sum1 += tr.getArea();
            }
            while ( side.next() );
            double sum2 = 0;
            for ( j = 0; j < trian.size(); ++j )
            {
                const Set3<nat> & t = trian[j];
                const Triangle3d tr ( temp[t.a], temp[t.b], temp[t.c] );
                sum2 += tr.getArea();
            }
            if ( 1.00001 * sum1 < sum2 ) 
            goto m1;
            // Убираем индексы граней из вершин
            side.top();
            do
            {
                const nat si = side.cur()->a;
                const Trigon<void> & t = poly3gon.side[si];
                delIndex ( vert[t.a], si );
                delIndex ( vert[t.b], si );
                delIndex ( vert[t.c], si );
            }
            while ( side.next() );
            // Формируем новые грани
//if ( temp.size() - trian.size() != 2 ) display << "size" << temp.size() << trian.size() << NL;
            side.top();
            for ( j = 0; j < side.size(); ++j )
            {
                const nat si = side.cur()->a;
                Trigon<void> & t = poly3gon.side[si];
                if ( j < trian.size() )
                {
                    const Set3<nat> & s = trian[j];
                    t.a = index[s.a];
                    t.b = index[s.b];
                    t.c = index[s.c];
                    // Добавляем индекс грани в вершины
                    vert[t.a].addAftLas ( new Item1n ( si ) );
                    vert[t.b].addAftLas ( new Item1n ( si ) );
                    vert[t.c].addAftLas ( new Item1n ( si ) );
                }
                else
                {
                    // Помечаем грань для удаления
                    t.a = t.b;
                }
                side.next();
            }
        }
    }
    delSides ( poly3gon );
    delVerts ( poly3gon );
}
/*
void trian3dtest()
{
    static PRand rand;
    const nat N = 5;
    FixArray<Vector2d, N> poly;
    FixArray<Vector3d, N> vert;
    regularPolygon ( poly, 1 );
    for ( nat j = 0; j < poly.size(); ++j )
    {
        vert[j].x = poly[j].x;
        vert[j].y = poly[j].y;
        vert[j].z = rand();
    }
    /*vert[0] = Vector3d ( 1, 1, rand() );
    vert[1] = Vector3d (-1, 1, rand() );
    vert[2] = Vector3d (-1,-1, rand() );
    vert[3] = Vector3d ( 1,-1, rand() );*
    vert *= 0.8;
    const Vector3d p ( 0, 0, 0.5 );
    DynArray< Set3<nat> > res;
    double r = trianNearPoint ( vert, Trian3dNearPoint ( p ), res );
    Triangle3d t;
    for ( nat i = 0; i < res.size(); ++i )
    {
        Set3<nat> & s = res[i];
        t.a = vert[s.a];
        t.b = vert[s.b];
        t.c = vert[s.c];
        draw ( t, 0, 1, 1 );
    }
    drawPoint ( p, 1, 1, 0 );
}*/