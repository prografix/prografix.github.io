
#ifndef INTERSECT2D_H
#define INTERSECT2D_H

#include "ShevArray.h"
#include "Vector2d.h"

//**************************** 08.03.2011 *********************************//
//
//               Пересечение точки и многоугольника
//
//**************************** 25.03.2011 *********************************//

bool isIntersect ( const Vector2d & p, CArrRef<Vector2d> poly );
inline
bool isIntersect ( CArrRef<Vector2d> poly, const Vector2d & p )
{
    return isIntersect ( p, poly );
}

//**************************** 29.07.2012 *********************************//
//
//                     Пересечение точки и круга
//
//**************************** 29.07.2012 *********************************//

inline
bool isIntersect ( const Vector2d & p, const Circle2d & fig )
{
    return qmod ( p - fig.o ) <= fig.r * fig.r;
}

inline
bool isIntersect ( const Circle2d & fig, const Vector2d & p )
{
    return isIntersect ( p, fig );
}


//**************************** 08.09.2006 *********************************//
//
//                      Пересечение двух прямых
//
//**************************** 23.05.2010 *********************************//

Def<Vector2d> intersection ( const Line2d & line1, const Line2d & line2 );


//**************************** 13.10.2010 *********************************//
//
//                      Пересечение прямой и отрезка
//
//**************************** 13.10.2010 *********************************//

Def<Vector2d> intersection ( const Line2d & line, const Segment2d & seg );
inline
Def<Vector2d> intersection ( const Segment2d & seg, const Line2d & line )
{
    return intersection ( line, seg );
}


//**************************** 16.10.2010 *********************************//
//
//                      Пересечение двух отрезков
//
//**************************** 16.10.2010 *********************************//

Def<Vector2d> intersection ( const Segment2d & s1, const Segment2d & s2 );


//**************************** 18.05.2008 *********************************//
//
//                  Отсечение прямой части отрезка
//
//**************************** 18.10.2010 *********************************//

Def<Segment2d> cut ( const Line2d & line, const Segment2d & seg );
inline
Def<Segment2d> cut ( const Segment2d & seg, const Line2d & line )
{
    return cut ( line, seg );
}


//**************************** 27.11.2010 *********************************//
//
//                  Пересечение прямой и круга
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Line2d & line, const Circle2d & cir );
inline
Def<Segment2d> intersection ( const Circle2d & cir, const Line2d & line )
{
    return intersection ( line, cir );
}


//**************************** 27.11.2010 *********************************//
//
//                  Пересечение отрезка и круга
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Segment2d & seg, const Circle2d & cir );
inline
Def<Segment2d> intersection ( const Circle2d & cir, const Segment2d & seg )
{
    return intersection ( seg, cir );
}


//**************************** 11.12.2010 *********************************//
//
//                  Пересечение прямой и эллипса
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Line2d & line, const Ellipse2d & e );
inline
Def<Segment2d> intersection ( const Ellipse2d & e, const Line2d & line )
{
    return intersection ( line, e );
}


//**************************** 11.12.2010 *********************************//
//
//                  Пересечение отрезка и эллипса
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Segment2d & seg, const Ellipse2d & e );
inline
Def<Segment2d> intersection ( const Ellipse2d & e, const Segment2d & seg )
{
    return intersection ( seg, e );
}


//**************************** 08.01.2011 *********************************//
//
//               Пересечение прямой и многоугольника
//
//**************************** 08.01.2011 *********************************//

DynArrRef<Segment2d> & intersection ( const Line2d & line, CArrRef<Vector2d> poly, DynArrRef<Segment2d> & res );
inline
DynArrRef<Segment2d> & intersection ( CArrRef<Vector2d> poly, const Line2d & line, DynArrRef<Segment2d> & res )
{
    return intersection ( line, poly, res );
}


//**************************** 08.01.2011 *********************************//
//
//               Пересечение отрезка и многоугольника
//
//**************************** 08.01.2011 *********************************//

SuiteRef<Segment2d> & intersection ( const Segment2d & seg, CArrRef<Vector2d> poly, SuiteRef<Segment2d> & res );
inline
SuiteRef<Segment2d> & intersection ( CArrRef<Vector2d> poly, const Segment2d & seg, SuiteRef<Segment2d> & res )
{
    return intersection ( seg, poly, res );
}


//**************************** 28.01.2011 *********************************//
//
//               Отсечение части многоугольника прямой
//
//**************************** 28.01.2011 *********************************//

SuiteRef< Suite<Vector2d> > & 
cut ( CArrRef<Vector2d> poly, const Line2d & line, SuiteRef< Suite<Vector2d> > & res );


//**************************** 28.01.2011 *********************************//
//
//            Пересечение выпуклого многоугольника с простым
//
//**************************** 28.01.2011 *********************************//

Suite< Suite<Vector2d> > & 
intersect1c ( CArrRef<Vector2d> conv, CArrRef<Vector2d> poly, 
              Suite< Suite<Vector2d> > & tmp, Suite< Suite<Vector2d> > & res );

Suite< Suite<Vector2d> > & 
intersect1c ( CArrRef<Vector2d> conv, CArrRef<Vector2d> poly, Suite< Suite<Vector2d> > & res );


//**************************** 23.05.2012 *********************************//
//
//             Пересечение двух простых многоугольников
//
//**************************** 23.05.2012 *********************************//

bool intersection ( CArrRef<Vector2d> poly1, CArrRef<Vector2d> poly2, Suite< Suite<Vector2d> > & res );


#endif