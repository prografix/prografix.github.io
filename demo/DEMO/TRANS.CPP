
#include "math.h"
#include "trans.h"
#include "Polyhedron.h"

// Неортогональное преобразование трёхмерных векторов в двухмерные

Vector2d transXP ( const Vector3d & u )
{
    return Vector2d ( u.y, u.z );
}

Vector2d transXO ( const Vector3d & u )
{
    return Vector2d ( u.z, u.y );
}

Vector2d transYP ( const Vector3d & u )
{
    return Vector2d ( u.z, u.x );
}

Vector2d transYO ( const Vector3d & u )
{
    return Vector2d ( u.x, u.z );
}

Vector2d transZP ( const Vector3d & u )
{
    return Vector2d ( u.x, u.y );
}

Vector2d transZO ( const Vector3d & u )
{
    return Vector2d ( u.y, u.x );
}

Func3to2 getTrans ( const Vector3d & norm )
{
    const double ax = fabs ( norm.x );
    const double ay = fabs ( norm.y );
    const double az = fabs ( norm.z );

    if ( ax >= ay && ax >= az )
    {
        return norm.x > 0 ? transXP : transXO;
    }

    if ( ay >= ax && ay >= az )
    {
        return norm.y > 0 ? transYP : transYO;
    }

    {
        return norm.z > 0 ? transZP : transZO;
    }
}

ArrRef<Vector2d> trans ( CArrRef<Vector3d> from, const Vector3d & norm, ArrRef<Vector2d> to )
{
    Func3to2 func = getTrans ( norm );
    const nat n = _min ( from.size(), to.size() );
    for ( nat i = 0; i < n; ++i ) to[i] = func ( from[i] );
    return to;
}

// Неортогональное преобразование двухмерных векторов в трёхмерные

Vector3d _transXP ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.x * c.x + u.y * c.y + c.z, u.x, u.y );
}

Vector3d _transXO ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.y * c.x + u.x * c.y + c.z, u.y, u.x );
}

Vector3d _transYP ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.y, u.x * c.x + u.y * c.y + c.z, u.x );
}

Vector3d _transYO ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.x, u.y * c.x + u.x * c.y + c.z, u.y );
}

Vector3d _transZP ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.x, u.y, u.x * c.x + u.y * c.y + c.z );
}

Vector3d _transZO ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( u.y, u.x, u.y * c.x + u.x * c.y + c.z );
}

Vector3d _trans0 ( const Vector3d & c, const Vector2d & u )
{
    return Vector3d ( 0, 0, 0 );
}

Func2to3::Func2to3 ( const Plane3d & plane )
{
    const double ax = fabs ( plane.norm.x );
    const double ay = fabs ( plane.norm.y );
    const double az = fabs ( plane.norm.z );

    if ( ax >= ay && ax >= az )
    {
        if ( ! ax )
        {
            func = _trans0;
            return;
        }
        const double t = -1. / plane.norm.x;
        c.x = plane.norm.y * t;
        c.y = plane.norm.z * t;
        c.z = plane.dist * t;
        func = plane.norm.x > 0 ? _transXP : _transXO;
    }
    else
    if ( ay >= ax && ay >= az )
    {
        const double t = -1. / plane.norm.y;
        c.x = plane.norm.z * t;
        c.y = plane.norm.x * t;
        c.z = plane.dist * t;
        func = plane.norm.y > 0 ? _transYP : _transYO;
    }
    else
    {
        const double t = -1. / plane.norm.z;
        c.x = plane.norm.x * t;
        c.y = plane.norm.y * t;
        c.z = plane.dist * t;
        func = plane.norm.z > 0 ? _transZP : _transZO;
    }
}

ArrRef<Vector3d> trans ( CArrRef<Vector2d> from, const Plane3d & plane, ArrRef<Vector3d> to )
{
    Func2to3 func ( plane );
    const nat n = _min ( from.size(), to.size() );
    for ( nat i = 0; i < n; ++i ) to[i] = func ( from[i] );
    return to;
}

// Ортогональное преобразование трёхмерных векторов в двухмерные

ArrRef<Vector2d> ortho_trans ( CArrRef<Vector3d> from, const Vector3d & norm, ArrRef<Vector2d> to )
{
    Vector3d vx, vy;
    reper ( norm, vx, vy );
    const nat n = _min ( from.size(), to.size() );
    for ( nat i = 0; i < n; ++i )
    {
        to[i].x = vx * from[i];
        to[i].y = vy * from[i];
    }
    return to;
}

// Ортогональное преобразование двухмерных векторов в трёхмерные

ArrRef<Vector3d> ortho_trans ( CArrRef<Vector2d> from, const Plane3d & plane, ArrRef<Vector3d> to )
{
    Vector3d vx, vy;
    reper ( plane.norm, vx, vy );
    const Vector3d vz = - plane.dist * plane.norm;
    const nat n = _min ( from.size(), to.size() );
    for ( nat i = 0; i < n; ++i ) to[i] = vx * from[i].x + vy * from[i].y + vz;
    return to;
}

// Преобразование грани многогранника в многоугольник на плоскости

DynArrRef<Vector2d> & ortho_trans ( CArrRef<Vector3d> v, const Facet & facet, DynArrRef<Vector2d> & p )
{
    Vector3d vx, vy;
    reper ( facet.plane.norm, vx, vy );
    p.resize ( facet.nv );
    for ( nat i = 0; i < facet.nv; ++i )
    {
        const Vector3d & t = v[facet.index[i]];
        p[i].x = vx * t;
        p[i].y = vy * t;
    }
    return p;
}

DynArrRef<Vector2d> & trans ( CArrRef<Vector3d> v, const Facet & facet, DynArrRef<Vector2d> & p )
{
    const double ax = fabs ( facet.plane.norm.x );
    const double ay = fabs ( facet.plane.norm.y );
    const double az = fabs ( facet.plane.norm.z );
    CArrRef<nat> index ( facet.index, 0, facet.nv );
    p.resize ( facet.nv );
    if ( ax >= ay && ax >= az )
    {
        if ( facet.plane.norm.x > 0 )
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.y;
                p[i].y = t.z;
            }
        }
        else
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.z;
                p[i].y = t.y;
            }
        }
    }
    else
    if ( ay >= ax && ay >= az )
    {
        if ( facet.plane.norm.y > 0 )
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.z;
                p[i].y = t.x;
            }
        }
        else
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.x;
                p[i].y = t.z;
            }
        }
    }
    else
    {
        if ( facet.plane.norm.z > 0 )
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.x;
                p[i].y = t.y;
            }
        }
        else
        {
            for ( nat i = 0; i < facet.nv; ++i )
            {
                const Vector3d & t = v[index[i]];
                p[i].x = t.y;
                p[i].y = t.x;
            }
        }
    }
    return p;
}

