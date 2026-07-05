
#pragma once

#include "Vector2d.h"
#include "ShevArray.h"

//**************************** 18.12.2022 *********************************//
//
//         ¬ычисление момента 4-го пор€дка относительно точки
//
//**************************** 18.12.2022 *********************************//

double momentum4sgm ( const Segment2d & s, const Vector2d & o );
double momentum4sgm ( CCArrRef<Segment2d> & segm, const Vector2d & o );


//**************************** 02.01.2008 *********************************//
//
//     ¬ычисление абсолютного момента 1-го пор€дка относительно пр€мой
//
//**************************** 15.10.2022 *********************************//

double momentum1pnt ( CCArrRef<Vector2d> & point, const Line2d & line );
double momentum1pnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass, const Line2d & line );
double momentum1sgm ( CCArrRef<Segment2d> & segm, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//       ¬ычисление момента 2-го пор€дка относительно пр€мой
//
//**************************** 25.09.2010 *********************************//

double momentum2pnt ( CCArrRef<Vector2d> & point, const Line2d & line );
double momentum2pnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass, const Line2d & line );
double momentum2sgm ( CCArrRef<Segment2d> & segm, const Line2d & line );
double momentum2plg ( CCArrRef<Vector2d>  & vert, const Line2d & line );


//**************************** 24.12.2022 *********************************//
//
//         ¬ычисление момента 4-го пор€дка относительно пр€мой
//
//**************************** 24.12.2022 *********************************//

double momentum4sgm ( const Segment2d & s, const Line2d & line );
double momentum4sgm ( CCArrRef<Segment2d> & segm, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//         ÷ентры масс 2 пор€дка ( точек, отрезков, многоугольника )
//
//**************************** 25.09.2010 *********************************//

Def<Vector2d> centerPnt ( CCArrRef<Vector2d> & point );
Def<Vector2d> centerPnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass );
Def<Vector2d> centerSgm ( CCArrRef<Segment2d> & segm );
Def<Vector2d> centerPlg ( CCArrRef<Vector2d>  & vert );


//**************************** 01.10.2022 *********************************//
//
//         ÷ентры масс 4 пор€дка ( точек, отрезков, многоугольника )
//
//**************************** 18.12.2022 *********************************//

Def<Vector2d> center4pnt ( CCArrRef<Vector2d> & point );
Def<Vector2d> center4sgm ( CCArrRef<Segment2d> & segm );


//**************************** 29.12.2007 *********************************//
//
//       ¬ычисление минимального и максимального момента 2-го пор€дка
//
//**************************** 26.09.2010 *********************************//

struct Mom2d
{
    Vector2d minA, maxA; // оси
    double minM, maxM; // моменты 2-го пор€дка
    Vector2d o; // центр масс
};

Def<Mom2d> momentum2pnt ( CCArrRef<Vector2d> & point );
Def<Mom2d> momentum2pnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass );
Def<Mom2d> momentum2sgm ( CCArrRef<Segment2d> & segm );
Def<Mom2d> momentum2plg ( CCArrRef<Vector2d> & vert );

Def<Mom2d> momentum2plg ( CCArrRef<Vector2d> & vert, const Vector2d & o );


//**************************** 25.09.2009 *********************************//
//
//          ѕостроение эллипса по моментам 2-го пор€дка
//
//**************************** 26.09.2010 *********************************//

Def<Ellipse2d> getEllipse ( const Def<Mom2d> & mom );


//**************************** 25.09.2009 *********************************//
//
//          ѕостроение пр€моугольника по моментам 2-го пор€дка
//
//**************************** 26.09.2010 *********************************//

Def<Rectangle2d> getRectangle ( const Def<Mom2d> & mom );
