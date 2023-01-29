
#include "math.h"
#include "tune.h"
#include "rand.h"

/////////////////////////////////////////////////////////////////////

QRand::QRand ( nat32 i ) : n ( i )
{
    for ( nat32 k = 0; k < qrand_size; ++k ) a[k] = 0;
}

double QRand::operator() ()
{
    ++a[0];
    nat32 k;
    for ( k = 0; k < qrand_size; ++k )
    {
        if ( a[k] < n ) break;
        a[k] = 0;
        nat32 m = k + 1;
        if ( m == qrand_size ) m = 0;
        ++a[m];
    }
    double res = 0.;
    double mul = 1. / n;
    for ( k = qrand_size; k > 0; )
    {
        res += a[--k];
        res *= mul;
    }
    return res;
}

double PNormalRand::operator() ()
{
    if ( ready )
    {
        ready = false;
        return v;
    }
    const double a = sqrt ( -2. * log ( 1. - rand() ) );
    const double b = 2. * M_PI * rand();
    v = a * sin ( b );
    ready = true;
    return a * cos ( b );
}

double QNormalRand::operator() ()
{
    if ( ready )
    {
        ready = false;
        return v;
    }
    const double a = sqrt ( -2. * log ( q2() ) );
    const double b = 2. * M_PI * q1();
    v = a * sin ( b );
    ready = true;
    return a * cos ( b );
}

/////////////////////////////////////////////////////////////////////

#ifdef VECTOR2D_H_IN_RAND_CPP

#include "Vector2d.h"

Vector2d & operator <= ( Vector2d & v, RandVector2d & r ) { return v = r(); }

Vector2d PRandVector2d::operator() ()
{
    const double a = 2. * M_PI * rand();
    return Vector2d ( cos ( a ), sin ( a ) );
}

Vector2d QRandVector2d::operator() ()
{
    const double a = 2. * M_PI * rand();
    return Vector2d ( cos ( a ), sin ( a ) );
}

Vector2d & operator <= ( Vector2d & v, RandPoint2d & r ) { return v = r(); }

Vector2d PRandPoint2d::operator() ()
{
    return Vector2d ( 2. * randX() - 1., 2. * randY() - 1. );
}

Vector2d QRandPoint2d::operator() ()
{
    return Vector2d ( 2. * randX() - 1., 2. * randY() - 1. );
}

const double f2x = 1/1.32471795724474602596;
const double f2y = f2x * f2x;

Vector2d QRand2Point2d::operator() ()
{
    if ( ( x += f2x ) > 1 ) x -= 1;
    if ( ( y += f2y ) > 1 ) y -= 1;
    return Vector2d ( x+x-1, y+y-1 );
}

Vector2d PNormalRandPoint2d::operator() ()
{
    const double a = sqrt ( -2. * log ( 1. - rand() ) );
    const double b = 2. * M_PI * rand();
    return Vector2d ( a * sin ( b ), a * cos ( b ) );
}

Vector2d QNormalRandPoint2d::operator() ()
{
    const double a = sqrt ( -2. * log ( q2() ) );
    const double b = 2. * M_PI * q1();
    return Vector2d ( a * sin ( b ), a * cos ( b ) );
}

Spin2d getRandSpin2d ( double p ) // p - случайное число в диапазоне [0,1]
{
    return Spin2d ( M_2PI * p );
}

////////////////////////////////////////////////////////////////////////////////

#endif // VECTOR2D_H_IN_RAND_CPP

/////////////////////////////////////////////////////////////////////

#ifdef VECTOR3D_H_IN_RAND_CPP

#include "Vector3d.h"

Vector3d & operator <= ( Vector3d & v, RandVector3d & r ) { return v = r(); }

Vector3d PRandVector3d::operator() ()
{
    const double a = 2. * prand() - 1.;
    const double b = 2. * M_PI * prand();
    const double c = sqrt ( 1. - a * a );
    return Vector3d ( c * cos ( b ), c * sin ( b ), a );
}

Vector3d QRandVector3d::operator() ()
{
    const double a = 2. * q2() - 1.;
    const double b = 2. * M_PI * q3();
    const double c = sqrt ( 1. - a * a );
    return Vector3d ( c * cos ( b ), c * sin ( b ), a );
}

Vector3d QRand2Vector3d::operator() ()
{
    if ( ( x += f2x ) > 1 ) x -= 1;
    if ( ( y += f2y ) > 1 ) y -= 1;
    const double a = 2. * x - 1.;
    const double b = 2. * M_PI * y;
    const double c = sqrt ( 1. - a * a );
    return Vector3d ( c * cos ( b ), c * sin ( b ), a );
}

Vector3d & operator <= ( Vector3d & v, RandPoint3d & r ) { return v = r(); }

Vector3d PRandPoint3d::operator() ()
{
    return Vector3d ( 2. * randX() - 1., 2. * randY() - 1., 2. * randZ() - 1. );
}

Vector3d QRandPoint3d::operator() ()
{
    return Vector3d ( 2. * randX() - 1., 2. * randY() - 1., 2. * randZ() - 1. );
}

// Случайный поворот в пространстве 
// a, b, c - независимые случайные числа в диапазоне [0,1]

Ortho3d getRandOrtho3d ( double a, double b, double c )
{
    if ( a < 0 ) a = 0; else
    if ( a > 1 ) a = 1;
    a = 2 * a - 1;
    const double s = sqrt ( 1 - a * a );
    b *= M_2PI;
    const double cb = cos ( b );
    const double sb = sin ( b );
    c *= M_2PI;
    const double cc = cos ( c );
    const double sc = sin ( c );
    Ortho3d ortho;
    LinTran3d & r = (LinTran3d &) ortho;
    r.x.x =   a;      r.x.y =   s * cb;                r.x.z =   s * sb;
    r.y.x =   s * cc; r.y.y = - a * cb * cc + sb * sc; r.y.z = - a * sb * cc - cb * sc;
    r.z.x = - s * sc; r.z.y =   a * cb * sc + sb * cc; r.z.z =   a * sb * sc - cb * cc;
    return ortho;
}

Spin3d getRandSpin3d ( double a, double b, double c )
{
    if ( a < 0 ) a = 0; else
    if ( a > 1 ) a = 1;
    const double s = sqrt (   a   );
    const double q = sqrt ( 1 - a );
    b *= M_PI;
    c *= M_PI;
    return Spin3d ( s*sin(b+c), s*cos(b+c), q*cos(b-c), q*sin(b-c) );
}

#endif // VECTOR3D_H_IN_RAND_CPP

/////////////////////////////////////////////////////////////////////

#ifdef VECTOR3F_H_IN_RAND_CPP

#include "Vector3f.h"

Vector3f PRandVector3f::operator() ()
{
    const double a = 2. * prand() - 1.;
    const double b = 2. * M_PI * prand();
    const double c = sqrt ( 1. - a * a );
    return Vector3f ( float ( c * cos ( b ) ), float ( c * sin ( b ) ) , float ( a ) );
}

Vector3f QRandVector3f::operator() ()
{
    const double a = 2. * q2() - 1.;
    const double b = 2. * M_PI * q3();
    const double c = sqrt ( 1. - a * a );
    return Vector3f ( float ( c * cos ( b ) ), float ( c * sin ( b ) ) , float ( a ) );
}

Vector3f PRandPoint3f::operator() ()
{
    return Vector3f ( float ( 2. * randX() - 1. ), float ( 2. * randY() - 1. ), float ( 2. * randZ() - 1. ) );
}

Vector3f QRandPoint3f::operator() ()
{
    return Vector3f ( float ( 2. * randX() - 1. ), float ( 2. * randY() - 1. ), float ( 2. * randZ() - 1. ) );
}

#endif // VECTOR3F_H_IN_RAND_CPP

/////////////////////////////////////////////////////////////////////

#ifdef VECTOR4D_H_IN_RAND_CPP

#include "Vector4d.h"

Vector4d PRandVector4d::operator() ()
{
    Vector4d v;
m1: v.x1 = 2. * prand() - 1.;
    v.x2 = 2. * prand() - 1.;
    v.x3 = 2. * prand() - 1.;
    v.x4 = 2. * prand() - 1.;
    double d = v * v;
    if ( d > 1. ) goto m1;
    return v /= sqrt ( d );
}

Vector4d QRandVector4d::operator() ()
{
    Vector4d v;
m1: v.x1 = 2. * q1() - 1.;
    v.x2 = 2. * q2() - 1.;
    v.x3 = 2. * q3() - 1.;
    v.x4 = 2. * q4() - 1.;
    double d = v * v;
    if ( d > 1. ) goto m1;
    return v /= sqrt ( d );
}

#endif // VECTOR4D_H_IN_RAND_CPP

/////////////////////////////////////////////////////////////////////
