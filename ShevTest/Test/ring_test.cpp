
#include "LinAlg.h"

// Кватернион

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
// Вычисление матрицы поворота
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

// Вычисление методом наименьших квадратов преобразования 
// X,Y координат точек point1 в точки point2

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

inline Vector3d operator * ( const Matrix3<double> & m, const Vector3d & v )
{
    return Vector3d ( m.aa * v.x + m.ab * v.y + m.ac * v.z,
                      m.ba * v.x + m.bb * v.y + m.bc * v.z,
                      m.ca * v.x + m.cb * v.y + m.cc * v.z );
}

void calc ( CCArrRef<Vector3d> & point1, CCArrRef<Vector3d> & point2, double & u, Matrix3<double> & m )
{
    nat i;
// Вначале считаем, что Z координаты точек point1 равны нулю.
// Вычисление методом наименьших квадратов преобразования 
// X,Y координат точек point1 в точки point2.
    Vector3d r1, r2;
    calc1 ( point1, point2, r1, r2 );
// Вычисление кватерниона по X,Y координат векторов r1 и r2
    Quater q;
    q.init ( r1, r2 );
// Вычисление матрицы поворота
    q.getMatr ( m );
// Образы точек point1 для данной матрицы поворота
    DynArray<Vector3d> point3 ( point1.size() );
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        point3[i] = m * Vector3d ( v.x, v.y, 0 );
    }
// Определим Z координаты точек point1 и их образы для данной матрицы поворота
    for ( i = 0; i < point1.size(); ++i )
    {
        const Vector3d & v = point1[i];
        double t1 = point2[i].x - ( m.aa * v.x + m.ab * v.y );
        double t2 = point2[i].y - ( m.ba * v.x + m.bb * v.y );
        double z = ( m.ac * t1 + m.bc * t2 ) / ( m.ac * m.ac + m.bc * m.bc );
        point3[i] = m * Vector3d ( v.x, v.y, z );
    }
// Уточнение кватерниона после определения Z координат
    SLU4<double> slu;
    slu.fill ( 0, 0 );
    for ( i = 0; i < point3.size(); ++i )
    {
        const Vector3d & v = point3[i];
        const Vector3d & u = point2[i];
        slu.aa += v.x * v.x + v.y * v.y;
        slu.ac += v.x * v.z;
        slu.ad -= v.y * v.z;
        slu.cc += v.z * v.z;
        slu.ae += u.x * v.x + u.y * v.y;
        slu.be -= u.x * v.y - u.y * v.x;
        slu.ce += u.x * v.z;
        slu.de -= u.y * v.z;
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
    q = s * q;
// Нормализация кватерниона и определение коэффициента увеличения
    u = q.norm();
// Вычисление матрицы поворота
    q.getMatr ( m );
}

// Вычисление коэффициента увеличения и матрицы поворота по двум фреймам

void compare ( const Frame & f1, const Frame & f2, double & u, Matrix3<double> & m )
{
// Пребразование исходных точек так, чтобы их цетры масс были в начале координат
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
// Вычисление коэффициента увеличения и матрицы поворота 
    calc ( point1, point2, u, m );
}
