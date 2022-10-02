
#include "math.h"
#include "moment2d.h"

const double M_PI = 3.14159265358979323846;

//**************************** 29.12.2007 *********************************//
//
//         Центры масс 2 порядка ( точек, отрезков, многоугольника )
//
//**************************** 25.09.2010 *********************************//

Def<Vector2d> centerPnt ( CArrRef<Vector2d> point )
{
    Vector2d o ( 0, 0 );
    return point.size() == 0 ? Def<Vector2d>() : Def<Vector2d> ( ( o += point ) / point.size() );
}

Def<Vector2d> centerPnt ( CArrRef<Vector2d> point, CArrRef<double> mass )
{
    double m = 0.;
    Vector2d o ( 0, 0 );
    for ( nat i = 0; i < point.size(); ++i )
    {
        o += mass[i] * point[i];
        m += mass[i];
    }
    return m == 0 ? Def<Vector2d>() : Def<Vector2d> ( o / m );
}

Def<Vector2d> centerSgm ( CArrRef<Segment2d> segm )
{
    double m = 0.;
    Vector2d o ( 0., 0. );
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment2d & s = segm[i];
        const double len = norm2 ( s );
        o += len * ( s.a + s.b );
        m += len;
    }
    return m == 0 ? Def<Vector2d>() : Def<Vector2d> ( o / ( m + m ) );
}

Def<Vector2d> centerPlg ( CArrRef<Vector2d> vert )
{
    if ( vert.size() < 3 ) return Def<Vector2d>();
    Vector2d o ( 0., 0. );
    double m = 0.;
    nat j = vert.size() - 1;
    for ( nat i = 0; i < vert.size(); ++i )
    {
        const double a = vert[j] % vert[i];
        o += a * ( vert[j] + vert[i] );
        m += a;
        j = i;
    }
    return m == 0 ? Def<Vector2d>() : Def<Vector2d> ( o / ( 3 * m ) );
}

//**************************** 01.10.2022 *********************************//
//
//         Центры масс 4 порядка ( точек, отрезков, многоугольника )
//
//**************************** 01.10.2022 *********************************//

Def<Vector2d> center4pnt ( CCArrRef<Vector2d> & point )
{
    const nat n = point.size();
    if ( ! n ) return Def<Vector2d>();
    nat i;
    Vector2d o ( 0, 0 );
    for ( i = 0; i < n; ++i ) o += point[i];
    o /= n;
    for ( i = 0; i < 11; ++i )
    {
        Vector2d o1 ( 0, 0 );
        double c = 0;
        for ( nat j = 0; j < n; ++j )
        {
            double d = qmod ( point[j] - o );
            o1 += d * ( point[j] + o );
            c += d;
        }
        if ( c ) o1 /= 2 * c;
        o = o1;
    }
    return o;
}

//**************************** 02.01.2008 *********************************//
//
//     Вычисление абсолютного момента 1-го порядка относительно прямой
//
//**************************** 25.09.2010 *********************************//

double momentum1Apnt ( CArrRef<Vector2d> point, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        sum += fabs ( line % point[i] );
    }
    return sum;
}

double momentum1Apnt ( CArrRef<Vector2d> point, CArrRef<double> mass, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        sum += fabs ( line % point[i] ) * mass[i];
    }
    return sum;
}

//**************************** 29.12.2007 *********************************//
//
//       Вычисление момента 2-го порядка относительно прямой
//
//**************************** 25.09.2010 *********************************//

double momentum2pnt ( CArrRef<Vector2d> point, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double d = line % point[i];
        sum += d * d;
    }
    return sum;
}

double momentum2pnt ( CArrRef<Vector2d> point, CArrRef<double> mass, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double d = line % point[i];
        sum += d * d * mass[i];
    }
    return sum;
}

double momentum2sgm ( CArrRef<Segment2d> segm, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment2d & s = segm[i];
        const double len = norm2 ( s );
        const double a = line % s.a;
        const double b = line % s.b;
        sum += ( a * ( a + b ) + b * b ) * len;
    }
    return sum / 3.;
}

double momentum2plg ( CArrRef<Vector2d> vert, const Line2d & line )
{
    if ( vert.size() < 3 ) return 0.;
    const Vector2d & va = vert[0];
    const double a = line % va;
    double b = line % vert[1];
    double sum = 0.;
    for ( nat i = 2; i < vert.size(); ++i )
    {
        const Vector2d & vb = vert[i-1];
        const Vector2d & vc = vert[i];
        const double s = ( vb - va ) % ( vc - vb );
        const double c = line % vert[i];
        sum += ( a * ( a + b + c ) + b * ( b + c ) + c * c ) * s;
        b = c;
    }
    return sum / 12.;
}

//**************************** 29.12.2007 *********************************//
//
//       Вычисление минимального и максимального момента 2-го порядка
//
//**************************** 28.09.2010 *********************************//

namespace
{
    Mom2d solve ( double xx, double xy, double yy, const Vector2d & o )
    {
        Mom2d mom;
        mom.o = o;
        const double a = xx + yy;
        const double b = ( xx - yy ) * 0.5;
        const double d = sqrt ( xy * xy + b * b );
        mom.maxM = a * 0.5 + d;
        mom.maxA.x = - xy;
        mom.maxA.y = b - d;
        const double q = mom.maxA * mom.maxA;
        if ( q == 0 )
        {
            mom.maxA.x = 1.;
            mom.maxA.y = 0.;
        }
        else
        {
            mom.maxA /= sqrt ( q );
        }
        mom.minM = a - mom.maxM;
        mom.minA = mom.maxA.leftPerpendicular();
        return mom;
    }
} // namespace

Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point )
{
    const Def<Vector2d> o = centerPnt ( point );
    if ( ! o.isDef ) return Def<Mom2d>();
    double xx = 0., xy = 0., yy = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const Vector2d v = point[i] - o;
        xx += v.y * v.y;
        xy -= v.x * v.y;
        yy += v.x * v.x;
    }
    return solve ( xx, xy, yy, o );
}

Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point, CArrRef<double> mass )
{
    const Def<Vector2d> o = centerPnt ( point, mass );
    if ( ! o.isDef ) return Def<Mom2d>();
    double xx = 0., xy = 0., yy = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const Vector2d v = point[i] - o;
        xx += v.y * v.y * mass[i];
        xy -= v.x * v.y * mass[i];
        yy += v.x * v.x * mass[i];
    }
    return solve ( xx, xy, yy, o );
}

Def<Mom2d> momentum2sgm ( CArrRef<Segment2d> segm )
{
    const Def<Vector2d> o = centerSgm ( segm );
    if ( ! o.isDef ) return Def<Mom2d>();
    double xx = 0., xy = 0., yy = 0.;
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment2d & s = segm[i];
        const double len = norm2 ( s );
        const Vector2d a = s.a - o;
        const Vector2d b = s.b - o;
        xx += ( a.y * ( a.y + b.y ) + b.y * b.y ) * len;
        xy -= ( a.x * ( a.y + a.y + b.y ) + b.x * ( a.y + b.y + b.y ) ) * len;
        yy += ( a.x * ( a.x + b.x ) + b.x * b.x ) * len;
    }
    return solve ( xx / 3, xy / 6, yy / 3, o );
}

Mom2d momentum2plg ( CArrRef<Vector2d> vert, const Vector2d & o )
{
    Vector2d a = vert.las() - o;
    double xx = 0., xy = 0., yy = 0.;
    for ( nat i = 0; i < vert.size(); ++i )
    {
        const Vector2d b = vert[i] - o;
        const double s = a % b;
        xx += ( a.y * ( a.y + b.y ) + b.y * b.y ) * s;
        xy -= ( a.x * ( a.y + a.y + b.y ) + b.x * ( a.y + b.y + b.y ) ) * s;
        yy += ( a.x * ( a.x + b.x ) + b.x * b.x ) * s;
        a = b;
    }
    return solve ( xx / 12, xy / 24, yy / 12, o );
}

Def<Mom2d> momentum2plg ( CArrRef<Vector2d> vert )
{
    const Def<Vector2d> o = centerPlg ( vert );
    return o.isDef ? momentum2plg ( vert, o ) : Def<Mom2d>();
}

//**************************** 25.09.2009 *********************************//
//
//          Построение эллипса по моментам 2-го порядка
//
//**************************** 26.09.2010 *********************************//

Def<Ellipse2d> getEllipse ( const Def<Mom2d> & mom )
{
    Def<Ellipse2d> res;
    if ( ! mom.isDef ) return res;
    res.o = mom.o;
    const double x = fabs ( mom.minM );
    const double y = fabs ( mom.maxM );
    const double c = sqrt ( x / y );
    res.a = sqrt ( sqrt ( 4 / M_PI * y / c ) );
    res.b = res.a * c;
    res.spin = Spin2d ( atan2 ( mom.minA.y, mom.minA.x ) );
    res.isDef = true;
    return res;
}

//**************************** 25.09.2009 *********************************//
//
//          Построение прямоугольника по моментам 2-го порядка
//
//**************************** 26.09.2010 *********************************//

Def<Rectangle2d> getRectangle ( const Def<Mom2d> & mom )
{
    Def<Rectangle2d> res;
    if ( ! mom.isDef ) return res;
    res.o = mom.o;
    const double x = fabs ( mom.minM );
    const double y = fabs ( mom.maxM );
    const double c = sqrt ( x / y );
    res.a = sqrt ( sqrt ( 0.75 * y / c ) );
    res.b = res.a * c;
    res.spin = Spin2d ( atan2 ( mom.minA.y, mom.minA.x ) );
    res.isDef = true;
    return res;
}
