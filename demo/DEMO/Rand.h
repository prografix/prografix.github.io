#ifndef RAND_H
#define RAND_H

#include "typedef.h"

class PRand
{
    nat32 next;
public:
    explicit PRand ( nat32 u = 0 ) : next ( u ) {}
    double operator() ()
    {
        next = next * 1103515245 + 12345;
        return next / 4294967296.;
    }
    nat32 number ( nat32 n )
    {
        next = next * 1103515245 + 12345;
        return nat32 ( next * nat64(n) >> 32 );
    }
};

inline double & operator <= ( double & v, PRand & r ) { return v = r(); }

const nat32 qrand_size = 20;

class QRand
{
    nat32 n;
    nat32 a[qrand_size];
public:
    explicit QRand ( nat32 i );
    double operator() ();
    nat32 number ( nat32 m )
    {
        return nat32 ( m * (*this)() );
    }
};

inline double & operator <= ( double & v, QRand & r ) { return v = r(); }

////////////////////// Перетасовать массив 24.09.2010 /////////////////////////

template<class A, class B> inline 
A & shuffle ( A & a, B & b )
{
    for ( nat32 n = a.size(); n > 1; --n ) _swap ( a[n-1], a[b.number(n)] ); 
    return a;
}

////////////////////////////////////////////////////////////////////////////////

class NormalRand
{
public:
    virtual double operator() () = 0;
};

class PNormalRand : public NormalRand
{
    bool ready;
    double v;
    PRand rand;
public:
    explicit PNormalRand ( nat32 u = 0 ) : ready ( false ), rand ( u ) {}
    double operator() ();
};

class QNormalRand : public NormalRand
{
    bool ready;
    double v;
    QRand q1, q2;
public:
    explicit QNormalRand ( nat32 p1 = 2, nat32 p2 = 3 ) : ready ( false ), q1(p1), q2(p2) {}
    double operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class Vector2d;

class RandVector2d
{
public:
    virtual Vector2d operator() () = 0;
};

Vector2d & operator <= ( Vector2d & v, RandVector2d & r );

class PRandVector2d : public RandVector2d
{
    PRand rand;
public:
    explicit PRandVector2d ( nat32 i = 0 ) : rand ( i ) {}
    Vector2d operator() ();
};

class QRandVector2d : public RandVector2d
{
    QRand rand;
public:
    explicit QRandVector2d ( nat32 i = 2 ) : rand ( i ) {}
    Vector2d operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class RandPoint2d
{
public:
    virtual Vector2d operator() () = 0;
};

Vector2d & operator <= ( Vector2d & v, RandPoint2d & r );

class PRandPoint2d : public RandPoint2d
{
    PRand randX, randY;
public:
    explicit PRandPoint2d ( nat32 x = 0, nat32 y = 1 ) : randX(x), randY(y) {}
    Vector2d operator() ();
};

class QRandPoint2d : public RandPoint2d
{
    QRand randX, randY;
public:
    explicit QRandPoint2d ( nat32 x = 2, nat32 y = 3 ) : randX(x), randY(y) {}
    Vector2d operator() ();
};

class QRand2Point2d : public RandPoint2d
{
    double x, y;
public:
    explicit QRand2Point2d () : x(0), y(0) {}
    Vector2d operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class NormalRandPoint2d
{
public:
    virtual Vector2d operator() () = 0;
};

class PNormalRandPoint2d : public NormalRandPoint2d
{
    PRand rand;
public:
    explicit PNormalRandPoint2d ( nat32 u = 0 ) : rand ( u ) {}
    Vector2d operator() ();
};

class QNormalRandPoint2d : public NormalRandPoint2d
{
    QRand q1, q2;
public:
    explicit QNormalRandPoint2d ( nat32 p1 = 2, nat32 p2 = 3 ) : q1(p1), q2(p2) {}
    Vector2d operator() ();
};

///////////////////// Случайный поворот на плоскости ///////////////////////////

class Spin2d getRandSpin2d ( double p );
// p - случайное число равномерно-распределённое на интервале [0,1]

////////////////////////////////////////////////////////////////////////////////

class Vector3d;

class RandVector3d
{
public:
    virtual Vector3d operator() () = 0;
};

Vector3d & operator <= ( Vector3d & v, RandVector3d & r );

class PRandVector3d : public RandVector3d
{
    PRand prand;
public:
    explicit PRandVector3d ( nat32 i = 0 ) : prand ( i ) {}
    Vector3d operator() ();
};

class QRandVector3d : public RandVector3d
{
    QRand q2, q3;
public:
    explicit QRandVector3d ( nat32 a = 2, nat32 b = 3 ) : q2(a), q3(b) {}
    Vector3d operator() ();
};

class QRand2Vector3d : public RandVector3d
{
    double x, y;
public:
    explicit QRand2Vector3d () : x(0), y(0) {}
    Vector3d operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class RandPoint3d
{
public:
    virtual Vector3d operator() () = 0;
};

Vector3d & operator <= ( Vector3d & v, RandPoint3d & r );

class PRandPoint3d : public RandPoint3d
{
    PRand randX, randY, randZ;
public:
    explicit PRandPoint3d ( nat32 x = 9, nat32 y = 1, nat32 z = 2 ) : randX(x), randY(y), randZ(z) {}
    Vector3d operator() ();
};

class QRandPoint3d : public RandPoint3d
{
    QRand randX, randY, randZ;
public:
    explicit QRandPoint3d ( nat32 x = 2, nat32 y = 3, nat32 z = 5 ) : randX(x), randY(y), randZ(z) {}
    Vector3d operator() ();
};

///////////////////// Случайный поворот в пространстве /////////////////////////

// a, b, c - независимые случайные числа равномерно-распределённые на интервале [0,1]

class Ortho3d getRandOrtho3d ( double a, double b, double c );

class Spin3d getRandSpin3d ( double a, double b, double c );


////////////////////////////////////////////////////////////////////////////////

class Vector3f;

class RandVector3f
{
public:
    virtual Vector3f operator() () = 0;
};

class PRandVector3f : public RandVector3f
{
    PRand prand;
public:
    explicit PRandVector3f ( nat32 i = 0 ) : prand ( i ) {}
    Vector3f operator() ();
};

class QRandVector3f : public RandVector3f
{
    QRand q2, q3;
public:
    explicit QRandVector3f ( nat32 a = 2, nat32 b = 3 ) : q2(a), q3(b) {}
    Vector3f operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class RandPoint3f
{
public:
    virtual Vector3f operator() () = 0;
};

class PRandPoint3f : public RandPoint3f
{
    PRand randX, randY, randZ;
public:
    explicit PRandPoint3f ( nat32 x = 9, nat32 y = 1, nat32 z = 2 ) : randX(x), randY(y), randZ(z) {}
    Vector3f operator() ();
};

class QRandPoint3f : public RandPoint3f
{
    QRand randX, randY, randZ;
public:
    explicit QRandPoint3f ( nat32 x = 2, nat32 y = 3, nat32 z = 5 ) : randX(x), randY(y), randZ(z) {}
    Vector3f operator() ();
};

////////////////////////////////////////////////////////////////////////////////

class Vector4d;

class RandVector4d
{
public:
    virtual Vector4d operator() () = 0;
};

class PRandVector4d : public RandVector4d
{
    PRand prand;
public:
    explicit PRandVector4d ( nat32 i = 0 ) : prand ( i ) {}
    Vector4d operator() ();
};

class QRandVector4d : public RandVector4d
{
    QRand q1, q2, q3, q4;
public:
    explicit QRandVector4d ( nat32 p1=2, nat32 p2=3, nat32 p3=5, nat32 p4=7 ) : q1(p1), q2(p2), q3(p3), q4(p4) {}
    Vector4d operator() ();
};

#endif
