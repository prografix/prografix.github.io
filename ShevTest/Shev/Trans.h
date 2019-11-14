
#pragma once

#include "Vector2d.h"
#include "Vector3d.h"
#include "ShevArray.h"

// Неортогональное преобразование трёхмерных векторов в двухмерные

typedef Vector2d (* Func3to2) ( const Vector3d & );
Func3to2 getTrans ( const Vector3d & norm );

inline Vector2d trans ( const Vector3d & from, const Vector3d & norm )
{
    return getTrans ( norm ) ( from );
}

ArrRef<Vector2d> trans ( CArrRef<Vector3d> from, const Vector3d & norm, ArrRef<Vector2d> to );

// Неортогональное преобразование двухмерных векторов в трёхмерные

class Func2to3
{
    Vector3d c; // коэффициенты преобразования
    Vector3d (* func) ( const Vector3d &, const Vector2d & );
public:
    explicit Func2to3 ( const Plane3d & );
    Vector3d operator () ( const Vector2d & v ) const { return func ( c, v ); }
};

inline Vector3d trans ( const Vector2d & from, const Plane3d & plane )
{
    return Func2to3 ( plane ) ( from );
}

ArrRef<Vector3d> trans ( CArrRef<Vector2d> from, const Plane3d & plane, ArrRef<Vector3d> to );

// Ортогональное преобразование трёхмерных векторов в двухмерные

class OrthoFunc3to2
{
    Vector3d vx, vy;
public:
    explicit OrthoFunc3to2 ( const Vector3d & norm ) { reper ( norm, vx, vy ); }
    Vector2d operator () ( const Vector3d & v ) const { return Vector2d ( vx*v, vy*v ); }
};

inline Vector2d ortho_trans ( const Vector3d & from, const Vector3d & norm )
{
    return OrthoFunc3to2 ( norm ) ( from );
}

ArrRef<Vector2d> ortho_trans ( CArrRef<Vector3d> from, const Vector3d & norm, ArrRef<Vector2d> to );

// Ортогональное преобразование двухмерных векторов в трёхмерные

class OrthoFunc2to3
{
    Vector3d vx, vy, vz;
public:
    explicit OrthoFunc2to3 ( const Plane3d & plane ) : vz ( - plane.dist * plane.norm )
    {
        reper ( plane.norm, vx, vy );
    }
    Vector3d operator () ( const Vector2d & v ) const { return vx * v.x + vy * v.y + vz; }
};

inline Vector3d ortho_trans ( const Vector2d & from, const Plane3d & plane )
{
    return OrthoFunc2to3 ( plane ) ( from );
}

ArrRef<Vector3d> ortho_trans ( CArrRef<Vector2d> from, const Plane3d & plane, ArrRef<Vector3d> to );

// Преобразование грани многогранника в многоугольник на плоскости

namespace Shev
{
    class Facet;
    class Polyhedron;
}

DynArrRef<Vector2d> & ortho_trans ( CArrRef<Vector3d> v, const Shev::Facet & facet, DynArrRef<Vector2d> & p );
DynArrRef<Vector2d> &       trans ( CArrRef<Vector3d> v, const Shev::Facet & facet, DynArrRef<Vector2d> & p );
