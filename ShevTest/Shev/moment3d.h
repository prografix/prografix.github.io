
#ifndef MOMENT3D_H
#define MOMENT3D_H

#include "ShevArray.h"
#include "Vector3d.h"

namespace Shev
{
    class Facet;
    class Polyhedron;
}

using namespace Shev;

//**************************** 09.01.2008 *********************************//
//
//            Центры масс ( точек, отрезков, грани и многогранника )
//
//**************************** 17.11.2012 *********************************//

Def<Vector3d> centerOfMass ( CArrRef<Vector3d> point ); // Центр масс набора точек

Def<Vector3d> centerOfMass ( CArrRef<Vector3d> point, CArrRef<double> mass ); // Центр масс набора точек с весами

Def<Vector3d> centerOfMass ( CArrRef<Segment3d> segm ); // Центр масс набора отрезков

Def<Vector3d> centerOfMass ( nat i, const Polyhedron & poly ); // Центр масс i-той грани

Def<Vector3d> centerOfMass ( const Facet & facet, CArrRef<Vector3d> vert ); // Центр масс грани

Def<Vector3d> centerOfMass ( const Polyhedron & poly ); // Центр масс многогранника


//**************************** 10.01.2008 *********************************//
//
//       Вычисление моментов 2-го порядка относительно плоскости
//
//**************************** 04.09.2010 *********************************//

double momentum2pnt ( CArrRef<Vector3d> point, const Plane3d & plane );

double momentum2pnt ( CArrRef<Vector3d> point, CArrRef<double> mass, const Plane3d & plane );

double momentum2sgm ( CArrRef<Segment3d> segm, const Plane3d & plane );

double momentum2fct ( nat i, const Polyhedron & poly, const Plane3d & plane );

double momentum2plh ( const Polyhedron & poly, const Plane3d & plane );


//**************************** 10.01.2008 *********************************//
//
//           Вычисление основных осей и моментов 2-го порядка
//
//**************************** 28.09.2010 *********************************//

struct Mom3d
{
    double minMom, midMom, maxMom;
    Vector3d minNorm, midNorm, maxNorm;
    Vector3d o;
};

Def<Mom3d> solve ( double xx, double xy, double xz, double yy, double yz, double zz, const Vector3d & o );

Def<Mom3d> momentum2pnt ( CArrRef<Vector3d> point );

Def<Mom3d> momentum2pnt ( CArrRef<Vector3d> point, CArrRef<double> mass );

Def<Mom3d> momentum2sgm ( CArrRef<Segment3d> segm );

Def<Mom3d> momentum2fct ( nat i, const Polyhedron & poly );

Def<Mom3d> momentum2plh ( const Polyhedron & poly );


//**************************** 29.11.2008 *********************************//
//
//          Создание эллипсоида по моментам 2-го порядка
//
//**************************** 29.09.2010 *********************************//

Def<Ellipsoid3d> getEllipsoid ( const Def<Mom3d> & mom );


//*************************** 15.05.2009 **********************************//
//
//    Создание прямоугольного параллелепипеда по моментам 2-го порядка
//
//*************************** 29.09.2010 **********************************//

Def<Cuboid3d> getCuboid ( const Def<Mom3d> & mom );


#endif