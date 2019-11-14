
#pragma once

#include "math.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "ShevArray.h"

class IVertFunc
{
public:
    virtual bool operator() ( const Vector3d & v ) = 0;
    virtual bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc ) = 0;
};

class AndFunc : public IVertFunc
{
public:
    Suite<IVertFunc *> array;

    bool operator() ( const Vector3d & v )
    {
        for ( nat i = 0; i < array.size(); ++i )
            if ( ! (*array[i]) ( v ) ) return false;
        return true;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        Vector3d v;
        bool is = false;
        for ( nat i = 0; i < array.size(); ++i )
        {
            if ( ! (*array[i]) ( va, vb, v ) ) continue;
            if ( is )
            {
                if ( qmod ( v - va ) < qmod ( vc - va ) ) vc = v;
            }
            else
            {
                vc = v;
                is = true;
            }
        }
        return is;
    }
};

class AndNotFunc : public IVertFunc
{
public:
    Suite<IVertFunc *> array;

    bool operator() ( const Vector3d & v )
    {
        for ( nat i = 0; i < array.size(); ++i )
            if ( (*array[i]) ( v ) ) return false;
        return true;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        Vector3d v;
        bool is = false;
        for ( nat i = 0; i < array.size(); ++i )
        {
            if ( ! (*array[i]) ( vb, va, v ) ) continue;
            if ( is )
            {
                if ( qmod ( v - vb ) > qmod ( vc - vb ) ) vc = v;
            }
            else
            {
                vc = v;
                is = true;
            }
        }
        return is;
    }
};

class OrFunc : public IVertFunc
{
public:
    Suite<IVertFunc *> array;

    bool operator() ( const Vector3d & v )
    {
        for ( nat i = 0; i < array.size(); ++i )
            if ( (*array[i]) ( v ) ) return true;
        return false;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        Vector3d v;
        bool is = false;
        for ( nat i = 0; i < array.size(); ++i )
        {
            if ( ! (*array[i]) ( va, vb, v ) ) continue;
            if ( is )
            {
                if ( qmod ( v - va ) > qmod ( vc - va ) ) vc = v;
            }
            else
            {
                vc = v;
                is = true;
            }
        }
        return is;
    }
};

class Sphere : public IVertFunc
{
    double r2;
    Vector3d o;
public:
    Sphere () {}
    Sphere ( const Vector3d & c, double r ) : o ( c ), r2(r*r) {}
    bool operator() ( const Vector3d & v )
    {
        return qmod ( v - o ) <= r2;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        if ( ! (*this) ( va ) || (*this) ( vb ) ) return false;
        const Vector3d ua = va - o;
        const Vector3d ub = vb - o;
        const Vector3d ba = ub - ua;
        const double A = ba * ba;
        const double B = ua * ba;
        const double C = ua * ua - r2;
        const double D = B * B - A * C;
        double t = D < 0 ? 0 : ( -B + sqrt ( D ) ) / A;
        vc = va + t * ba;
        return true;
    }
};

class Torus : public IVertFunc
{
    double r2, r, R;
    Vector3d o;
public:
    Torus () {}
    Torus ( const Vector3d & c, double rb, double rs ) : o ( c ), R(rb), r(rs), r2(rs*rs) {}
    bool operator() ( const Vector3d & v )
    {
        Vector3d w = v;
        w.y *= 0.8;
        const Vector3d u = w - o;
        return _pow2 ( sqrt ( u.x*u.x + u.y*u.y ) - R ) + u.z*u.z <= r2;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        Vector3d wa = va;
        wa.y *= 0.8;
        Vector3d wb = vb;
        wb.y *= 0.8;
        const Vector3d ua = wa - o;
        double ra = _pow2 ( sqrt ( ua.x*ua.x + ua.y*ua.y ) - R ) + ua.z*ua.z;
        if ( ra > r2 ) return false;
        const Vector3d ub = wb - o;
        double rb = _pow2 ( sqrt ( ub.x*ub.x + ub.y*ub.y ) - R ) + ub.z*ub.z;
        if ( rb <= r2 ) return false;
        ra = sqrt ( ra ) - r;
        rb = sqrt ( rb ) - r;
        double t = ra / ( ra - rb );
        vc = va + t * ( vb - va );
        return true;
    }
};

class Cylinder : public IVertFunc
{
    double r, h; // радиус и половина высоты
    Vector3d o;  // центр
    Ortho3d rot; // поворот
public:
    Cylinder () {}
    Cylinder ( const Vector3d & a, const Vector3d & b, double c ) : r(c), o ( 0.5 * ( a + b ) ), 
        rot ( Spin3d ( ( a - b ), Vector3d ( 0, 0, 1 ) ) ), h ( 0.5 * norm2 ( a - b ) )
    {
    }
    bool operator() ( const Vector3d & v )
    {
        const Vector3d u = rot ( v - o );
        return fabs ( u.z ) <= h && u.x*u.x + u.y*u.y <= r*r;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        const Vector3d ua = rot ( va - o );
        const Vector3d ub = rot ( vb - o );
        if ( fabs ( ua.z ) > h ) return false;
        const double aa = ua.x * ua.x + ua.y * ua.y;
        const double rr = r * r;
        if ( aa > rr ) return false;
        if ( fabs ( ub.z ) > h )
        {
            double da, db;
            if ( ub.z > 0 )
            {
                da = h - ua.z;
                db = ub.z - h;
            }
            else
            {
                da = - ( ua.z + h );
                db = ub.z + h;
            }
            double dd = db + da;
            da /= dd;
            db /= dd;
            vc = ua * db + ub * da;
            double s = vc.x * vc.x + vc.y * vc.y;
            if ( s > rr )
            {
                const Vector2d ba ( vc.x - ua.x, vc.y - ua.y );
                const double A = ba * ba;
                const double B = ba * Vector2d ( ua.x, ua.y );
                const double C = aa - r * r;
                const double D = B * B - A * C;
                double t = D < 0 ? 0 : ( -B + sqrt ( D ) ) / A;
                vc = ua + t * ( vc - ua );
            }
        }
        else
        {
            double s = ub.x * ub.x + ub.y * ub.y;
            if ( s > rr )
            {
                const Vector2d ba ( ub.x - ua.x, ub.y - ua.y );
                const double A = ba * ba;
                const double B = ba * Vector2d ( ua.x, ua.y );
                const double C = aa - r * r;
                const double D = B * B - A * C;
                double t = D < 0 ? 0 : ( -B + sqrt ( D ) ) / A;
                vc = ua + t * ( ub - ua );
            }
            else
                return false;
        }
        vc = (~rot) ( vc ) + o;
        return true;
    }
};

class LayerFunc : public IVertFunc
{
    Plane3d plane1, plane2;
public:
    LayerFunc ( const Plane3d & p1, const Plane3d & p2 ) : plane1(p1), plane2(p2) {}
    bool operator() ( const Vector3d & v )
    {
        return _max ( plane1 % v, plane2 % v ) <= 0;
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        vc = 0.5 * ( va + vb );
        if ( ! (*this) ( va ) || (*this) ( vb ) ) return false;
        if ( plane1 % vb > 0 )
        {
            double a = plane1 % va;
            double b = plane1 % vb;
            double t = a / ( a - b );
            vc = va + ( vb - va ) * t;
        }
        else
        {
            double a = plane2 % va;
            double b = plane2 % vb;
            double t = a / ( a - b );
            vc = va + ( vb - va ) * t;
        }
        return true;
    }
};
