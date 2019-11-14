
#ifndef INTERSECT3D_H
#define INTERSECT3D_H

#include "ShevArray.h"
#include "Vector3d.h"
#include "lists.h"

namespace Shev
{
    class Polyhedron;
}

using namespace Shev;

//**************************** 02.08.2012 *********************************//
//
//                     Пересечение точки и шара
//
//**************************** 02.08.2012 *********************************//

inline
bool isIntersect ( const Vector3d & p, const Sphere3d & fig )
{
    return qmod ( p - fig.o ) <= fig.r * fig.r;
}

inline
bool isIntersect ( const Sphere3d & fig, const Vector3d & p )
{
    return isIntersect ( p, fig );
}

//**************************** 01.02.2007 *********************************//
//
//                    Пересечение трёх плоскостей
//
//**************************** 23.05.2010 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane1, const Plane3d & plane2, const Plane3d & plane3 );


//**************************** 01.02.2007 *********************************//
//
//                    Пересечение двух плоскостей
//
//**************************** 23.05.2010 *********************************//

Def<Line3d> intersection ( const Plane3d & plane1, const Plane3d & plane2 );


//**************************** 09.02.2007 *********************************//
//
//                    Пересечение плоскости и прямой
//
//**************************** 23.05.2010 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane, const Line3d & line );
inline
Def<Vector3d> intersection ( const Line3d & line, const Plane3d & plane )
{
    return intersection ( plane, line );
}

//**************************** 25.10.2010 *********************************//
//
//                    Пересечение плоскости и отрезка
//
//**************************** 25.10.2010 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane, const Segment3d & seg );
inline
Def<Vector3d> intersection ( const Segment3d & seg, const Plane3d & plane )
{
    return intersection ( plane, seg );
}

//**************************** 14.05.2008 *********************************//
//
//                    Отсечение плоскостью части отрезка
//
//**************************** 20.08.2011 *********************************//

Def<Segment3d> cut ( const Plane3d & plane, const Segment3d & seg );
inline
Def<Segment3d> cut ( const Segment3d & seg, const Plane3d & plane )
{
    return cut ( plane, seg );
}

//**************************** 04.12.2010 *********************************//
//
//                  Пересечение прямой и шара
//
//**************************** 04.12.2010 *********************************//

Def<Segment3d> intersection ( const Line3d & line, const Sphere3d & sphere );
inline
Def<Segment3d> intersection ( const Sphere3d & sphere, const Line3d & line )
{
    return intersection ( line, sphere );
}

//**************************** 04.12.2010 *********************************//
//
//                  Пересечение отрезка и шара
//
//**************************** 04.12.2010 *********************************//

Def<Segment3d> intersection ( const Segment3d & seg, const Sphere3d & sphere );
inline
Def<Segment3d> intersection ( const Sphere3d & sphere, const Segment3d & seg )
{
    return intersection ( seg, sphere );
}

//**************************** 18.12.2010 *********************************//
//
//                  Пересечение прямой и эллипсоида
//
//**************************** 18.12.2010 *********************************//

Def<Segment3d> intersection ( const Line3d & line, const Ellipsoid3d & e );
inline
Def<Segment3d> intersection ( const Ellipsoid3d & e, const Line3d & line )
{
    return intersection ( line, e );
}

//**************************** 18.12.2010 *********************************//
//
//                  Пересечение отрезка и эллипсоида
//
//**************************** 18.12.2010 *********************************//

Def<Segment3d> intersection ( const Segment3d & seg, const Ellipsoid3d & e );
inline
Def<Segment3d> intersection ( const Ellipsoid3d & e, const Segment3d & seg )
{
    return intersection ( seg, e );
}

//**************************** 28.01.2012 *********************************//
//
//                Пересечение многогранника и плоскости
//
//**************************** 28.01.2012 *********************************//

bool intersection ( const Polyhedron & poly, const Plane3d & plane, 
                    DynArrRef<DynArray<Vector3d> > & res );
inline
bool intersection ( const Plane3d & plane, const Polyhedron & poly, 
                    DynArrRef<DynArray<Vector3d> > & res )
{
    return intersection ( poly, plane, res );
}

//**************************** 11.02.2012 *********************************//
//
//              Отсечение части многогранника плоскостью
//
//        Функция возвращает false, если 
//        1) ссылки poly и res указывают на один тот же объект 
//        2) нормаль плоскости равна нулевому вектору
//        3) произошла ошибка в программе
//
//**************************** 11.02.2012 *********************************//

struct CutStor3d
{
    Suite<double> dist;
    Suite<nat> index, fullFacet;
    Suite<SortItem<Set2<nat>, Vector3d> > vert;
    List1n list1n;
    List2n list2n;
};

bool cut ( const Polyhedron & poly, const Plane3d & plane, Polyhedron & res, 
           CutStor3d & stor, int info = 0 );

inline
bool cut ( const Polyhedron & poly, const Plane3d & plane, Polyhedron & res, int info = 0 )
{
    CutStor3d stor;
    return cut ( poly, plane, res, stor, info );
}

//**************************** 16.08.2012 *********************************//
//
//            Пересечение выпуклого многогранника с произвольным
//
//**************************** 16.08.2012 *********************************//

bool intersect1c ( const Polyhedron & conv, const Polyhedron & poly, Polyhedron & res );

#endif