
#ifndef MOMENT2D_H
#define MOMENT2D_H

#include "Vector2d.h"
#include "ShevArray.h"


//**************************** 29.12.2007 *********************************//
//
//            Центры масс ( точек, отрезков, многоугольника )
//
//**************************** 25.09.2010 *********************************//

Def<Vector2d> centerPnt ( CArrRef<Vector2d> point );
Def<Vector2d> centerPnt ( CArrRef<Vector2d> point, CArrRef<double> mass );
Def<Vector2d> centerSgm ( CArrRef<Segment2d> segm );
Def<Vector2d> centerPlg ( CArrRef<Vector2d> vert );


//**************************** 02.01.2008 *********************************//
//
//     Вычисление абсолютного момента 1-го порядка относительно прямой
//
//**************************** 25.09.2010 *********************************//

double momentum1Apnt ( CArrRef<Vector2d> point, const Line2d & line );
double momentum1Apnt ( CArrRef<Vector2d> point, CArrRef<double> mass, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//       Вычисление момента 2-го порядка относительно прямой
//
//**************************** 25.09.2010 *********************************//

double momentum2pnt ( CArrRef<Vector2d> point, const Line2d & line );
double momentum2pnt ( CArrRef<Vector2d> point, CArrRef<double> mass, const Line2d & line );
double momentum2sgm ( CArrRef<Segment2d> segm, const Line2d & line );
double momentum2plg ( CArrRef<Vector2d > vert, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//       Вычисление минимального и максимального момента 2-го порядка
//
//**************************** 26.09.2010 *********************************//

struct Mom2d
{
    Vector2d minA, maxA; // оси
    double minM, maxM; // моменты 2-го порядка
    Vector2d o; // центр масс
};

Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point );
Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point, CArrRef<double> mass );
Def<Mom2d> momentum2sgm ( CArrRef<Segment2d> segm );
Def<Mom2d> momentum2plg ( CArrRef<Vector2d> vert );

Mom2d momentum2plg ( CArrRef<Vector2d> vert, const Vector2d & o );


//**************************** 25.09.2009 *********************************//
//
//          Построение эллипса по моментам 2-го порядка
//
//**************************** 26.09.2010 *********************************//

Def<Ellipse2d> getEllipse ( const Def<Mom2d> & mom );


//**************************** 25.09.2009 *********************************//
//
//          Построение прямоугольника по моментам 2-го порядка
//
//**************************** 26.09.2010 *********************************//

Def<Rectangle2d> getRectangle ( const Def<Mom2d> & mom );

#endif