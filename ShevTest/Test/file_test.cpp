
#include "math.h"
#include "stdio.h"
#include "float.h"

#include "../draw.h"

#include "../Shev/rand.h"
#include "../Shev/PseudoFile.h"
#include "../Shev/StringFile.h"
#include "../Shev/ShevArray.h"

#include "../Shev/tune.h"
#include "../Shev/mathem.h"
#include "../Shev/LinAlg.h"
#include "../Shev/moment2d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/ShevList.h"
#include "../Shev/RealFile.h"

#include "display.h"

//#define sprintf sprintf_s

namespace {

void file_test1 ()
{
    StringWriteFile file;
    static PRand rand;
    for ( int i=0; i < 40000; ++i )
    {
        double a = 2. * rand() - 1;
        if ( !a ) continue;
        a = rand() / a;
//if(i!=233) continue;
        if ( fabs ( a ) < 300 ) a *= exp ( a );
        //a = ldexp ( 1, 63 );
        //if ( fabs ( a ) > 1 ) a = 1 / a;
        file.clear();
        if ( printf ( file, "%.19f ", a ) )
        {
            double b;
            //file.rewind();
            //if ( readFltDec ( file, b ) )
            {
                b = atof ( file() );
                char buf[310];
                sprintf ( buf, "%.19f", a );
                double c = atof ( buf );
                if ( b != c )
                {
                    double e1 = b - c;
                    double e2 = a - c;
                    double e3 = b - a;
//                    if ( fabs(e1/a) > 1e-15 && fabs(e2) > fabs(e3) )
//                    if ( fabs(e1) > 1.1e-9 && fabs(e1/a) > 2e-15 && fabs(e2) < fabs(e3) )
                    if ( fabs(e1/a) > 1e-15 && fabs(e2) < fabs(e3))
                    {
//                    display << i << NL << file() << NL << buf << NL;
                        display << i << fabs(e1/a) << NL;
//                        file.rewind();
                        //printf ( file, "%.9f", a );
                    }
                }
            }
/*            else
            {
                display << "err" << NL;
            }*/
        }
        else
        {
            display << "err" << NL;
        }
    }//*/
    display << "end" << NL;
}

void writeFltDec_test()
{
    Suite<bit8> arr;
//    static QRand rand ( 7 );
    PseudoFile file ( 350, arr );
    double d = 1e70;//DBL_MAX;//100*rand();
    //d -= floor(d);
/*    if ( writeFltDec ( file, d, 0 ) && file.putc ( "" ) )
    {
        display << d << NL;
        display << (char*) &arr[0] << NL;
        display << file.tell() << NL;
    }
    else
        display << "err" << NL;*/
    //for ( nat i = 0; i <= 18; ++i )
    {
        file.rewind();
        printf ( file, "%f", d );
        file.putc ( "" );
        display << d << (char*) arr() << NL;
        d *= 10;
    }
}

void printf_test()
{
    char buf[32];
    StringWriteFile file;
    static PRand rand;
    double d = 100.*rand();
    /*int i = rand.number(333);
    nat n = i;
    i = -i;
    printf ( file, "n = %d ha-ha %u %s %.3f", i, n, "hi-hi", d );
    display << file() << NL;
    file.clear();
    printf ( file, "e = %.3e E = %.3E", d, d );
    display << file() << NL;*
    printf ( file, "%5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%5d", -123 );
    display << file() << NL;
    sprintf ( buf, "%5d", -123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%+5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%+5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%05d", 123 );
    display << file() << NL;
    sprintf ( buf, "%05d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%-5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%-5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%5u", 123 );
    display << file() << NL;
    sprintf ( buf, "%5u", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%7.3f", d );
    display << file() << NL;
    sprintf ( buf, "%7.3f", d );
    display << buf << NL;
    file.clear();
    printf ( file, "% 4.3f", d );
    display << file() << NL;
    sprintf ( buf, "% 4.3f", d );
    display << buf << NL;
    file.clear();*
    printf ( file, "%7.3e", d );
    display << file() << NL;
    sprintf ( buf, "%7.3e", d );
    display << buf << NL;
    file.clear();*/
    printf ( file, "ha-ha % s ha-ha ", "hi-hi" );
    display << file() << NL;
    sprintf ( buf, "ha-ha % s ha-ha ", "hi-hi" );
    display << buf << NL;
    file.clear();
    printf ( file, "% 1d", 123 );
    display << file() << NL;
    sprintf ( buf, "% 1d", 123 );
    display << buf << NL;
    file.clear();
}

struct JNode
{
    Suite<bit8> key, value;
    List<ListItem<JNode> > list;
};

struct Frame
{
    Suite<Vector3d> point;
    double angle1, angle2;
};

bool copyJSON ( List<ListItem<JNode> > & list, Suite<Frame> & frame )
{
    frame.resize ( list.size() );
    list.top();
    for ( nat i = 0; i < list.size(); ++i )
    {
        Frame & fi = frame[i];
        fi.angle1 = fi.angle2 = 0;
        ListItem<JNode> * p = list.cur();
        List<ListItem<JNode> > & point = p->list.top()->list;
        fi.point.resize ( point.size() );
if(i==351)
i=i;
        point.top();
        for ( nat j = 0; j < point.size(); ++j )
        {
            Vector3d & v = fi.point[j];
            ListItem<JNode> * c = point.cur();
            List<ListItem<JNode> > & coor = c->list;
            if ( coor.size() == 3 )
            {
                const JNode & x = * coor.top();
                if ( ! readFltDec ( PseudoReadSeekFile ( x.value ), v.x ) ) return false;
                const JNode & y = * coor.next();
                if ( ! readFltDec ( PseudoReadSeekFile ( y.value ), v.y ) ) return false;
                const JNode & z = * coor.next();
                if ( ! readFltDec ( PseudoReadSeekFile ( z.value ), v.z ) ) return false;
            }
            else
                return false;
            point.next();
        }
        for(;;)
        {
            ListItem<JNode> * item = p->list.next();
            if ( ! item ) break;
            JNode * node = item;
            if ( node->key == CArrRef<bit8> ( (const bit8 *) "Angle_1", 7 ) )
            {
                if ( ! readFltDec ( PseudoReadSeekFile ( node->value ), fi.angle1 ) ) return false;
                continue;
            }
            if ( node->key == CArrRef<bit8> ( (const bit8 *) "Angle_2", 7 ) )
            {
                if ( ! readFltDec ( PseudoReadSeekFile ( node->value ), fi.angle2 ) ) return false;
                continue;
            }
        }
        list.next();
    }
    return true;
}

bool loadStr ( IReadFile & file, Suite<bit8> & str )
{
    char c;
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case '"':
            return true;
        default:
            str.inc() = c;
        }
    }
    return false;
}

bool loadString ( IReadFile & file, Suite<bit8> & str )
{
    char c;
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case '"':
            return loadStr ( file, str );
        }
    }
    return false;
}

bool loadValue ( IReadFile & file, JNode & json, char & c );

bool loadArray ( IReadFile & file, JNode & json, char & c )
{
    for(;;)
    {
        ListItem<JNode> * p = new ListItem<JNode>;
        json.list.addAftLas ( p );
        if ( ! loadValue ( file, *p, c ) ) return false;
        switch ( c )
        {
        case ']':
            return true;
        case ',':
            continue;
        default:
            return false;
        }
    }
    return false;
}

bool loadPair ( IReadFile & file, JNode & json, char & c )
{
    if ( ! loadString ( file, json.key ) ) return false;
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case ' ':
            break;
        case ':':
            return loadValue ( file, json, c );
        default:
            return false;
        }
    }
    return false;
}

bool loadPairs ( IReadFile & file, JNode & json, char & c )
{
    for(;;)
    {
        ListItem<JNode> * p = new ListItem<JNode>;
        json.list.addAftLas ( p );
        if ( ! loadPair ( file, *p, c ) ) return false;
        switch ( c )
        {
        case '}':
            return true;
        case ',':
            continue;
        default:
            return false;
        }
    }
    return false;
}

bool loadValue ( IReadFile & file, JNode & json, char & c )
{
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '+':
        case '-':
        case '.':
        case 'e':
        case 'E':
            json.value.inc() = c;
            break;
        case ',':
        case '}':
        case ']':
            return true;
        case '{':
            if ( ! loadPairs ( file, json, c ) ) return false;
            break;
        case '[':
            if ( ! loadArray ( file, json, c ) ) return false;
            break;
        }
    }
    return false;
}

bool loadJSON ( IReadFile & file, JNode & json)
{
    char c;
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case '{':
            return loadPairs ( file, json, c );
        case '[':
            return loadArray ( file, json, c );
        }
    }
    return true;
}

struct Quater
{
    double la, mu, nu, ro;

    double norm ()
    {
        double t = ro*ro + la*la + mu*mu + nu*nu;
        if ( t )
        {
            double r = sqrt ( t );
            ro /= r;
            la /= r;
            mu /= r;
            nu /= r;
        }
        return t;
    }

    Quater & init ( const Vector3d & r1, const Vector3d & r2 )
    {
        double b = 0.5 * ( r1.x + r2.y );
        double c = _pow2 ( 0.25 * ( r2.x - r1.y ) );
        double d = sqrt ( b * b + 4 * c );
        nu = sqrt ( 0.5 * ( d - b ) );
        ro = sqrt ( 0.5 * ( d + b ) );
        if ( r2.x - r1.y < 0 ) nu = -nu;
        b = 0.5 * ( r1.x - r2.y );
        c = _pow2 ( 0.25 * ( r2.x + r1.y ) );
        d = sqrt ( b * b + 4 * c );
        mu = sqrt ( 0.5 * ( d - b ) );
        la = sqrt ( 0.5 * ( d + b ) );
        return *this;
    }

    void getMatr ( Matrix3<double> & r ) const
    {
        const double xx = la * la;
        const double yy = mu * mu;
        const double zz = nu * nu;
        const double tt = ro * ro;
        const double xy = la * mu;
        const double yz = mu * nu;
        const double zx = nu * la;
        const double xt = la * ro;
        const double yt = mu * ro;
        const double zt = nu * ro;
        r.aa = tt + xx - yy - zz; r.ab = 2. * ( xy - zt );  r.ac = 2. * ( zx + yt );
        r.ba = 2. * ( xy + zt );  r.bb = tt + yy - zz - xx; r.bc = 2. * ( yz - xt );
        r.ca = 2. * ( zx - yt );  r.cb = 2. * ( yz + xt );  r.cc = tt + zz - xx - yy;
    }
};

Quater operator * ( const Quater & l, const Quater & r )
{
    Quater p;
    p.ro = l.ro * r.ro - l.la * r.la - l.mu * r.mu - l.nu * r.nu;
    p.la = l.ro * r.la + l.la * r.ro + l.mu * r.nu - l.nu * r.mu;
    p.mu = l.ro * r.mu + l.mu * r.ro + l.nu * r.la - l.la * r.nu;
    p.nu = l.ro * r.nu + l.nu * r.ro + l.la * r.mu - l.mu * r.la;
    return p;
}

void calc1 ( CCArrRef<Vector3d> & point1, CCArrRef<Vector3d> & point2, Vector3d & r1, Vector3d & r2 )
{
    nat i;
    DynArray2<double> data ( 21, 4 );
    DynArray<double> x ( 3 );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        ArrRef<double> r = data[i];
        r[0] = v.x;
        r[1] = v.y;
        r[2] = 1;
        r[3] = point2[i].x;
    }
    minNorm2 ( data, x );
    r1 = Vector3d ( x[0], x[1], x[2] );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        ArrRef<double> r = data[i];
        r[0] = v.x;
        r[1] = v.y;
        r[2] = 1;
        r[3] = point2[i].y;
    }
    minNorm2 ( data, x );
    r2 = Vector3d ( x[0], x[1], x[2] );
}

double calcDif ( CCArrRef<Vector3d> & point1, CCArrRef<Vector3d> & point2 )
{
    double sum = 0;
    for ( nat i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        double t1 = v.x - point2[i].x;
        double t2 = v.y - point2[i].y;
        sum += sqrt ( t1*t1 + t2*t2 );
//display << i << t1 << t2 << NL;
    }
    return sum/point1.size();
}

inline Vector3d operator * ( const Matrix3<double> & m, const Vector3d & v )
{
    return Vector3d ( m.aa * v.x + m.ab * v.y + m.ac * v.z,
                      m.ba * v.x + m.bb * v.y + m.bc * v.z,
                      m.ca * v.x + m.cb * v.y + m.cc * v.z );
}

void calc ( CCArrRef<Vector3d> & point1, CCArrRef<Vector3d> & point2, double & u, Matrix3<double> & m )
{
    nat i;
    Vector3d r1, r2;
    calc1 ( point1, point2, r1, r2 );
//display << r1 << NL << r2 << NL << NL;
    Quater q;
    q.init ( r1, r2 );
    q.getMatr ( m );
    DynArray<Vector3d> point3 ( point1.size() );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        point3[i] = m * Vector3d ( v.x, v.y, 0 );
    }
    //display << calcDif ( point2, point3 );
    double sum = 0;
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        double t1 = point2[i].x - ( m.aa * v.x + m.ab * v.y );
        double t2 = point2[i].y - ( m.ba * v.x + m.bb * v.y );
        double z = ( m.ac * t1 + m.bc * t2 ) / ( m.ac * m.ac + m.bc * m.bc );
        point3[i] = m * Vector3d ( v.x, v.y, z );
    }
    //display << calcDif ( point2, point3 ) << NL;
    SLU4<double> slu;
    slu.fill ( 0, 0 );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point3[i];
        const Vector3d & u = point2[i];
        slu.aa += v.x*v.x + v.y*v.y;
        slu.ac += v.x*v.z;
        slu.ad -= v.y*v.z;
        slu.cc += v.z*v.z;
        slu.ae += u.x*v.x + u.y*v.y;
        slu.be -= u.x*v.y - u.y*v.x;
        slu.ce += u.x*v.z;
        slu.de -= u.y*v.z;
    }
    slu.bb = slu.aa;
    slu.bc = slu.ad;
    slu.bd = -slu.ac;
    slu.ca = slu.ac;
    slu.cb = slu.bc;
    slu.da = slu.ad;
    slu.db = slu.bd;
    slu.dd = slu.cc;
    Quater s;
    slu.gauss ( s.ro, s.la, s.mu, s.nu );
    s.ro = sqrt ( s.ro );
    s.la /= s.ro + s.ro;
    s.mu /= s.ro + s.ro;
    s.nu /= s.ro + s.ro;
    s.norm();
    u = q.norm();
    q = s * q;
display << 2*RAD * acos ( q.ro ) << NL;return;
//display << m.aa << m.bb << m.cc << NL;
    if ( 0)//max < 0 )
    {
        q.la = - q.la;
        q.mu = - q.mu;
        m.ca = - m.ca;
        m.cb = - m.cb;
        m.ac = - m.ac;
        m.bc = - m.bc;
    }
    q.getMatr ( m );
}

void calc2 ( CCArrRef<Vector3d> & point1, CCArrRef<Vector3d> & point2, double & u, Matrix3<double> & m )
{
    nat i;
    Vector3d r1, r2;
    calc1 ( point1, point2, r1, r2 );
//display << r1 << NL << r2 << NL << NL;
    Quater q;
    q.init ( r1, r2 );
    q.getMatr ( m );
    DynArray<Vector3d> point3 ( point1.size() );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        point3[i] = m * Vector3d ( v.x, v.y, 0 );
    }
    //display << calcDif ( point2, point3 );
    double sum = 0;
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        double t1 = point2[i].x - ( m.aa * v.x + m.ab * v.y );
        double t2 = point2[i].y - ( m.ba * v.x + m.bb * v.y );
        double z = ( m.ac * t1 + m.bc * t2 ) / ( m.ac * m.ac + m.bc * m.bc );
        point3[i] = m * Vector3d ( v.x, v.y, z );
    }
    //display << calcDif ( point2, point3 ) << NL;
    SLU4<double> slu;
    slu.fill ( 0, 0 );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point3[i];
        const Vector3d & u = point2[i];
        double a = v.x * q.nu + v.y * q.ro - v.z * q.la;
        double b = v.x * q.mu - v.y * q.la - v.z * q.ro;
        double c = v.x * q.la + v.y * q.nu + v.z * q.mu;
        double d = v.x * q.ro + v.y * q.mu - v.z * q.nu;
        slu.aa += a * a + d * d;
        slu.ab += a * b + c * d;
        slu.ac += a * c - b * d;
        slu.bb += b * b + c * c;
        slu.ae += u.x*v.x + u.y*v.y;
        slu.be -= u.x*v.y - u.y*v.x;
        slu.ce += u.x*v.z;
        slu.de -= u.y*v.z;
    }
    slu.dd = slu.aa;
    slu.cc = slu.bb;
    slu.bd = -slu.ac;
    slu.dc = slu.cd = slu.ba = slu.ab;
    slu.ca = slu.ac;
    slu.db = slu.bd;
    Quater s;
    slu.gauss ( s.ro, s.la, s.mu, s.nu );
    s.ro = sqrt ( s.ro );
    s.la /= s.ro + s.ro;
    s.mu /= s.ro + s.ro;
    s.nu /= s.ro + s.ro;
    s.norm();
    u = q.norm();
    q = s * q;
display << 2*RAD * acos ( q.ro ) << NL;return;
//display << m.aa << m.bb << m.cc << NL;
    if ( 0)//max < 0 )
    {
        q.la = - q.la;
        q.mu = - q.mu;
        m.ca = - m.ca;
        m.cb = - m.cb;
        m.ac = - m.ac;
        m.bc = - m.bc;
    }
    q.getMatr ( m );
}

void compare ( const Frame & f1, const Frame & f2, double & u, Matrix3<double> & m )
{
    DynArray<Vector3d> point1 ( f1.point );
    Vector3d o = null3d;
    o += point1;
    o /= point1.size();
    point1 -= o;
    DynArray<Vector3d> point2 ( f2.point );
    o = null3d;
    o += point2;
    o /= point2.size();
    point2 -= o;
    calc ( point1, point2, u, m );
}

void drawFrame ( const Frame & f, float r, float g, float b )
{
    DynArray<Vector3d> point ( f.point );
    Vector3d o = null3d;
    o += point;
    o /= point.size();
    point -= o;
    point /= 600;
    drawPoints  ( point, r, g, b );
}

void test ()
{
    FixArray<Vector2d, 4> poly1;
    poly1[0] = Vector2d ( -81,-44 );
    poly1[1] = Vector2d ( -81, 44 );
    poly1[2] = Vector2d (  81, 44 );
    poly1[3] = Vector2d (  81,-44 );
    poly1 /= 60;
    drawPolygon ( poly1, 0, 1, 1 );
    FixArray<Vector2d, 4> poly2;
    poly2[0] = Vector2d ( -18,-28 );
    poly2[1] = Vector2d ( -18,-20 );
    poly2[2] = Vector2d (  18,-20 );
    poly2[3] = Vector2d (  18,-28 );
    poly2 /= 60;
    drawPolygon ( poly2, 1, 0, 0 );
    FixArray<Vector2d, 4> poly3;
    double h = 6;
    poly3[0] = Vector2d ( -58,-34+h );
    poly3[1] = Vector2d ( -58, 34+h );
    poly3[2] = Vector2d (  58, 34+h );
    poly3[3] = Vector2d (  58,-34+h );
    poly3 /= 60;
    drawPolygon ( poly3, 1, 1, 0 );
    return;
    nat i;
    RealFile file ( "D:/PROJECTS/temp/DATA/vid_1.mp4.log.json", "r" );
    //RealFile file ( "D:/PROJECTS/temp/DATA/VID_20220203_122000.mp4.log.json", "r" );
    //RealFile file ( "D:/PROJECTS/temp/DATA/VID_21_11_21_c.mp4.log.json", "r" );
    JNode json;
    loadJSON ( file, json );
    nat n = json.list.las()->list.size();
    Suite<Frame> frame;
    copyJSON ( json.list.las()->list, frame );
    /*drawFrame ( frame[0], 0, 1, 1 );
    drawFrame ( frame[16], 1, 1, 0 );
    drawFrame ( frame[17], 1, 0.5f, 0.5f );
    return;*
    test ( frame );
    return;*/
    double u;
    Matrix3<double> m;
    double max = 0, min = 2;
//display << frame.size() << NL;
    for ( i = 1; i < frame.size(); ++i )
    {
//if(i!=10) continue;
        const Frame & f1 = frame[0];
        const Frame & f2 = frame[i];
display << f2.angle1;
        compare ( f1, f2, u, m );
        /*if ( max < u ) max = u;
        if ( min > u ) min = u;
    
    double sy = sqrt ( m.aa * m.aa + m.ba * m.ba );
    double x = RAD * atan2 ( m.cb, m.cc );
    double y = RAD * atan2 (-m.ca, sy );*/
//display << i << f2.angle1 << f2.angle2 << NL;
//if(i==352) break;
    }
//display << min << max << NL;
}

} // namespace

void file_test ()
{
    drawNewList2d();
    test();
    endNewList();
//    file_test1 ();
//    writeFltDec_test();
//    printf_test();
}
