
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/LinAlg.h"
#include "../Shev/Mathem.h"
#include "../Shev/func2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/opti2d.h"
#include "../Shev/trian2d.h"
#include "../Shev/approx2d.h"
#include "../Shev/RealFile.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/WireModel.h"
#include "../Shev/intersect2d.h"
#include "../Shev/heap.h"
#include "../Shev/lists.h"

#include "../draw.h"
#include "display.h"

double timeInSec();

bool savePgn ( const char * name, CArrRef<Vector2d> vert )
{
    RealFile file ( name, "wb" );
    if ( ! file.isValid() ) return false;
    return vert.size() == 0 || file.write ( &vert[0], sizeof(Vector2d), vert.size() ) == vert.size();
}

bool loadPgn ( const char * name, SuiteRef<Vector2d> & vert )
{
    RealFile file ( name, "rb" );
    if ( ! file.isValid() ) return false;
    nat n = 0;
    for (;;)
    {
        Vector2d v;
        n = file.read ( &v, 1, sizeof(Vector2d) );
        if ( n < sizeof(Vector2d) ) break;
        vert.inc() = v;
    }
    return n == 0;
}

namespace
{
    bool points2lines ( CArrRef<Vector2d> poly, ArrRef<Line2d> line )
    {
    // Для каждой стороны вычислим параметры уравнения прямой
    // Уравнение прямой имеет вид: norm.x * x + norm.y * y + dist = 0
        for ( nat i = 0; i < poly.size(); ++i )
        {
            const Vector2d & v2 = poly[i+1<poly.size()?i+1:0];
            const Vector2d & v1 = poly[i];
            Line2d & linei = line[i];
            Vector2d & norm = linei.norm;
            norm.x = v2.y - v1.y;
            norm.y = v1.x - v2.x;
            const double q = norm * norm;
            if ( q == 0 ) return false;
            norm /= sqrt ( q );
            const double d1 = norm * v1;
            const double d2 = norm * v2;
            linei.dist = d1 > d2 ? - d1 : - d2;
        }
        return true;
    }

    bool points2lines ( CArrRef<Vector2d> poly, Conform2d & conf, ArrRef<Line2d> line )
    {
    // Определение габаритов многоугольника
        const Def<Segment2d> seg = dimensions ( poly );
        if ( ! seg.isDef ) return false;
    // Проверка на малость площади по сравнению с габаритами многоугольника
        if ( area ( poly ) < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return false;
    // Преобразование вершин многоугольника к стандартному виду
        const Def<Conform2d> c = standardPosition ( seg );
        if ( ! c.isDef ) return false;
        conf = c;
        DynArray<Vector2d> temp ( poly );
        temp *= Affin2d ( conf );
    // Для каждой стороны вычислим параметры уравнения прямой
        return points2lines ( temp, line );
    }
}

void maxPolygonInConvexPolygon_test()
{
    FixArray<Vector2d, 4> vert1;
    randPolygon ( vert1 );
    Suite<Vector2d> vert2 ( 7, 7 );
    randConvexPolygon ( vert2 );
    drawPolygon ( vert2, 0, 1, 1 );
    const Def<Conform2d> conf = maxPolygonInConvexPolygon ( vert1, vert2 );
    if ( conf.isDef )
    {
        drawPolygon ( vert1 *= conf, 1, 0, 1 );
    }
}

void maxAffinPolygonInConvexPolygon_test()
{
    FixArray<Vector2d, 5> vert1;
    regularPolygon ( vert1 );
    Suite<Vector2d> vert2 ( 7, 7 );
    randConvexPolygon ( vert2 );
    drawPolygon ( vert2, 0, 1, 1 );
    Def<Affin2d> affin = maxAffinPolygonInConvexPolygon ( vert1, vert2 );
    if ( affin.isDef )
    {
        drawPolygon ( vert1 *= affin, 1, 0, 1 );
    }
}

void maxCircle_correct_test()
{
    Suite<Vector2d> p;
    static PRand rand;
    for ( nat n = 3; n <= 1000; ++n )
    {
        p.resize ( n );
        for ( nat i = 0; i < n; ++i )
        {
            p[i].x = rand();
            p[i].y = rand();
//            p[i].x = cos(i*2*M_PI/n) + 1e-3*rand();
//            p[i].y = sin(i*2*M_PI/n) + 1e-3*rand();
        }
//if(j!=6448) continue;
        convexMN ( p );
        const Def<Circle2d> cir1 = maxCircle_NlogN ( p );
        const Def<Circle2d> cir2 = maxCircleInConvexPolygon ( p );
        if ( cir1.isDef && cir2.isDef )
        {
            if ( qmod ( cir1.o - cir2.o ) > 1e-20 || fabs ( cir1.r - cir2.r ) > 1e-14 )
            {
                display << cir1.r << cir2.r << NL;
            }
        }
        else
        {
        }
//        break;
    }
    display << "end" << NL;
}

void maxCircle_time_test()
{
    Suite<Vector2d> p;
    static PRand rand;
    for ( nat j = 2; j <= 500; ++j )
    {
        nat n = 2 * j;
        p.resize ( n );
        for ( nat i = 0; i < n; ++i )
        {
            p[i].x = rand();
            p[i].y = rand();
//            p[i].x = cos(i*2*M_PI/n) + 1e-1*rand();
//            p[i].y = sin(i*2*M_PI/n) + 1e-1*rand();
//            p[i].x /= 1.5;
        }
        convexMN ( p );
        double tt1 = 0, tt2 = 0, tt3 = 0;
        for ( nat l=0; l<20; ++l )
        {
            double t0 = timeInSec();
            const Def<Circle2d> cir1 = maxCircle_NlogN ( p );
            double t1 = timeInSec();
            const Def<Circle2d> cir2 = maxCircle_NlogN ( p );
            double t2 = timeInSec();
            maxCircleInConvexPolygon ( p );
            double t3 = timeInSec();
            tt1 += t1 - t0;
            tt2 += t2 - t1;
            tt3 += t3 - t2;
        }
        display << p.size() << tt1 << tt2 << tt3 << NL;
    }
}

namespace
{

struct Object2d
{
    nat i; // индекс прямой или начальной точки
    nat j; // индекс конечной точки ( v == false ) или предыдущей прямой ( v == true )
    bool v; // если v == false, то это отрезок, а если v == true, то вершина ( невыпуклая )
};

bool check ( const Vector2d * vert, const Line2d * line, 
             const Object2d & oi, const Object2d & oj, const Object2d & ok, 
             const Vector2d & center, const double radius )
{
    const double eps = 1e-12 * radius;
    if ( oi.v )
    {
        const Vector2d v ( center - vert[oi.i] );
        if ( oi.j != ok.i )
        {
            if ( v % line[oi.j].norm > eps ) return false;
        }
        if ( oi.i != oj.i )
        {
            if ( line[oi.i].norm % v > eps ) return false;
        }
    }
    else
    {
        if ( oi.i != ( ok.v ? ok.i : ok.j ) )
        {
            if ( ( center - vert[oi.i] ) % line[oi.i].norm > eps ) return false;
        }
        if ( oi.j != ( oj.v ? oj.i : oj.j ) )
        {
            if ( ( vert[oi.j] - center ) % line[oi.i].norm > eps ) return false;
        }
    }
    if ( oj.v )
    {
        const Vector2d v ( center - vert[oj.i] );
        if ( oj.j != oi.i )
        {
            if ( v % line[oj.j].norm > eps ) return false;
        }
        if ( oj.i != ok.i )
        {
            if ( line[oj.i].norm % v > eps ) return false;
        }
    }
    else
    {
        if ( oj.i != ( oi.v ? oi.i : oi.j ) )
        {
            if ( ( center - vert[oj.i] ) % line[oj.i].norm > eps ) return false;
        }
        if ( oj.j != ( ok.v ? ok.i : ok.j ) )
        {
            if ( ( vert[oj.j] - center ) % line[oj.i].norm > eps ) return false;
        }
    }
    if ( ok.v )
    {
        const Vector2d v ( center - vert[ok.i] );
        if ( ok.j != oj.i )
        {
            if ( v % line[ok.j].norm > eps ) return false;
        }
        if ( ok.i != oi.i )
        {
            if ( line[ok.i].norm % v > eps ) return false;
        }
    }
    else
    {
        if ( ok.i != ( oj.v ? oj.i : oj.j ) )
        {
            if ( ( center - vert[ok.i] ) % line[ok.i].norm > eps ) return false;
        }
        if ( ok.j != ( oi.v ? oi.i : oi.j ) )
        {
            if ( ( vert[ok.j] - center ) % line[ok.i].norm > eps ) return false;
        }
    }
    return true;
}

bool circle ( const Vector2d * vert, const Line2d * line,
              const Object2d & oi, const Object2d & oj, const Object2d & ok, 
              const Vector2d & point, const Vector2d & dir, double & radius, Vector2d & center )
{
    double r[2];
    Vector2d c[2];
    nat nr;
    if ( oi.v )
    {
        if ( oj.v )
        {
            if ( ok.v )
            {
                double r2;
                if ( ! ::circlePPP ( vert[oi.i], vert[oj.i], vert[ok.i], c[0], r2 ) ) return false;
                r[0] = sqrt ( r2 );
                nr = 1;
            }
            else
            {
                nr = circlePPL ( vert[oi.i], vert[oj.i], line[ok.i], c, r );
                if ( nr == 0 ) return false;
            }
        }
        else
        {
            if ( ok.v )
            {
                nr = circlePPL ( vert[oi.i], vert[ok.i], line[oj.i], c, r );
                if ( nr == 0 ) return false;
            }
            else
            {
                nr = circlePLL ( vert[oi.i], line[oj.i], line[ok.i], c, r );
                if ( nr == 0 ) return false;
            }
        }
    }
    else
    {
        if ( oj.v )
        {
            if ( ok.v )
            {
                nr = circlePPL ( vert[oj.i], vert[ok.i], line[oi.i], c, r );
                if ( nr == 0 ) return false;
            }
            else
            {
                nr = circlePLL ( vert[oj.i], line[ok.i], line[oi.i], c, r );
                if ( nr == 0 ) return false;
            }
        }
        else
        {
            if ( ok.v )
            {
                nr = circlePLL ( vert[ok.i], line[oj.i], line[oi.i], c, r );
                if ( nr == 0 ) return false;
            }
            else
            {
                if ( ! circleLLL ( line[oi.i], line[oj.i], line[ok.i], c[0], r[0] ) ) return false;
                nr = 1;
            }
        }
    }
    if ( dir * ( c[0] - point ) > -1e-12 * r[0] && check ( vert, line, oi, oj, ok, c[0], r[0] ) )
    {
        center = c[0];
        radius = r[0];
        if ( nr == 2 )
        {
            if ( dir * ( c[1] - point ) >= -1e-12 * r[1] && 
                 check ( vert, line, oi, oj, ok, c[1], r[1] ) &&  
                 qmod ( c[1] - point ) < qmod ( c[0] - point ) )
            {
                center = c[1];
                radius = r[1];
            }
        }
    }
    else
    {
        if ( nr == 1 || dir * ( c[1] - point ) < -1e-12 * r[1] ) return false;
        if ( ! check ( vert, line, oi, oj, ok, c[1], r[1] )  ) return false;
        center = c[1];
        radius = r[1];
    }
    return true;
}

bool circle ( const Vector2d * vert, const Line2d * line,
              const Object2d & oi, const Object2d & oj, const Object2d & ok, 
              double & radius, Vector2d & center )
{
    if ( ok.v ) return false;
    if ( oi.v )
    {
        if ( oj.v )
        {
            return false;
        }
        else
        {
            const double s = line[oj.i].norm * line[ok.i].norm - 1.;
            if ( s >= 0 ) return false;
            radius = ( line[oj.i] % vert[oi.i] ) / s;
            center = vert[oi.i] - line[ok.i].norm * radius;
        }
    }
    else
    {
        if ( oj.v )
        {
            const double s = line[oi.i].norm * line[ok.i].norm - 1.;
            if ( s >= 0 ) return false;
            radius = ( line[oi.i] % vert[oj.i] ) / s;
            center = vert[oj.i] - line[ok.i].norm * radius;
        }
        else
        {
            return circleLLL ( line[oi.i], line[oj.i], line[ok.i], center, radius );
        }
    }
    return true;
}

bool maxCircleInPolygon1 ( nat no, const Object2d * object, const Vector2d * vert, 
                          const Line2d * line, Vector2d point, Vector2d & max_c, double & max_r )
{
    const nat no1 = no - 1;
    const Object2d & oi = object[no1];
    const Vector2d & normi = line[oi.i].norm;
    nat j = 0;
    for(;;)
    {
        const Object2d & oj = object[j];
        const Vector2d & normj = line[oj.i].norm;
        Vector2d dir; // направление обхода области близости
        if ( oi.v )
        {
            if ( oj.v )
            {
                dir = ( vert[oj.i] - vert[oi.i] ).leftPerpendicular();
                if ( ! dir )
                {
                    return false;
                }
                dir.setNorm2();
            }
            else 
            {
                if ( oi.i == oj.i )
                {
                    dir = - normj;
                }
                else
                {
                    dir = normj.leftPerpendicular();
                }
            }
        }
        else
        {
            if ( oj.v )
            {
                if ( oi.j == oj.i )
                {
                    dir = - normi;
                }
                else
                {
                    dir = normi.rightPerpendicular();
                }
            }
            else
            {
                dir = ( normi * normj > 0 ) ?
                    normi % normj > 0 ? - ( normi + normj ) : normi + normj
                    :
                    ( normi - normj ).rightPerpendicular();
                dir.setNorm2();
            }
        }
        double newRadius;
        Vector2d newCenter;
        double min = 1e300;
        nat km = no;
        for ( nat k = j+1; k < no1; ++k )
        {
            double radius;
            Vector2d center;
            const Object2d & ok = object[k];
            if ( oj.v )
            {
                if ( oj.j == oi.i && oj.i == ok.i ) continue;
            }
            else
            {
                if ( oi.v && ok.v && oi.i == ok.j ) continue;
            }
            if ( oi.v )
            {
                if ( oi.j == ok.i && oi.i == oj.i ) break;
            }
            else
            {
                if ( ok.v && oj.v && ok.i == oj.j ) break;
            }
            if ( ok.v )
            {
                if ( ok.j == oj.i && ok.i == oi.i ) break;
            }
            else
            {
                if ( oj.v && oi.v && oj.i == oi.j ) break;
            }
            if ( ! circle ( vert, line, oi, oj, ok, point, dir, radius, center ) ) continue;
            const double t = qmod ( center - point );
            if ( min > t )
            {
                min = t;
                km = k;
                newCenter = center;
                newRadius = radius;
            }
        }
        if ( km == no ) 
        {
            return false;
        }
        point = newCenter;
        if ( max_r < newRadius )
        {
            max_r = newRadius;
            max_c = newCenter;
        }
        if ( km > j + 1 )
        {
            if ( km == j + 2 ) 
            {
                if ( ! circle ( vert, line, object[km], oj, object[km-1], newRadius, newCenter ) )
                {
                    circle ( vert, line, object[km], oj, object[km-1], newRadius, newCenter );
                    return false;
                }
                if ( max_r < newRadius )
                {
                    max_r = newRadius;
                    max_c = newCenter;
                }
            }
            else
            if ( ! maxCircleInPolygon1 ( km + 1 - j, object + j, vert, line, point, max_c, max_r ) )
                return false;
        }
        if ( km + 1 == no1 ) break;
        if ( km + 2 == no1 ) 
        {
            if ( ! circle ( vert, line, oi, object[km], object[km+1], newRadius, newCenter ) )
                return false;
            if ( max_r < newRadius )
            {
                max_r = newRadius;
                max_c = newCenter;
            }
            break;
        }
        j = km;
    }
    return true;
}

Def<Circle2d> maxCircleInPolygon1 ( CArrRef<Vector2d> poly )
{
    Def<Circle2d> cir;
// Определение габаритов многоугольника
    const Def<Segment2d> seg = dimensions ( poly );
    if ( ! seg.isDef ) return cir;
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( area ( poly ) < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return cir;
// Для каждой стороны вычислим параметры уравнения прямой
    DynArray<Line2d> line ( poly.size() );
    if ( ! points2lines ( poly, line ) ) return cir;
// Заполнение массива объектов ( отрезков и невыпуклых вершин )
    DynArray<Object2d> object ( 2*poly.size() );
    const nat n1 = poly.size() - 1; // nv >= 3
    nat no = 0;
    for ( nat i = 0; i < poly.size(); ++i )
    {
//display << poly[i] << NL;
        const nat j = i < n1 ? i + 1 : 0;
        Object2d & a = object[no++];
        a.i = i;
        a.v = false;
        a.j = j;
        if ( line[i].norm % line[j].norm <= 0 )
        {
            Object2d & b = object[no++];
            b.i = j;
            b.j = i;
            b.v = true;
        }
    }
// Поиск максимальной окружности
    Vector2d max_c;
    double max_r = 0.;
    Vector2d point ( poly[0] ); // начальная точка
    if ( ! maxCircleInPolygon1 ( no, &object[0], &poly[0], &line[0], point, max_c, max_r ) ) return cir;
    cir.o = max_c;
    cir.r = max_r;
    cir.isDef = true;
    return cir;
}

void maxCircleInPolygon_test1()
{
    Suite<Vector2d> vert;
    const char * filename = "data/opti2d_test.pgn";
    if ( ! loadPgn ( filename, vert ) )
    {
        display << "Read error for" << filename << NL;
        return;
    }
    Def<Segment2d> seg = dimensions ( vert );
//    vert -= 0.5 * ( seg.a + seg.b );
    drawPolygon ( vert, 0, 1, 1 );
    drawPoints ( vert, 1, 1, 0 );
    Def<Circle2d> cir2 = maxCircleInPolygon ( vert );
    if ( cir2.isDef )
    {
        draw ( cir2, 1, 1, 0 );
    }
    else
    {
//        cir2.isDef = true;
        draw ( cir2, 1, 1, 0 );
        display << "err2" << NL;
    }
    display << "end" << NL;
}

void maxCircleInPolygon_test2()
{
    const nat n = 9;
    const nat nj = 99000;
    FixArray<Vector2d, n> vert;
    for ( nat nv = 4; nv <= n; ++nv )
    {
        double time1 = 0, time2 = 0;
        for ( nat j = 0; j < nj; ++j )
        {
            randPolygon ( ArrRef<Vector2d> ( vert, 0, nv ) );
            CArrRef<Vector2d> ref ( vert, 0, nv );
            double t1 = timeInSec();
            Def<Circle2d> cir1 = maxCircleInPolygon ( ref );
            time1 += timeInSec() - t1;
            nat k = 0;
            if ( k > 0 )
            {
//                maxCircleInPolygon ( ref );
                for ( k = 0; k < ref.size(); ++k ) display << ref[k] << NL;
                drawPolygon ( ref, 0, 1, 1 );
                draw ( cir1, 1, 0, 0 );
                return;
            }
            if ( ! cir1.isDef )
            {
                maxCircleInPolygon ( ref );
                display << "err j =" << j << " nv =" << nv << NL;
                for ( k = 0; k < ref.size(); ++k ) display << ref[k] << NL;
                drawPolygon ( ref, 0, 1, 1 );
                cir1.isDef = true;
                draw ( cir1, 1, 0, 0 );
                return;
            }
            double t2 = timeInSec();
            Def<Circle2d> cir2 = maxCircleInPolygon1 ( ref );
            time2 += timeInSec() - t2;
            if ( ! cir2.isDef )
            {
                maxCircleInPolygon1 ( ref );
                display << "err2" << NL;
                if(0)
                {
                    draw ( cir2, 1, 0, 0 );
                    return;
                }
                continue;
            }
            const double q1 = qmod ( cir1.o - cir2.o );
            const double q2 = fabs ( cir1.r - cir2.r );
            if ( q1 > 1e-20 || q2 > 1e-10 )
            {
                display << "err j =" << j << " nv =" << nv << "r1 - r2 =" << cir1.r - cir2.r << NL;
 //               if ( cir.r - cir2.r > 0.01 )
                {
                    maxCircleInPolygon1 ( ref );
                    drawPolygon ( ref, 0, 1, 1 );
                    draw ( cir1, 1, 1, 0 );
                    draw ( cir2, 1, 0, 0 );
                    return;
                }
            }
        }
display << nv << time1 << time2 << time1 / time2 << NL;
    }
    display << "end" << NL;
}

void maxCircleInPolygon_test3()
{
    const char * filename = "cont 99.txt";
//    const char * filename = "cont 707.txt";
    RealFile file ( filename, "r" );
    if ( ! file.isValid() )
    {
        display << "Not open file" << filename << NL;
        return;
    }
    nat32 nv;
    Suite<Vector2d> vert;
    while ( readIntDec ( file, nv ) )
    {
        vert.resize ( nv );
        int32 x, y;
        nat32 i;
        for ( i = 0; i < nv; ++i )
        {
            if ( ! readIntDec ( file, x ) || ! readIntDec ( file, y ) ) break;
            vert[i].x = x;
            vert[i].y = y;
        }
        if ( i == nv ) //&& nv < 2000 )
        {
            double t1 = timeInSec();
            Def<Circle2d> cir1 = maxCircleInPolygon ( vert );
            double time1 = timeInSec() - t1;
            if ( ! cir1.isDef )
            {
                display << "err1" << NL;
                if(1)
                {
                    drawPolygon ( vert, 0, 1, 1 );
                }
            }
            double t2 = timeInSec();
            Def<Circle2d> cir2 = maxCircleInPolygon1 ( vert );
            double time2 = timeInSec() - t2;
            if ( ! cir2.isDef )
            {
                display << "err2" << NL;
            }
//display << nv << time1 << time2 << NL;
            const double q2 = fabs ( cir1.r - cir2.r );
            if ( q2 > 1e-12 )
            {
                display << " nv =" << nv << "r1 - r2 =" << cir1.r - cir2.r << NL;
                if ( nv < 30 )
                {
                    for ( nat i = 0; i < nv; ++i ) display << vert[i] << NL;
                    maxCircleInPolygon1 ( vert );
                    Def<Conform2d> conf = standardPosition ( dimensions ( vert ) );
                    if ( conf.isDef )
                    {
                        conf.magn *= 2;
                        conf.trans *= 2;
                        vert *= Affin2d ( conf );
                        cir1 *= conf;
                        cir2 *= conf;
                    }
                    drawPolygon ( vert, 0, 1, 1 );
                    draw ( cir1, 1, 1, 0 );
                    draw ( cir2, 1, 0, 0 );
                    return;
                }
            }
        }
        else
            display << nv << NL;
    }
//    Vector2d dir, o;
//    getLine2 ( CArrRef<Vector2d>(graph,count), dir, o );
//    display << "end" << dir.y/dir.x << (dir%o)/dir.x << NL;
    display << "end" << NL;
}

void maxFixEllipseInConvexPolygon_test()
{
    Suite<Vector2d> vert2 ( 9, 9 );
    randConvexPolygon ( vert2 );
    drawPolygon ( vert2, 0, 1, 1 );
    const double ext = 0.59;
    const Ellipse2d elli = maxFixEllipseInConvexPolygon ( ext, vert2 );
    draw ( elli, 1, 0, 1 );
}

void maxFixEllipseInPolygon_test()
{
    const nat n = 5;
    Suite<Vector2d> vert ( n, n );
    randPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    const double ext = 1/64.;
    const Ellipse2d elli = maxFixEllipseInPolygon ( ext, vert );
    draw ( elli, 1, 0, 1 );
}

void maxConvexPolygonInPolygon_test()
{
    Suite<Vector2d> inner;
    Suite<Vector2d> outer;
    if(0)
    {
        const char * file1 = "data/inner.pgn";
        if ( ! loadPgn ( file1, inner ) )
        {
            display << "Not load" << file1 << NL;
            return;
        }
        const char * file2 = "data/outer.pgn";
        if ( ! loadPgn ( file2, outer ) )
        {
            display << "Not load" << file2 << NL;
            return;
        }
    }
    else
    {
        //for ( nat k = 0; k < 7; ++k )
        {
            inner.resize(4);
            randConvexPolygon ( inner );
        //    regularPolygon ( inner, 1 );
            outer.resize(7);
            randPolygon ( outer );
        }
    }
    drawPolygon ( outer, 0, 1, 1 );
    const Def<Conform2d> conf = maxConvexPolygonInPolygon ( inner, outer );
    if ( conf.isDef )
    {
        drawPolygon ( inner *= conf, 1, 1, 0 );
    }
    else
    {
//        for ( nat i = 0; i < outer.size(); ++i ) display << outer[i] << NL;
//        maxConvexPolygonInPolygon ( inner, outer );
    }
    display << "end" << NL;
}

void maxConvexPolygonInPolygon_test2()
{
    CmbSuite<Vector2d, 20> inner ( 0 );
    CmbSuite<Vector2d,  9> outer ( 0 );
    Suite<Segment2d> ss;
    for ( nat no = 3; no <= 9; ++no )
    {
        outer.resize(no);
        for ( nat i = 0; i < 300; ++i )
        {
            randPolygon ( outer );
            for ( nat ni = 3; ni <= 20; ++ni )
            {
                inner.resize(ni);
                regularPolygon ( inner );
                const Def<Conform2d> conf = maxConvexPolygonInPolygonNR ( inner, outer );
                if ( conf.isDef )
                {
                    inner *= conf;
                    for ( nat j = 0; j < ni; ++j )
                    {
                        const Segment2d seg ( inner[j], inner.cnext(j) );
                        intersection ( seg, outer, ss );
                        double sum = 0;
                        for ( nat k = 0; k < ss.size(); ++k ) sum += norm2 ( ss[k] );
                        if ( sum < 0.999 * norm2 ( seg ) )
                            display << "err2" << sum / norm2 ( seg ) << NL;
//                        drawPolygon ( outer, 0, 1, 1 );
//                        drawPolygon ( inner, 1, 1, 0 );
//                        maxConvexPolygonInPolygon ( inner, outer );
                    }
                }
                else
                    display << "err" << no << i << ni << NL;
            }
        }
    }
    display << "end" << NL;
}

void maxConvexPolygonInPolygon_test3()
{
    FixArray<Vector2d, 30> inner;
    CmbSuite<Vector2d,  9> outer ( 0 );
    double time = 0;
    for ( nat no = 3; no <= 9; ++no )
    {
        outer.resize(no);
        for ( nat i = 0; i < 10000; ++i )
        {
            randPolygon ( outer );
            regularPolygon ( inner );
            double t1 = timeInSec();
            const Def<Conform2d> conf = maxConvexPolygonInPolygonNR ( inner, outer );
            double t2 = timeInSec();
            time += t2 - t1;
            if ( conf.isDef )
            {
                Def<Circle2d> cir = maxCircleInPolygon ( outer );
                if ( conf.isDef )
                {
                    if ( fabs ( conf.magn - cir.r ) > 4e-3 )
                    {
                        display << fabs ( conf.magn - cir.r ) << conf.magn << cir.r << no << NL;
                        inner *= conf;
                        drawPolygon ( outer, 0, 1, 1 );
                        drawPolygon ( inner, 1, 1, 0 );
                        draw ( cir, 1, 0, 1 );
                        for ( nat k = 0; k < outer.size(); ++k ) display << outer[k] << NL;
                        return;
                    }
                }
                else
                    display << "err2" << no << i << NL;
            }
            else
                display << "err" << no << i << NL;
        }
    }
    display << "end" << time << NL;
}

void maxEllipseInPolygon_test()
{
    const nat n = 5;
    Suite<Vector2d> vert ( n, n );
    randPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    const Ellipse2d elli = maxEllipseInPolygonA ( vert );
    draw ( elli, 1, 0, 1 );
}

void minEllipseAroundPoints_test()
{
    FixArray<Vector2d, 5> vert2;
    static PRandPoint2d rand;
    vert2 << rand;
//    vert2[0] = Vector2d ( -1, 0.5 );
//    vert2[1] = Vector2d (  1, 0.5 );
    drawPoints ( vert2, 0, 1, 1 );
    const double ext = 0.5;
    const Ellipse2d elli = minEllipseAroundPoints ( ext, vert2 );
    draw ( elli, 1, 0, 1 );
}

Def<Ellipse2d> minEllipseAroundPoints ( CArrRef<Vector2d> points, bool (*func) ( const WireModel<5> &, Double<5> & ) )
{
    Def<Ellipse2d> res;
// Преобразование множества точек к стандартному виду
    DynArray<Vector2d> temp ( points );
    Def<Conform2d> conf = setStandardPosition ( temp );
    if ( ! conf.isDef ) return res;
// Инициализация области допустимых преобразований
    List< Vertex<5> > stor;
    WireModel<5> model;
    model.simplex ( 10000., stor );
    Double<5> d5;
    d5.d2 = d5.d3 = d5.d4 = 2000.;
    d5.d1 = d5.d0 = 0.;
    model.vlist -= d5;
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Double<5> best;
        if ( ! func ( model, best ) ) return res;
        const double f = best.d0;
        const double h = best.d1;
        const double g = best.d2;
        const Vector2d v ( best.d3, best.d4 );
// Поиск максимального нарушения ограничений для выбранного решения
        nat k, km;
        double max = 0.;
        for ( k = 0; k < temp.size(); ++k )
        {
            const Vector2d & p = temp[k];
            const Vector2d p1 ( p.x * f - p.y * g + v.x, p.y * h + v.y );
            const double t = p1 * p1;
            if ( max < t ) max = t, km = k;
        }
// Если нарушение мало, то завершение программы
        if ( max < 0.250001 )
        {
            const double fi = 0.5 * atan2 ( g * h, 0.5 * ( h * h - f * f - g * g ) );
            const double a = cos ( fi );
            const double b = sin ( fi );
            const double a11 = a * f;
            const double a12 = b * h - a * g;
            const double t = b * f;
            const double s = b * g + a * h;
            const double ext = fabs ( a12 ) > fabs ( a11 ) ? t / a12 : s / a11;
            const Vector2d u ( ( a * v.x + b * v.y ) * ext, a * v.y - b * v.x );
            conf = ~conf * ~Conform2d ( Spin2d ( atan2 ( -t, s ) ), u + u, 2 * sqrt ( s * s + t * t ) ) *
                   Conform2d ( Spin2d::d090, null2d, ext );
            res.a = conf.magn / ext;
            res.b = conf.magn;
            res.o = conf.trans;
            res.spin = conf.spin;
            res.isDef = true;
            return res;
        }
// Применение ограничения к области допустимых преобразований
        const Vector2d & p = temp[km];
        Vector2d norm ( p.x * f - p.y * g + v.x, p.y * h + v.y );
        norm.setNorm2();
        Double<6> plane;
        plane.d0 = norm.x * p.x;
        plane.d1 = norm.y * p.y;
        plane.d2 = - norm.x * p.y;
        plane.d3 = norm.x;
        plane.d4 = norm.y;
        plane.d5 = -0.5;
        model.cut ( plane, stor );
    }
    return res;
}

bool _area ( const WireModel<5> & model, Double<5> & best )
{
// Поиск максимального решения на вершинах и рёбрах
    Show< Vertex<5> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<5> * p = show.cur();
        const Double<5> & pc = p->coor;
        const double t = pc.d0 * pc.d1;
        if ( max < t ) max = t, best = pc;
        for ( nat k = 0; k < 5; ++k )
        {
            const Vertex<5> * v = p->vertex[k];
            if ( v < p ) continue;
            const Double<5> & vc = v->coor;
            const double d0 = pc.d0 - vc.d0;
            const double d1 = pc.d1 - vc.d1;
            const double y0 = vc.d1 * d0 + vc.d0 * d1;
            const double y1 = pc.d1 * d0 + pc.d0 * d1;
            if ( y0 * y1 >= 0 ) continue;
            const double x0 = y0 / ( y0 - y1 );
            const double x1 = 1. - x0;
            const double u = ( pc.d0 * x0 + vc.d0 * x1 ) * ( pc.d1 * x0 + vc.d1 * x1 );
            if ( max >= u ) continue;
            max = u;
            best = pc;
            best -= vc;
            best *= x0;
            best += vc;
        }
    }
    while ( show.next() );
    return max > 0;
}

Def<Ellipse2d> minEllipseAroundPointsA1 ( CArrRef<Vector2d> points )
{
    return minEllipseAroundPoints ( points, _area );
}

void minEllipseAroundPointsAP_test()
{
    FixArray<Vector2d, 9> point;
    static PRandVector2d rand;
    for ( nat i = 0; i <= 7; ++i )
    point << rand;
        point[0] = point[8];
        point[1] = point[7];
        point[2] = point[6];
        point[0].x += 1e-11;
        point[1].x += 1e-11;
        point[2].x += 1e-11;
    drawPoints ( point, 0, 1, 1 );
    Def<Ellipse2d> e1 = minEllipseAroundPointsA1 ( point );
    draw ( e1, 1, 0, 1 );
    Def<Ellipse2d> e2 = minEllipseAroundPointsA ( point );
    draw ( e2, 1, 1, 0 );
    if ( e2.isDef )
    {
        const double da = fabs ( e1.a - e2.a );
        const double db = fabs ( e1.b - e2.b );
        const double dc = norm2 ( e1.o - e2.o);
        const double e = 0.001;
        if ( da > e || db > e || dc > e )
        display << da << db << dc << NL;
    }
    else
        display << "err" << NL;
}

void minEllipseAroundPointsAP_test2()
{
    FixArray<Vector2d, 9> point;
    static QRandPoint2d rand;
    for ( nat i = 0; i < 2000; ++i )
    {
        point << rand;
        double t1 = timeInSec();
        point[0] = point[8];
        point[1] = point[7];
        point[2] = point[6];
        point[0].x += 1e-11;
        point[1].x += 1e-11;
        point[2].x += 1e-11;
        Def<Ellipse2d> e1 = minEllipseAroundPointsA1 ( point );
        Def<Ellipse2d> e2 = minEllipseAroundPointsA ( point );
        if ( e2.isDef )
        {
            const double da = fabs ( e1.a - e2.a );
            const double db = fabs ( e1.b - e2.b );
            const double dc = norm2 ( e1.o - e2.o);
            const double e = 0.001;
            if ( da > e || db > e || dc > e )
            display << i << da << db << dc << NL;
        }
        else
            display << i << "err" << NL;
    }
    display << "end" << NL;
}

void minEllipseAroundPointsAP_test3()
{
    const nat N = 500;
    CmbArray<Vector2d, N> point;
    static QRandPoint2d rand;
    for ( nat n = 3; n <= N; ++n )
    {
        point.resize ( n );
        double t = 0;
        for ( nat i = 0; i < 100; ++i )
        {
            point << rand;
            double t1 = timeInSec();
            minEllipseAroundPointsA ( point );
            t += timeInSec() - t1;
        }
        display << n << t << NL;
    }
}

Def<Ellipse2d> maxEllipseInConvexPolygon ( CArrRef<Vector2d> poly, bool (*func) ( const WireModel<5> &, Double<5> & ) )
{
    Def<Ellipse2d> res;
// Получение границ многоугольника в виде прямых линий
    Conform2d conf;
    DynArray<Line2d> line ( poly.size() );
    if ( ! points2lines ( poly, conf, line ) ) return res;
// Инициализация области допустимых преобразований
    List< Vertex<5> > stor;
    WireModel<5> model;
    model.simplex ( 10., stor );
    Double<5> d5;
    d5.d2 = d5.d3 = d5.d4 = 2.;
    d5.d1 = d5.d0 = 0.;
    model.vlist -= d5;
    const double eps = 1e-5;
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Double<5> best;
        if ( ! func ( model, best ) ) return res;
        const double f = best.d0;
        const double h = best.d1;
        const double g = best.d2;
        const Vector2d v ( best.d3, best.d4 );
// Поиск максимального нарушения ограничений для выбранного решения
        Vector2d pm;
        nat k, km;
        double max = 0.;
        for ( k = 0; k < line.size(); ++k )
        {
            const Line2d & lk = line[k];
            const Vector2d & norm = lk.norm;
            Vector2d p ( norm.x * f, norm.y * h - norm.x * g );
            p.setNorm2(); // праобраз самой дальней точки в направлении norm
            const double t = norm.x * ( p.x * f - p.y * g ) + norm.y * p.y * h + lk % v;
            if ( max < t ) max = t, pm = p, km = k;
        }
// Если нарушение мало, то завершение программы
        if ( max < eps )
        {
            const double fg = f * f + g * g;
            const double hh = h * h;
            const double fh = f * h;
            if ( fabs ( fh ) < 1e-12 )
            {
                conf = ~conf * Conform2d ( Spin2d ( atan2 ( g, h ) ), v, sqrt ( fg + hh ) );
                res.a = 0.;
                res.b = conf.magn;
                res.o = conf.trans;
                res.spin = conf.spin;
                return res;
            }
            const double b = 0.5 * ( fg + hh ) / fh;
            const double d = b * b - 1.;
            if ( d < 1e-12 )
            {
                conf = ~conf * Conform2d ( Spin2d (), v, sqrt ( fh ) );
                res.a = res.b = conf.magn;
                res.o = conf.trans;
                res.spin = conf.spin;
            }
            else
            {
                const double ext = b - sqrt ( d );
                const double e = 1 - ext * ext;
                double s, t;
                if ( h > f )
                {
                    t = sqrt ( ( hh - ext * fh ) / e );
                    s = h * g / ( e * t );
                }
                else
                {
                    s = sqrt ( ( fh / ext - hh ) / e );
                    t = h * g / ( e * s );
                }
                conf = ~conf * Conform2d ( Spin2d ( atan2 ( s, t ) ), v, sqrt ( fh / ext ) );
                res.a = ext * conf.magn;
                res.b = conf.magn;
                res.o = conf.trans;
                res.spin = conf.spin;
            }
            res.isDef = true;
            return res;
        }
// Применение ограничения к области допустимых преобразований
        const double nx = line[km].norm.x;
        const double ny = line[km].norm.y;
        Double<6> plane;
        plane.d0 = nx * pm.x;
        plane.d1 = ny * pm.y;
        plane.d2 = - nx * pm.y;
        plane.d3 = nx;
        plane.d4 = ny;
        plane.d5 = line[km].dist;
        model.cut ( plane, stor );
    }
    return res;
}

Def<Ellipse2d> maxEllipseInConvexPolygonA1 ( CArrRef<Vector2d> poly )
{
    return maxEllipseInConvexPolygon ( poly, _area );
}

static bool perimeter ( const WireModel<5> & model, Double<5> & best )
{
// Поиск максимального решения на вершинах
    Show< Vertex<5> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Double<5> & c = show.cur()->coor;
        const double t = c.d0 * c.d0 + c.d1 * c.d1 + c.d2 * c.d2 + 0.5 * c.d0 * c.d1;
        if ( max < t ) max = t, best = c;
    }
    while ( show.next() );
    return max > 0;
}

Def<Ellipse2d> maxEllipseInConvexPolygonP ( CArrRef<Vector2d> poly )
{
    return maxEllipseInConvexPolygon ( poly, perimeter );
}

void maxEllipseInConvexPolygon_test()
{
    Suite<Vector2d> vert2 ( 0, 3 );
//    for ( nat i = 0; i < 2; ++i )
//    do vert2.resize(6); while ( randConvexPolygon ( vert2 ).size() < 5 );
    randConvexPolygon ( vert2 );
    drawPolygon ( vert2, 0, 1, 1 );
    double pa = 0;
    const Def<Ellipse2d> ea = maxEllipseInConvexPolygonA ( vert2 );
    if ( ea.isDef )
    {
        draw ( ea, 1, 1, 0 );
    }
    else
        display << "err A" << NL;
    const Def<Ellipse2d> ep = maxEllipseInConvexPolygonA1 ( vert2 );
    if ( ep.isDef )
    {
//        draw ( ep, 1, 0, 0 );
    }
    else
        display << "err A1" << NL;
}

void maxEllipseInConvexPolygon_test2()
{
    Suite<Vector2d> vert2(0, 999);
    for ( nat n = 3; n <= 999; ++n )
    {
        vert2.resize(n);
        randConvexPolygon ( vert2 );
        double t0 = timeInSec();
        const Def<Ellipse2d> e1 = maxEllipseInConvexPolygonA ( vert2 );
        if ( ! e1.isDef )
        {
            maxEllipseInConvexPolygonA ( vert2 );
            display << "err 1" << NL;
        }
        double t1 = timeInSec();
        const Def<Ellipse2d> e2 = maxEllipseInConvexPolygonA1 ( vert2 );
        if ( ! e2.isDef )
        {
            display << "err 2" << NL;
        }
        double t2 = timeInSec();
//        display << vert2.size() << t1 - t0 << t2 - t1 << NL;
        if ( e1.isDef && e2.isDef )
        {
            double a = fabs ( e1.a - e2.a );
            double b = fabs ( e1.b - e2.b );
            double e = 2.8e-3;
            if ( a > e || b > e )
            {
                display << n << a << b << NL;
                drawPolygon ( vert2, 0, 1, 1 );
                draw ( e1, 1, 1, 0 );
                draw ( e2, 1, 0, 0 );
                return;
            }
        }
    }
    display << "end" << NL;
}

void maxRectangleInConvexPolygon_test()
{
    Suite<Vector2d> poly ( 0, 9 );
//    for(nat i=0;i<3;++i) 
    randConvexPolygon ( poly );
/*    static PRand rand;
    double a = 0.5*rand()+0.1;
    double b = 0.5*rand()+0.1;
    poly[0] = Vector2d ( a, b );
    poly[1] = Vector2d (-a, b );
    poly[2] = Vector2d (-a,-b );
    poly[3] = Vector2d ( a,-b );
    for012 ( poly ) *= Spin2d ( rand() );
    static PRandPoint2d prand;
    for012 ( poly ) += 0.1*prand();*/
    drawPolygon ( poly, 0, 1, 1 );
    Def<Rectangle2d> rect = maxRectangleInConvexPolygonA ( poly );
    if ( rect.isDef )
    {
        display << rect.a * rect.b;
        draw ( rect, 1, 0, 1 );
    }
    rect = maxRectangleInPolygonA ( poly );
    if ( rect.isDef )
    {
        display << rect.a * rect.b << NL;
        draw ( rect, 1, 1, 0 );
    }
}

void maxRectangleInPolygon_test()
{
    Suite<Vector2d> poly ( 0, 5 );
    randPolygon ( poly );
    drawPolygon ( poly, 0, 1, 1 );
    Def<Rectangle2d> rect = maxRectangleInPolygonA ( poly );
    if ( rect.isDef )
    {
        draw ( rect, 1, 0, 1 );
    }
}

void maxRectangleInConvexPolygon_test2()
{
    FixArray<Vector2d, 4> rect;
    Suite<Vector2d> poly ( 0, 9 );
    for ( nat i = 0; i < 300; ++i ) 
    {
        poly.resize ( 4 );
        randConvexPolygon ( poly );
        if ( i != 290 ) continue;
        Def<Rectangle2d> rect1 = maxRectangleInConvexPolygonA ( poly );
        if ( ! rect1.isDef )
        {
            display << i << "err" << NL;
            continue;
        }
        const Def<Conform2d> conf = maxPolygonInConvexPolygon ( rect, poly );
        if ( fabs ( conf.magn - 1 ) > 1e-2 )
            display << i << conf.magn - 1 << NL;
        if ( fabs ( conf.magn - 1 ) > 0.067 )
        {
            drawPolygon ( poly, 0, 1, 1 );
            drawPolygon ( rect, 1, 0, 1 );
            drawPolygon ( rect *= conf, 1, 1, 0 );
            break;
        }
    }
    display << "end" << NL;
}

void maxRhombusInConvexPolygon_test()
{
    Suite<Vector2d> poly ( 0, 9 );
    randConvexPolygon ( poly );
    drawPolygon ( poly, 0, 1, 1 );
    Def<Rhombus2d> fig = maxRhombusInConvexPolygonA ( poly );
    if ( fig.isDef )
    {
        draw ( fig, 1, 0, 1 );
    }
    fig = maxRhombusInConvexPolygonP ( poly );
    if ( fig.isDef )
    {
        draw ( fig, 1, 1, 0 );
    }
}

void maxRhombusInConvexPolygon_test2()
{
    FixArray<Vector2d, 4> rect;
    Suite<Vector2d> poly ( 0, 9 );
    for ( nat i = 0; i < 3000; ++i ) 
    {
        poly.resize ( 7 );
        randConvexPolygon ( poly );
//        if ( i != 290 ) continue;
        Def<Rhombus2d> fig = maxRhombusInConvexPolygonP ( poly );
        if ( ! fig.isDef )
        {
            display << i << "err" << NL;
            continue;
        }
        makePolygon ( fig, rect );
        const Def<Conform2d> conf = maxPolygonInConvexPolygon ( rect, poly );
        if ( fabs ( conf.magn - 1 ) > 1e-6 )
            display << i << conf.magn - 1 << NL;
        if ( fabs ( conf.magn - 1 ) > 0.06 )
        {
            drawPolygon ( poly, 0, 1, 1 );
            drawPolygon ( rect, 1, 0, 1 );
            drawPolygon ( rect *= conf, 1, 1, 0 );
            break;
        }
    }
    display << "end" << NL;
}

void maxTriangleInConvexPolygon_test()
{
    Suite<Vector2d> poly ( 0, 9 );
    randConvexPolygon ( poly );
    drawPolygon ( poly, 0, 1, 1 );
    draw ( maxTriangleInConvexPolygonA ( poly ), 1, 0, 1 );
}

void maxQuadInConvexPolygon_test()
{
    FixArray<Vector2d, 4> quad;
    Suite<Vector2d> poly ( 0, 9 );
    randConvexPolygon ( poly );
    drawPolygon ( poly, 0, 1, 1 );
    if ( maxQuadInConvexPolygonA ( quad, poly ) )
    {
        drawPolygon ( quad, 1, 0, 1 );
    }
}

void minRectangleAroundPoints_test1 ()
{
    Suite<Vector2d> points ( 0, 9 );
    static PRandPoint2d rand;
    points << rand;
    drawPoints ( points, 0, 1, 1 );
    double a1 = 0, a2 = 0, p1 = 0, p2 = 0;
    const Rectangle2d rectA = minRectangleAroundPointsA ( points );
    draw ( rectA, 1, 0, 0 );
    a1 = rectA.a * rectA.b;
    p1 = rectA.a + rectA.b;
    const Rectangle2d rectP = minRectangleAroundPointsP ( points );
    draw ( rectP, 1, 1, 0 );
    a2 = rectP.a * rectP.b;
    p2 = rectP.a + rectP.b;
    if ( a1 > a2 + 1e-15 ) 
        display << "a1 - a1 =" << a1 - a2 << NL;
    if ( p1 < p2 - 1e-15 ) 
        display << "p2 - p1 =" << p2 - p1 << NL;
}

void minRectangleAroundPoints_test2 ()
{
    Suite<Vector2d> points ( 0, 33 );
    static PRandPoint2d rand;
    for ( nat i = 0; i < 9000; ++i )
    {
        points << rand;
        double a1 = 0, a2 = 0, p1 = 0, p2 = 0;
        const Rectangle2d rectA = minRectangleAroundPointsA ( points );
        a1 = rectA.a * rectA.b;
        p1 = rectA.a + rectA.b;
        const Rectangle2d rectP = minRectangleAroundPointsP ( points );
        a2 = rectP.a * rectP.b;
        p2 = rectP.a + rectP.b;
        if ( a1 > a2 + 1e-14 ) 
            display << "a1 - a1 =" << a1 - a2 << NL;
        if ( p1 < p2 - 1e-15 ) 
            display << "p2 - p1 =" << p2 - p1 << NL;
    }
    display << "end" << NL;
}

void minRectangleAroundPolygon_test1 ()
{
    FixArray<Vector2d, 4> poly;
    Suite<Vector2d> points ( 0, 9 );
    randPolygon ( points );
    drawPolygon ( points, 0, 1, 1 );
    double a1 = 0, a2 = 0, p1 = 0, p2 = 0;
    const Rectangle2d rectA = minRectangleAroundPolygonA ( points );
    draw ( rectA, 1, 0, 0 );
    a1 = rectA.a * rectA.b;
    p1 = rectA.a + rectA.b;
    const Rectangle2d rectP = minRectangleAroundPolygonP ( points );
    draw ( rectP, 1, 1, 0 );
    a2 = rectP.a * rectP.b;
    p2 = rectP.a + rectP.b;
    if ( a1 > a2 + 1e-15 ) 
        display << "a1 - a1 =" << a1 - a2 << NL;
    if ( p1 < p2 - 1e-15 ) 
        display << "p2 - p1 =" << p2 - p1 << NL;
}

bool maxPointsInConvexPolygon ( CArrRef<Vector2d> poly, Triangle2d & res, bool (*func) ( const WireModel<6> &, Double<6> & ) )
{
// Получение границ многоугольника в виде прямых линий
    Conform2d conf;
    DynArray<Line2d> line ( poly.size() );
    if ( ! points2lines ( poly, conf, line ) ) return false;
// Инициализация области допустимых преобразований
    List< Vertex<6> > stor;
    WireModel<6> model;
    model.simplex ( 2*(6+1), stor );
    Double<6> dn;
    dn.fill ( 1 );
    dn.d2 = dn.d5 = 0;
    model.vlist -= dn;
// Поиск оптимального значения
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Double<6> best;
        if ( ! func ( model, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
        nat km, pm;
        double max = 0.;
        const Vector2d p1 ( best.d0 + best.d2, best.d1 + best.d3 );
        const Vector2d p2 ( best.d0 + best.d4, best.d1 + best.d5 );
        const Vector2d p3 ( best.d0 - best.d2, best.d1 - best.d3 );
        const Vector2d p4 ( best.d0 - best.d4, best.d1 - best.d5 );
        for ( nat k = 0; k < line.size(); ++k )
        {
            const double t1 = line[k] % p1;
            if ( max < t1 ) max = t1, pm = 1, km = k;
            const double t2 = line[k] % p2;
            if ( max < t2 ) max = t2, pm = 2, km = k;
            const double t3 = line[k] % p3;
            if ( max < t3 ) max = t3, pm = 3, km = k;
            const double t4 = line[k] % p4;
            if ( max < t4 ) max = t4, pm = 4, km = k;
        }
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            conf = ~conf;
            res.a = Vector2d ( conf.magn * best.d0, conf.magn * best.d1 ) + conf.trans; 
            res.b = Vector2d ( conf.magn * best.d2, conf.magn * best.d3 );
            res.c = Vector2d ( conf.magn * best.d4, conf.magn * best.d5 );
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const double nx = line[km].norm.x;
        const double ny = line[km].norm.y;
        Double<7> plane;
        plane.d0 = nx;
        plane.d1 = ny;
        switch ( pm )
        {
        case 1:
            plane.d2 = nx;
            plane.d3 = ny;
            plane.d4 = 
            plane.d5 = 0;
            break;
        case 2:
            plane.d2 = 
            plane.d3 = 0;
            plane.d4 = nx;
            plane.d5 = ny;
            break;
        case 3:
            plane.d2 = -nx;
            plane.d3 = -ny;
            plane.d4 = 
            plane.d5 = 0;
            break;
        case 4:
            plane.d2 = 
            plane.d3 = 0;
            plane.d4 = -nx;
            plane.d5 = -ny;
            break;
        }
        plane.d6 = line[km].dist;
        model.cut ( plane, stor );
    }
    return false;
}

bool area2 ( const WireModel<6> & model, Double<6> & best )
{
// Поиск максимального решения на вершинах и рёбрах
    Show< Vertex<6> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Double<6> & pc = show.cur()->coor;
        const double t = pc.d2 * pc.d5 - pc.d4 * pc.d3;
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

//Def<Triangle2d> maxParallelogramInConvexPolygon ( CArrRef<Vector2d> poly )
Def<Parallelogram2d> maxParallelogramInConvexPolygon ( CArrRef<Vector2d> poly )
{
    Triangle2d triangle;
    if ( ! maxPointsInConvexPolygon ( poly, triangle, area2 ) ) return Def<Parallelogram2d>();
    return Parallelogram2d ( triangle.a + triangle.b, triangle.a + triangle.c, triangle.a - triangle.b );
}

void maxParallelogramInConvexPolygon_test()
{
    Suite<Vector2d> poly ( 0, 9 );
    for ( nat i = 0; i < 1; ++i )
    {
        poly.resize(9);
        randConvexPolygon ( poly );
//        Def<Parallelogram2d> fig1 = maxParallelogramInConvexPolygon  ( poly );
        Def<Parallelogram2d> fig2 = maxParallelogramInConvexPolygonA ( poly );
        //Def<Triangle2d> fig1 = maxParallelogramInConvexPolygon  ( poly );
        //Def<Triangle2d> fig2 = maxTriangleInConvexPolygonA ( poly );
//        double area1 = fig1.isDef ? fig1.getArea() : 0;
//        double area2 = fig2.isDef ? fig2.getArea() : 0;
 //       display << i << area1 << area2 << NL;
        drawPolygon ( poly, 0, 1, 1 );
        draw ( fig2, 1, 1, 0 );
    }
}

void minTriangleAroundConvexPolygon_test1 ()
{
    static PRandPoint2d rand;
    FixArray<Vector2d, 6> point;
    for ( nat i = 0; i < 1; ++i )
        point << rand;
    draw ( minTriangleAroundPointsA ( point ), 0, 1, 1 );
    drawPoints ( point, 1, 0, 0 );
//    display << point.size() << NL;
}

void minTriangleAroundConvexPolygon_test2 ()
{
    static PRandPoint2d rand;
    FixArray<Vector2d, 9> point;
    Suite<Vector2d> poly;
    for ( nat i = 0; i < 1000; ++i )
    {
        point << rand;
        convexNlogN ( point, poly );
        Triangle2d trian1 = minTriangleAroundConvexPolygonA ( poly );
        Triangle2d trian2 = minTriangleAroundPointsA ( point );
        double a1 = trian1.getArea();
        double a2 = trian2.getArea();
        if ( fabs ( a1 - a2 ) > 1e-9 )
        display << i << a1 - a2 << NL;
    }
    display << "end" << NL;
}

void minParallelogramAroundPolygon_test1 ()
{
    FixArray<Vector2d, 6> point;
    randPolygon ( point );
    drawPolygon ( point, 0, 1, 1 );
    draw ( minParallelogramAroundPolygonA ( point ), 1, 0, 0 );
}

void minRhombAroundPolygon_test1 ()
{
    FixArray<Vector2d, 6> point;
    randPolygon ( point );
    drawPoints ( point, 0, 1, 1 );
    Def<Rhombus2d> rhomb = minRhombusAroundPointsP ( point );
    if ( rhomb.isDef )
    {
        draw ( rhomb, 1, 1, 0 );
    }
    else display << "err" << NL;
    rhomb = minRhombusAroundPointsA ( point );
    if ( rhomb.isDef )
    {
        draw ( rhomb, 1, 0, 0 );
    }
    else display << "err" << NL;
}

Circle2d minCircle ( CArrRef<Vector2d> points )
{
    const nat n = points.size();
    if ( n == 0 ) return Circle2d ();
    if ( n == 1 ) return Circle2d ( points[0], 0 );
    if ( n == 2 )
    {
        const Vector2d o = 0.5 * ( points[0] + points[1] );
        return Circle2d ( o, norm2 ( points[0] - o ) );
    }
    DynArray<const Vector2d *> p ( n );
    for ( nat i = 0; i < n; ++i ) p[i] = & points[i];
    shuffle ( p, PRand() );
    Vector2d o = 0.5 * ( *p[0] + *p[1] );
    double q = qmod ( *p[0] - o );
    for ( nat i1 = 2; i1 < n; ++i1 )
    {
        const Vector2d & v1 = *p[i1];
        if ( qmod ( v1 - o ) <= q ) continue;
        o = 0.5 * ( v1 + *p[0] );
        q = qmod ( v1 - o );
        for ( nat i2 = 1; i2 < i1; ++i2 )
        {
            const Vector2d & v2 = *p[i2];
            if ( qmod ( v2 - o ) <= q ) continue;
            o = 0.5 * ( v1 + v2 );
            q = qmod ( v1 - o );
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Vector2d & v3 = *p[i3];
                if ( qmod ( v3 - o ) <= q ) continue;
                circlePPP ( v1, v2, v3, o, q );
            }
        }
    }
    return Circle2d ( o, sqrt ( q ) );
}

void minCircleAroundPoints_test()
{
    FixArray<Vector2d, 29> buf;
    static PRandPoint2d rand;
    nat n = 4;
    for ( n = 4; n <= buf.size(); n += 1 )
    {
        double s1 = 0, s2 = 0;
        ArrRef<Vector2d> data ( buf, 0, n );
        for ( nat i = 0; i < 1000; ++i )
        {
            data << rand;
            double t1 = timeInSec();
            const Circle2d cir1 = minCircleAroundPoints ( data );
            double t2 = timeInSec();
            const Circle2d cir2 = minCircle ( data );
            double t3 = timeInSec();
            s1 += t2 - t1;
            s2 += t3 - t2;
            if(n==0)
            {
                draw ( cir1, 1, 0, 0 );
                draw ( cir2, 1, 1, 0 );
                drawPoints ( data, 0, 1, 1 );
            }
            const double dr = fabs(cir1.r-cir2.r);
            const double dc = qmod(cir1.o-cir2.o);
            if ( dr > 2e-15 || dc > 5e-30 )
                display << n << i << dr << dc << NL;
        }
        display << n << s1 << s2 << NL;
    }
    display << "end" << NL;
}

void minCircleAroundCircles_test()
{
    FixArray<Circle2d, 40> buf;
    static PRandPoint2d rand;
    static PRand color;
    nat j, n = 9;
//    for ( n = 4; n <= buf.size(); n += 1 )
    {
        ArrRef<Circle2d> data ( buf, 0, n );
//        for ( nat i = 0; i < 1000; ++i )
        {
            for ( j = 0; j < data.size(); ++j )
            {
                data[j].o = 0.25*rand();
                data[j].r = 0.35*fabs ( rand().x );
            }
            for ( j = 0; j < data.size(); ++j )
            {
                const float r = (float) color();
                const float g = (float) color();
                const float b = (float) color();
                draw ( data[j], r, g, b );
            }
            const Def<Circle2d> cir1 = minCircleAroundCircles ( data );
            if ( cir1.isDef == false )
    display << "err" << NL;
            else
            {
                draw ( cir1, 1, 1, 0 );
            }
        }
    }
    display << "end" << NL;
}

void minCircleAroundCircles_test2()
{
    FixArray<Circle2d, 40> buf;
    FixArray<Vector2d, 40> buf2;
    static PRandPoint2d rand;
    nat j, n = 9;
    for ( n = 4; n <= buf.size(); n += 1 )
    {
        ArrRef<Circle2d> data ( buf, 0, n );
        for ( nat i = 0; i < 1000; ++i )
        {
            for ( j = 0; j < data.size(); ++j )
            {
                data[j].o = buf2[j] = 0.25*rand();
                data[j].r = 0.25;
            }
            const Def<Circle2d> cir1 = minCircleAroundCircles ( data );
            if ( cir1.isDef == false )
    display << "err" << NL;
            else
            {
                const Circle2d cir2 = minCircle ( ArrRef<Vector2d> ( buf2, 0, n ) );
                const double dr = fabs(cir1.r-cir2.r-0.25);
                const double dc = qmod(cir1.o-cir2.o);
                if ( dr > 2e-15 || dc > 5e-30 )
                    display << n << i << dr << dc << NL;
            }
        }
    }
    display << "end" << NL;
}

Def<Vector2d> pointInPolygon ( CArrRef<Vector2d> vert )
{
    Def<Vector2d> res;
    const nat nv = vert.size();
    if ( nv < 3 ) return res;
    DynArray<Line2d> line ( nv );
// Для каждой стороны вычислим параметры уравнения прямой
    nat i;
    for ( i = 0; i < nv; ++i )
    {
        const Vector2d & v2 = vert.cnext(i);
        const Vector2d & v1 = vert[i];
        Line2d & linei = line[i];
        Vector2d & norm = linei.norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return res;
        norm /= sqrt ( q );
        const double d1 = norm * v1;
        const double d2 = norm * v2;
        linei.dist = d1 > d2 ? - d1 : - d2;
    }
// Заполнение массива объектов ( отрезков и невыпуклых вершин )
    Suite<Set2<nat, bool> > object ( nv+nv );
    nat im;
    double max = 0;
    for ( i = 0; i < nv; ++i )
    {
        if ( line.cprev(i).norm % line[i].norm <= 0 )
        {
            Set2<nat, bool> & b = object.inc();
            b.a = i;
            b.b = true; // невыпуклая вершина 
        }
        // Находим ребро максимальной длины
        const double t = qmod ( vert[i] - vert.cnext(i) );
        if ( max < t ) max = t, im = object.size();
        Set2<nat, bool> & a = object.inc();
        a.a = i;
        a.b = false; // отрезок
    }
    if ( ! max ) return res;
    const Line2d & line1 = line[im];
    nat i1 = im + 1;
    if ( i1 >= object.size() ) i1 -= object.size();
// Находим вписанные окружности касающиеся ребра максимальной длины
    Suite<Set2<Circle2d, nat> > circle;
    for ( i = 2; i < object.size(); ++i )
    {
        nat j = im + i;
        if ( j >= object.size() ) j -= object.size();
        if ( object[j].b )
        {
            double r[2];
            Vector2d o[2];
            const Vector2d & v2 = vert[object[j].a];
            while ( circle.size() > 0 )
            {
                const Circle2d & cir = circle.las().a;
                if ( qmod ( v2 - cir.o ) < cir.r * cir.r ) break;
                circle.dec();
            }
            const nat pre = circle.size() > 0 ? circle.las().b : i1;
            if ( object[pre].b )
            {
                const nat n = circlePPL ( v2, vert[object[pre].a], line1, o, r );
                if ( n == 0 ) return res;
            }
            else
            {
                const nat n = circlePLL ( v2, line[object[pre].a], line1, o, r );
                if ( n == 0 ) return res;
            }
        }
        else
        {
            const Line2d & line2 = line[object[j].a];
            while ( circle.size() > 0 )
            {
                const Circle2d & cir = circle.las().a;
                if ( line2 % cir.o + cir.r < 0 ) break;
                circle.dec();
            }
            const nat pre = circle.size() > 0 ? circle.las().b : i1;
            if ( object[pre].b )
            {
                double r[2];
                Vector2d o[2];
                const nat n = circlePLL ( vert[object[pre].a], line1, line2, o, r );
                if ( n == 0 ) return res;
                Set2<Circle2d, nat> & cir = circle.inc();
                if ( n == 1 )
                {
                    cir.a.r = r[0]; 
                    cir.a.o = o[0];
                    cir.b = j;
                }
            }
            else
            {
                double r;
                Vector2d o;
                const Line2d & line3 = line[pre];
                if ( ! circleLLL ( line1, line2, line3, o, r ) ) return res;
                Set2<Circle2d, nat> & cir = circle.inc();
                cir.a.r = r; 
                cir.a.o = o;
                cir.b = j;
            }
        }
    }
// Выбираем центр вписанной окружности максимального радиуса
    max = 0;
    for ( i = 0; i < circle.size(); ++i )
    {
        const Circle2d & c = circle[i].a;
        if ( max < c.r ) max = c.r, res = c.o;
    }
    return res;
}

void pointInPolygon_test()
{
    Suite<Vector2d> vert ( 7, 7 );
//    for ( nat i = 0; i < 5; ++i ) 
        randConvexPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    Def<Vector2d> res = pointInPolygon ( vert );
    if ( res.isDef )
    {
        drawPoint ( res, 1, 1, 0 );
    }
}

void minTrapezoidAroundPoints_test1 ()
{
    FixArray<Vector2d, 4> para, trap;
    Suite<Vector2d> vert ( 7, 7 );
    randConvexPolygon ( vert );
    drawPoints ( vert, 0, 1, 1 );
    if ( ! minTrapezoidAroundConvexPolygonA ( trap, vert ) )
    {
        display << "error 1" << NL;
        return;
    }
    drawPolygon ( trap, 1, 1, 0 );
 /*   double a1 = area ( trap );
    if ( ! minParallelogramAroundConvexPolygonA ( para, vert ) )
    {
        display << "error 2" << NL;
        return;
    }
    drawPolygon ( para, 1, 0, 0 );
    double a2 = area ( para );
    display << a2 << a1 << a2 - a1 << NL;*/
}
/*
void minTrapezoidAroundPoints_test2 ()
{
    FixArray<Vector2d, 4> para, trap;
    Suite<Vector2d> vert;
    for ( nat n = 4; n <= 100; n += 1 )
    {
        vert.resize ( n );
        randConvexPolygon ( vert );
        minTrapezoidAroundConvexPolygonA ( trap, vert );
        minParallelogramAroundConvexPolygonA ( para, vert );
        double a1 = area ( trap );
        double a2 = area ( para );
        if ( a1 > a2 )
        display << n << a1 - a2 << NL;
    }
    display << "end" << NL;
}
*/
void minNgonAroundPoints_test1 ()
{
    FixArray<Vector2d, 4> para, trap;
    Suite<Vector2d> vert ( 0, 66 );
    randConvexPolygon ( vert );
    drawPoints ( vert, 0, 1, 1 );
    if ( ! minTrapezoidAroundConvexPolygonA ( trap, vert ) )
    {
        display << "error 1" << NL;
        return;
    }
    drawPolygon ( trap, 1, 1, 0 );
    double a1 = area ( trap );
    if ( ! minNgonAroundConvexPolygonA ( para, vert ) )
    {
        display << "error 2" << NL;
        return;
    }
    drawPolygon ( para, 1, 0, 0 );
    double a2 = area ( para );
    display << a2 << a1 << a1 - a2 << NL;
}

void minNgonAroundPoints_test2 ()
{
    FixArray<Vector2d, 4> quad, poly;
    Suite<Vector2d> vert;
    for ( nat n = 5; n <= 500; n += 1 )
    {
        vert.resize ( n );
        randConvexPolygon ( vert );
        minTrapezoidAroundConvexPolygonA ( quad, vert );
        minNgonAroundConvexPolygonA ( poly, vert );
        double a1 = area ( quad );
        double a2 = area ( poly );
        if ( a1 < a2 )
        {
            display << n << a1 - a2 << vert.size() << NL;
            //maxNgonInConvexPolygonA ( poly, vert );
            drawPoints ( vert, 0, 1, 1 );
            drawPolygon ( vert, 0, 1, 1 );
            drawPolygon ( quad, 1, 1, 0 );
            drawPolygon ( poly, 1, 0, 0 );
            break;
        }
    }
    display << "end" << NL;
}

void maxNgonInConvexPolygon_test1 ()
{
    FixArray<Vector2d, 5> quad, poly;
    Suite<Vector2d> vert ( 0, 99 );
    randConvexPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
/*    if ( ! maxQuadInConvexPolygonA ( quad, vert ) )
    {
        display << "error 1" << NL;
        return;
    }
    drawPolygon ( quad, 1, 1, 0 );
    double a1 = area ( quad );*/
    if ( ! maxNgonInConvexPolygonA ( poly, vert ) )
    {
        display << "error 2" << NL;
        return;
    }
    drawPolygon ( poly, 1, 0, 0 );
//    double a2 = area ( poly );
//    display << a2 << a1 << a2 - a1 << NL;
}

void maxNgonInConvexPolygon_test2 ()
{
    FixArray<Vector2d, 4> quad, poly;
    Suite<Vector2d> vert;
    for ( nat n = 5; n <= 5000; n += 1 )
    {
        vert.resize ( n );
        randConvexPolygon ( vert );
        maxQuadInConvexPolygonA ( quad, vert );
        maxNgonInConvexPolygonA ( poly, vert );
        double a1 = area ( quad );
        double a2 = area ( poly );
        if ( a1 > a2 )
        {
            display << n << a1 - a2 << vert.size() << NL;
            maxNgonInConvexPolygonA ( poly, vert );
            drawPoints ( vert, 0, 1, 1 );
            drawPolygon ( vert, 0, 1, 1 );
            drawPolygon ( quad, 1, 1, 0 );
            drawPolygon ( poly, 1, 0, 0 );
            break;
        }
    }
    display << "end" << NL;
}

void minEquianglarPolygonAroundPolygon_test ()
{
    FixArray<Vector2d, 3> outer;
    Suite<Vector2d> inner ( 5, 5 );
    static PRandPoint2d rand;
    inner << rand;
//    randConvexPolygon ( inner );
//    randPolygon ( inner );
    drawPoints ( inner, 0, 1, 1 );
//    if ( minEquianglarPolygonAroundConvexPolygonA ( outer, inner ) )
//    if ( minEquianglarPolygonAroundPolygonA ( outer, inner ) )
    if ( minEquianglarPolygonAroundPointsA ( outer, inner ) )
    {
        drawPolygon ( outer, 1, 1, 0 );
    }
    else display << "err" << NL;
}

class Bound
{
    mutable DynArray<Plane3d> plane;
public:
    bool isLine;
    Line2d line;
    DynArray<Vector2d> poly;

    double distance ( const Vector2d & v ) const
    {
        if ( isLine ) return line % v;
        nat index;
        bool isBone;
        double d;
        ::distance ( poly, v, d, index, isBone );
        return d;
    }

    CArrRef<Plane3d> getPlane () const
    {
        if ( ! plane.size() && poly.size() > 2 )
        {
            nat i;
            plane.resize ( 2 * poly.size() );
            for ( i = 0; i < poly.size(); ++i )
            {
                const Line2d temp ( poly[i], poly.cnext(i) );
                Plane3d & p = plane[i];
                p.norm.x = temp.norm.x;
                p.norm.y = temp.norm.y;
                p.norm.z = 1;
                p.dist = temp.dist;
            }
            for ( i = 0; i < poly.size(); ++i )
            {
                const Plane3d & p2 = plane[i>0?i-1:poly.size()-1];
                const Plane3d & p1 = plane[i];
                Plane3d & p3 = plane[i + poly.size()];
                p3.norm = Vector3d ( p1.norm.x, p1.norm.y, -1 ) % Vector3d ( p2.norm.x, p2.norm.y, -1 );
                if ( p3.norm.z < 0 )
                p3 = -p3;
                p3.dist = - ( p3.norm.x * poly[i].x + p3.norm.y * poly[i].y );
            }
        }
        return plane;
    }
};

inline
Vector3d getDir ( const Vector2d & a, const Vector2d & b, const Vector2d & c )
{
    const Vector3d v = Vector3d ( a.x, a.y, 1 ) % Vector3d ( b.x, b.y, 1 );
    return v * Vector3d ( c.x, c.y, 1 ) > 0 ? -v : v;
}

struct Solution
{
    Circle2d cir;
    Vector3d va, vb, vc;
    const Bound * a, * b, * c;

    bool init ( const Circle2d & p0, const Vector3d & v, const Bound * p1, const Bound * p2, const Bound * p3 )
    {
        a = p1;
        b = p2;
        c = p3;
        if ( ! a->isLine && b->isLine ) _swap ( a, b );
        if ( a->isLine )
        {
            if ( b->isLine )
            {
                if ( c->isLine )
                {
                    cir = circleLLL ( a->line, b->line, c->line );
                }
                else
                {
                    CArrRef<Plane3d> plane = c->getPlane();
                    const Vector3d & o = ( const Vector3d & ) p0;
                    for ( nat i = 0; i < plane.size(); ++i )
                    {
                        const Plane3d & pi = plane[i];
                        const double s = pi.norm * v;
                        if ( s <= 0 ) continue;
                        double t = ( pi % o ) / s;
                        i = i;
                    }
                }
            }
            return false;
        }
        else
        {
            return false;
        }
    }
};

Def<Circle2d> maxCircleInPolygon2 ( CArrRef<Vector2d> poly )
{
    nat i;
    Def<Circle2d> cir;
// Определение габаритов многоугольника
    const Def<Segment2d> seg = dimensions ( poly );
    if ( ! seg.isDef ) return cir;
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( area ( poly ) < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return cir;
// Определение вершин выпуклой оболочки
    Suite<nat> convexIndex;
    convexPolygon ( poly, convexIndex );
    if ( poly.size() == convexIndex.size() ) return maxCircleInConvexPolygon ( poly );
// Сделаем минимальный индекс первым в массиве
    convexIndex <<= firMin ( convexIndex );
// Для каждой стороны выпуклой оболочки построим прямую и запишим их в ограничения
    List< ListItem<Bound> > bound;
    DynArray<Line2d> line ( convexIndex.size() );
    DynArray<const Bound *> boundPtr ( convexIndex.size() );
    for ( i = 0; i < convexIndex.size(); ++i )
    {
        const Vector2d & v2 = poly[convexIndex.cnext(i)];
        const Vector2d & v1 = poly[convexIndex[i]];
        Vector2d & norm = line[i].norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return cir;
        norm /= sqrt ( q );
        line[i].dist = - ( norm * v1 );
        ListItem<Bound> * b = new ListItem<Bound>;
        b->line = line[i];
        b->isLine = true;
        bound.addAftLas ( b );
        boundPtr[i] = b;
    }
// Впишем окружность в выпуклую оболочку и запишем её в список решений
    List< ListItem<Solution> > solution;
    ListItem<Solution> * p = new ListItem<Solution>;
    nat i1, i2, i3;
    p->cir = maxCircleInConvexPolygon ( line, i1, i2, i3 );
    p->a = boundPtr[i1];
    p->b = boundPtr[i2];
    p->c = boundPtr[i3];
    p->va = getDir ( line[i2].norm, line[i3].norm, line[i1].norm );
    p->vb = getDir ( line[i3].norm, line[i1].norm, line[i2].norm );
    p->vc = getDir ( line[i1].norm, line[i2].norm, line[i3].norm );
    solution.addAftLas ( p );
// Добавим невыпуклые ограничения в список ограничений
    {
        const nat a = convexIndex.las();
        const nat n = convexIndex[0] + poly.size() - a + 1;
        if ( n > 2 )
        {
            ListItem<Bound> * b = new ListItem<Bound>;
            b->poly.resize ( n );
            for ( nat j = 0; j < n; ++j ) b->poly[j] = poly.cnext ( a, j );
            b->isLine = false;
            bound.addAftLas ( b );
        }
    }
    for ( i = 1; i < convexIndex.size(); ++i )
    {
        const nat a = convexIndex[i-1];
        const nat n = convexIndex[i] - a + 1;
        if ( n > 2 )
        {
            ListItem<Bound> * b = new ListItem<Bound>;
            b->poly.resize ( n );
            for ( nat j = 0; j < n; ++j ) b->poly[j] = poly[a+j];
            b->isLine = false;
            bound.addAftLas ( b );
        }
    }
    //List< ListItem<ShevList> > part;
    //convexParts ( poly, part );
display << bound.size() << NL;
    {
        ListItem<Solution> * p = solution.fir();
        double max = -1e300;
        const ListItem<Bound> * bm = 0;
        bound.top();
        do
        {
            const ListItem<Bound> * b = bound.cur();
            double t = b->distance ( p->cir.o );
            if ( max < t ) max = t, bm = b;
        }
        while ( bound.next() );
        max += p->cir.r;
        if ( max < 1e-3 ) return p->cir;
        ListItem<Solution> * s = new ListItem<Solution>;
        s->init ( p->cir, p->va, p->b, p->c, bm );
        s->init ( p->cir, p->vb, p->c, p->a, bm );
        s->init ( p->cir, p->vc, p->a, p->b, bm );
        solution.addAftLas ( s );
    }
    //cir.isDef = true;
    return cir;
}

void maxCircleInPolygon_test4()
{
    /*Suite<Vector2d> vert;
    const char * filename = "data/opti2d_test.pgn";
    if ( ! loadPgn ( filename, vert ) )
    {
        display << "Read error for" << filename << NL;
        return;
    }
    Def<Segment2d> seg = dimensions ( vert );
//    vert -= 0.5 * ( seg.a + seg.b );*/
    FixArray<Vector2d, 4> vert;
    randPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    drawPoints ( vert, 1, 1, 0 );
    Def<Circle2d> cir = maxCircleInPolygon2 ( vert );
    if ( cir.isDef )
    {
        draw ( cir, 1, 1, 0 );
    }
    else
    {
/*        cir2.isDef = true;
        draw ( cir2, 1, 1, 0 );
        display << "err2" << NL;*/
    }
//    display << "end" << NL;
}

template <nat N> struct LPCone
{
    Double<N> arr[N+1];
};

Def<Rectangle2d> maxRectangleInConvexPolygonANR2 ( const Spin2d & spin, CCArrRef<Vector2d> & outer )
{
    Def<Rectangle2d> res;
    const nat nv = outer.size();
    if ( nv < 3 ) return res;
// Получение границ многоугольника в виде прямых линий
    DynArray<Line2d> line ( nv );
    if ( ! points2lines ( outer, line ) ) return res;
    nat i, j, l;
// Получение габаритов многоугольника
    Vector2d ax, ay;
    (~spin).getReper ( ax, ay );
    double x1, x2, y1, y2;
    x1 = x2 = outer[0] * ax;
    y1 = y2 = outer[0] * ay;
    for ( j = 1; j < nv; ++j )
    {
        const double x = outer[j] * ax;
        const double y = outer[j] * ay;
        if ( x1 > x ) x1 = x; else
        if ( x2 < x ) x2 = x;
        if ( y1 > y ) y1 = y; else
        if ( y2 < y ) y2 = y;
    }
    const double dx = x2 - x1;
    const double dy = y2 - y1;
// Конус допустимых значений
    const nat N = 4;
    const nat NT = 7;
    LPCone<N> cone[NT];
    {
        Double<N> * con = cone[0].arr;
        Double<N> & dn = con[0];
        dn.init ( dx, dy, x2, y2 );
        for ( l = 1; l <= N; ++l )
        {
            con[l].fill(0);
            (&con[l].d0)[l-1] = -1;
        }
    }
// Вычисление ограничений
    DynArray<Double<N+1> > cor ( nv + 4 );
    for ( l = 0; l < nv; ++l )
    {
        const Line2d & p = line[l];
        Double<N+1> & c = cor[l];
        c.d0 = fabs ( p.norm * ax );
        c.d1 = fabs ( p.norm * ay );
        c.d2 = p.norm.x;
        c.d3 = p.norm.y;
        c.d4 = p.dist;
    }
    {
        const Double<N> & dn = cone[0].arr[0];
        cor[ nv ].init ( 1, 0, 0, 0, -dn.d0 );
        cor[nv+1].init ( 0, 1, 0, 0, -dn.d1 );
        cor[nv+2].init ( 0, 0, 1, 0, -dn.d2 );
        cor[nv+3].init ( 0, 0, 0, 1, -dn.d3 );
    }
    MaxHeap<SortItem<double,nat> > heap ( NT );
    const double q = dx * dy;
    for ( l = 0; l < NT; ++l )
    {
        heap << SortItem<double,nat> ( q, l );
    }
    const double eps = 1e-6 * ( dx + dy );
    for ( nat k = 0; k < 1000; ++k )
    {
        Double<N> * con = cone[heap[0]->tail].arr;
        Double<N> & dn = con[0];
        nat im = 0;
        double max = cor[0] % dn;
        for ( i = 1; i < nv; ++i )
        {
            const double t = cor[i] % dn;
            if ( max < t ) max = t, im = i;
        }
        if ( max < eps )
        {
            res.a = dn.d0;
            res.b = dn.d1;
            res.o.x = dn.d2;
            res.o.y = dn.d3;
            res.isDef = true;
            break;
        }
        const double dist = max;
        const Double<N> & norm = ( const Double<N> & ) cor[im];
        const double lvl = -1e-8 * sqrt ( norm * norm );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= N; ++i )
        {
            const Double<N> & v = con[i];
            double t = norm * v;
            if ( t > lvl ) continue;
            t = -1./ t;
            const double c = dist * t;
            const double x = dn.d0 + c * v.d0; 
            const double y = dn.d1 + c * v.d1; 
            if ( !ib )
            {
                max = x * y;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = x * y;
                if ( max < s ) max = s, ib = i, sm = t;
            }
        }
        if ( !ib )
        {
            return res;
        }
        const Double<N> & v = con[ib];
        dn += v * ( dist * sm );
        for ( i = 1; i <= N; ++i )
        {
            if ( i == ib ) continue;
            Double<N> & ai = con[i];
            ai += v * ( ( norm * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return res;
}

void maxRectangleInConvexPolygonANR_test()
{
    Suite<Vector2d> vert ( 7, 7 );
    randConvexPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    Def<Rectangle2d> fig = maxRectangleInConvexPolygonANR ( vert );
 //   Def<Rhombus2d> fig = maxRhombusInConvexPolygonANR ( vert );
    if ( fig.isDef )
    {
        draw ( fig, 1, 0, 0 );
    }
    else
    {
        display << "err" << NL;
    }
    Def<Rectangle2d> fig2 = maxRectangleInConvexPolygonANR2 ( Spin2d(), vert );
    if ( fig2.isDef )
    {
        draw ( fig2, 1, 1, 0 );
    }
    else
    {
        display << "err" << NL;
    }
    display << "end" << NL;
}

} // end of namespace

void opti2d_test ()
{
    drawNewList2d ();
//    minRectangleAroundPoints_test1();
//    minRectangleAroundPolygon_test1();
//    minParallelogramAroundPolygon_test1();
//    minParallelogramAroundPoints_test3();
//    minRhombAroundPolygon_test1();
//    minTriangleAroundPoints_test3();
//    maxQuadInConvexPolygon_test();
//    maxParallelogramInConvexPolygon_test();
//    maxTriangleInConvexPolygon_test();
//    maxRhombusInConvexPolygon_test();
//    maxRectangleInConvexPolygon_test();
//    maxRectangleInPolygon_test();
//    minEllipseAroundPointsAP_test3();
//    maxEllipseInConvexPolygon_test2();
//    maxFixEllipseInConvexPolygon_test();
//    maxFixEllipseInPolygon_test();
//    maxEllipseInPolygon_test();
//    maxCircle_correct_test();
//    maxCircle_time_test();
//    maxCircleInPolygon_test4();
    maxAffinPolygonInConvexPolygon_test();
//    maxConvexPolygonInPolygon_test3();
//    minConvexPolygonDiameter_test2();
//    minCircleAroundPoints_test();
//    minCircleAroundCircles_test();
//    pointInPolygon_test();
//    minTrapezoidAroundPoints_test2();
//    minNgonAroundPoints_test1 ();
//    maxNgonInConvexPolygon_test1 ();
//    minEquianglarPolygonAroundPolygon_test ();
//    maxRectangleInConvexPolygonANR_test();
    endNewList ();
}
