
#pragma once

template <unsigned int N> class Double;

template <> class Double<3>
{
public:
    double d0, d1, d2;

    Double<3> & fill ( double d )
    {
        d0 = d1 = d2 = d;
        return *this;
    }
};

inline
Double<3> & operator += ( Double<3> & a, const Double<3> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    return a;
}

inline
Double<3> & operator -= ( Double<3> & a, const Double<3> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    return a;
}

inline
Double<3> & operator *= ( Double<3> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    return a;
}

template <> class Double<4>
{
public:
    double d0, d1, d2, d3;

    Double<4> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d;
        return *this;
    }

    Double<4> & init ( double p0, double p1, double p2, double p3 )
    {
        d0 = p0; d1 = p1; d2 = p2; d3 = p3;
        return *this;
    }

    double & las() { return d3; }
};

inline
Double<4> & operator += ( Double<4> & a, const Double<4> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    return a;
}

inline
Double<4> & operator -= ( Double<4> & a, const Double<4> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    return a;
}

inline
Double<4> & operator *= ( Double<4> & a, const Double<4> & b )
{
    a.d0 *= b.d0;
    a.d1 *= b.d1;
    a.d2 *= b.d2;
    a.d3 *= b.d3;
    return a;
}

inline
Double<4> & operator *= ( Double<4> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    return a;
}

inline
double operator * ( const Double<4> & a, const Double<4> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3 * b.d3;
}

inline
double operator % ( const Double<4> & a, const Double<3> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3;
}

template <> class Double<5>
{
public:
    double d0, d1, d2, d3, d4;

    Double<5> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d;
        return *this;
    }

    Double<5> & init ( double p0, double p1, double p2, double p3, double p4 )
    {
        d0 = p0; d1 = p1; d2 = p2; d3 = p3; d4 = p4;
        return *this;
    }
};

inline
Double<5> & operator += ( Double<5> & a, const Double<5> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    return a;
}

inline
Double<5> & operator -= ( Double<5> & a, const Double<5> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    return a;
}

inline
Double<5> & operator *= ( Double<5> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    return a;
}

inline
double operator * ( const Double<5> & a, const Double<5> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + 
           a.d2 * b.d2 + a.d3 * b.d3 + a.d4 * b.d4;
}

inline
double operator % ( const Double<5> & a, const Double<4> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + 
           a.d2 * b.d2 + a.d3 * b.d3 + a.d4;
}

template <> class Double<6>
{
public:
    double d0, d1, d2, d3, d4, d5;

    Double<6> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d;
        return *this;
    }

    Double<6> & init ( double p0, double p1, double p2, double p3, double p4, double p5 )
    {
        d0 = p0; d1 = p1; d2 = p2; d3 = p3; d4 = p4; d5 = p5;
        return *this;
    }

    double & las() { return d5; }
};

inline
Double<6> & operator += ( Double<6> & a, const Double<6> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    a.d5 += b.d5;
    return a;
}

inline
Double<6> & operator -= ( Double<6> & a, const Double<6> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    a.d5 -= b.d5;
    return a;
}

inline
Double<6> & operator *= ( Double<6> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    a.d5 *= b;
    return a;
}

inline
double operator * ( const Double<6> & a, const Double<6> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + 
           a.d3 * b.d3 + a.d4 * b.d4 + a.d5 * b.d5;
}

inline
double operator % ( const Double<6> & a, const Double<5> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + 
           a.d3 * b.d3 + a.d4 * b.d4 + a.d5;
}

template <> class Double<7>
{
public:
    double d0, d1, d2, d3, d4, d5, d6;

    Double<7> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d6 = d;
        return *this;
    }

    Double<7> & init ( double p0, double p1, double p2, double p3, double p4, double p5, double p6 )
    {
        d0 = p0; d1 = p1; d2 = p2; d3 = p3; d4 = p4; d5 = p5; d6 = p6;
        return *this;
    }
};

inline
Double<7> & operator += ( Double<7> & a, const Double<7> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    a.d5 += b.d5;
    a.d6 += b.d6;
    return a;
}

inline
Double<7> & operator -= ( Double<7> & a, const Double<7> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    a.d5 -= b.d5;
    a.d6 -= b.d6;
    return a;
}

inline
Double<7> & operator *= ( Double<7> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    a.d5 *= b;
    a.d6 *= b;
    return a;
}

inline
double operator * ( const Double<7> & a, const Double<7> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + 
           a.d3 * b.d3 + a.d4 * b.d4 + a.d5 * b.d5 + a.d6 * b.d6;
}


inline
double operator % ( const Double<7> & a, const Double<6> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + 
           a.d3 * b.d3 + a.d4 * b.d4 + a.d5 * b.d5 + a.d6;
}

template <> class Double<8>
{
public:
    double d0, d1, d2, d3, d4, d5, d6, d7;
    Double<8> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d6 = d7 = d;
        return *this;
    }
};

inline
Double<8> & operator += ( Double<8> & a, const Double<8> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    a.d5 += b.d5;
    a.d6 += b.d6;
    a.d7 += b.d7;
    return a;
}

inline
Double<8> & operator -= ( Double<8> & a, const Double<8> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    a.d5 -= b.d5;
    a.d6 -= b.d6;
    a.d7 -= b.d7;
    return a;
}

inline
Double<8> & operator *= ( Double<8> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    a.d5 *= b;
    a.d6 *= b;
    a.d7 *= b;
    return a;
}

inline
double operator % ( const Double<8> & a, const Double<7> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3 * b.d3 +
           a.d4 * b.d4 + a.d5 * b.d5 + a.d6 * b.d6 + a.d7;
}

template <> class Double<9>
{
public:
    double d0, d1, d2, d3, d4, d5, d6, d7, d8;

    Double<9> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d6 = d7 = d8 = d;
        return *this;
    }

    double & las() { return d8; }
};

inline
Double<9> & operator += ( Double<9> & a, const Double<9> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    a.d5 += b.d5;
    a.d6 += b.d6;
    a.d7 += b.d7;
    a.d8 += b.d8;
    return a;
}

inline
Double<9> & operator -= ( Double<9> & a, const Double<9> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    a.d5 -= b.d5;
    a.d6 -= b.d6;
    a.d7 -= b.d7;
    a.d8 -= b.d8;
    return a;
}

inline
Double<9> & operator *= ( Double<9> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    a.d5 *= b;
    a.d6 *= b;
    a.d7 *= b;
    a.d8 *= b;
    return a;
}

inline
double operator % ( const Double<9> & a, const Double<8> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3 * b.d3 +
           a.d4 * b.d4 + a.d5 * b.d5 + a.d6 * b.d6 + a.d7 * b.d7 + a.d8;
}

template <> class Double<10>
{
public:
    double d0, d1, d2, d3, d4, d5, d6, d7, d8, d9;

    Double<10> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d6 = d7 = d8 = d9 = d;
        return *this;
    }

    double & las() { return d9; }
};

inline
Double<10> & operator += ( Double<10> & a, const Double<10> & b )
{
    a.d0 += b.d0;
    a.d1 += b.d1;
    a.d2 += b.d2;
    a.d3 += b.d3;
    a.d4 += b.d4;
    a.d5 += b.d5;
    a.d6 += b.d6;
    a.d7 += b.d7;
    a.d8 += b.d8;
    a.d9 += b.d9;
    return a;
}

inline
Double<10> & operator -= ( Double<10> & a, const Double<10> & b )
{
    a.d0 -= b.d0;
    a.d1 -= b.d1;
    a.d2 -= b.d2;
    a.d3 -= b.d3;
    a.d4 -= b.d4;
    a.d5 -= b.d5;
    a.d6 -= b.d6;
    a.d7 -= b.d7;
    a.d8 -= b.d8;
    a.d9 -= b.d9;
    return a;
}

inline
Double<10> & operator *= ( Double<10> & a, const double & b )
{
    a.d0 *= b;
    a.d1 *= b;
    a.d2 *= b;
    a.d3 *= b;
    a.d4 *= b;
    a.d5 *= b;
    a.d6 *= b;
    a.d7 *= b;
    a.d8 *= b;
    a.d9 *= b;
    return a;
}

inline
double operator % ( const Double<10> & a, const Double<9> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3 * b.d3 + a.d4 * b.d4 + 
           a.d5 * b.d5 + a.d6 * b.d6 + a.d7 * b.d7 + a.d8 * b.d8 + a.d9;
}

template <> class Double<11>
{
public:
    double d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
    Double<11> & fill ( double d )
    {
        d0 = d1 = d2 = d3 = d4 = d5 = d6 = d7 = d8 = d9 = d10 = d;
        return *this;
    }
};

inline
double operator % ( const Double<11> & a, const Double<10> & b )
{
    return a.d0 * b.d0 + a.d1 * b.d1 + a.d2 * b.d2 + a.d3 * b.d3 + a.d4 * b.d4 + 
           a.d5 * b.d5 + a.d6 * b.d6 + a.d7 * b.d7 + a.d8 * b.d8 + a.d9 * b.d9 + a.d10;
}

template <unsigned int N> inline
Double<N> operator * ( const Double<N> & a, const double & b )
{
    return Double<N>(a) *= b;
}

template <unsigned int N>
bool cutSimplex ( Double<N> * simplex, const Double<N> & norm, const double & dist )
{
    double sg, max;
    unsigned int i, ib = 0;
    for ( i = 1; i <= N; ++i )
    {
        const Double<N> & v = simplex[i];
        double t = norm * v;
        if ( t > -1e-8 ) continue;
        t = 1./ t;
        if ( ib == 0 )
        {
            max = v.d0 * t;
            ib = i;
            sg = t;
        }
        else
        {
            const double s = v.d0 * t;
            if ( s < max ) max = s, ib = i, sg = t;
        }
    }
    if ( ib == 0 )
        return false;
    const Double<N> & v = simplex[ib];
    simplex[0] -= v * ( dist * sg );
    for ( i = 1; i <= N; ++i )
    {
        if ( i == ib ) continue;
        Double<N> & ai = simplex[i];
        ai -= v * ( ( norm * ai ) * sg );
        ai *= ( 1./ sqrt ( ai * ai ) );
    }
    return true;
}
