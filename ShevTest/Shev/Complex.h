
#pragma once

class Complex
{
public:
    double re, im;

    Complex () {}
    Complex ( double r           ) : re(r), im(0) {}
    Complex ( double r, double i ) : re(r), im(i) {}

    bool operator ! () const
    {
	    return ! re && ! im;
    }

    Complex operator ~ () const
    {
        return Complex ( re, -im );
    }

    Complex operator - () const
    {
        return Complex ( -re, -im );
    }

    Complex & operator = ( const double & b )
    {
	    re = b;
        im = 0.;
        return *this;
    }

    Complex & operator += ( const double & b )
    {
	    re += b;
        return *this;
    }

    Complex & operator -= ( const double & b )
    {
	    re -= b;
        return *this;
    }

    Complex & operator *= ( const double b )
    {
	    re *= b;
        im *= b;
        return *this;
    }

    Complex & operator /= ( const double b )
    {
	    re /= b;
        im /= b;
        return *this;
    }

    Complex & operator += ( const Complex & b )
    {
	    re += b.re;
        im += b.im;
        return *this;
    }

    Complex & operator -= ( const Complex & b )
    {
	    re -= b.re;
        im -= b.im;
        return *this;
    }

    Complex & operator *= ( const Complex & b )
    {
        const double rr = re * b.re - im * b.im;
        const double ii = re * b.im + im * b.re;
        re = rr;
        im = ii;
        return *this;
    }

    Complex & operator /= ( const Complex & b )
    {
	    const double rr = re * b.re + im * b.im;
        const double ii = im * b.re - re * b.im;
        const double sq = b.re * b.re + b.im * b.im;
	    re = rr / sq;
        im = ii / sq;
        return *this;
    }
};

////////////////////////////////////////////////////////////////////////

inline Complex operator + ( const double & a, const Complex & b )
{
	return Complex ( a + b.re, b.im );
}

inline Complex operator - ( const double & a, const Complex & b )
{
	return Complex ( a - b.re, - b.im );
}

inline Complex operator * ( const double & a, const Complex & b )
{
	return Complex ( a * b.re, a * b.im );
}

inline Complex operator / ( const double & a, const Complex & b )
{
    const double sq = b.re * b.re + b.im * b.im;
	return Complex ( a * b.re / sq, - a * b.im / sq );
}

////////////////////////////////////////////////////////////////////////

inline Complex operator + ( const Complex & a, const double & b )
{
	return Complex ( a.re + b, a.im );
}

inline Complex operator - ( const Complex & a, const double & b )
{
	return Complex ( a.re - b, a.im );
}

inline Complex operator * ( const Complex & a, const double & b )
{
	return Complex ( a.re * b, a.im * b );
}

inline Complex operator / ( const Complex & a, const double & b )
{
	return Complex ( a.re / b, a.im / b );
}

////////////////////////////////////////////////////////////////////////

inline Complex operator + ( const Complex & a, const Complex & b )
{
	return Complex ( a.re + b.re, a.im + b.im );
}

inline Complex operator - ( const Complex & a, const Complex & b )
{
	return Complex ( a.re - b.re, a.im - b.im );
}

inline Complex operator * ( const Complex & a, const Complex & b )
{
	return Complex ( a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re );
}

inline Complex operator / ( const Complex & a, const Complex & b )
{
	const double re = a.re * b.re + a.im * b.im;
    const double im = a.im * b.re - a.re * b.im;
    const double sq = b.re * b.re + b.im * b.im;
	return Complex ( re / sq, im / sq );
}

////////////////////////////////////////////////////////////////////////

inline bool operator == ( const Complex & a, const Complex & b )
{
	return ( a.re == b.re ) && ( a.im == b.im );
}

inline bool operator != ( const Complex & a, const Complex & b )
{
	return ( a.re != b.re ) || ( a.im != b.im );
}


//************************ 08.04.2006 *************************//
//
//                    Квадрат модуля числа
//
//************************ 08.04.2006 *************************//

inline double qmod ( const Complex & c )
{
    return c.re * c.re + c.im * c.im;
}

//************************ 08.04.2006 *************************//
//
//             Абсолютное значение ( модуль числа )
//
//************************ 08.04.2006 *************************//

double abs ( const Complex & );


//************************ 18.11.2018 *************************//
//
//                         Экспонента
//
//************************ 18.11.2018 *************************//

Complex exp ( const Complex & );


//************************ 08.04.2006 *************************//
//
//                     Квадратный корень
//
//************************ 08.04.2006 *************************//

Complex sqrt ( const Complex & );


//************************ 24.12.2011 *************************//
//
//                     Кубические корни
//
//************************ 24.12.2011 *************************//

void root3 ( Complex x, Complex & r1, Complex & r2, Complex & r3 );


//************************ 02.12.2011 *************************//
//
//      Решение алгебраического уравнения 2-ой степени
//      с действительными коэффициентами
//      x^2 + a * x + b = 0
//
//************************ 02.12.2011 *************************//

void root2 ( double a, double b, Complex & x1, Complex & x2 );


//************************ 17.12.2011 *************************//
//
//      Решение алгебраического уравнения 2-ой степени
//      с комплексными коэффициентами
//      x^2 + a * x + b = 0
//
//************************ 17.12.2011 *************************//

void root2 ( const Complex & a, const Complex & b, Complex & x1, Complex & x2 );


//************************ 06.12.2011 *************************//
//
//      Решение неполного алгебраического уравнения 3-й степени
//      с действительными коэффициентами
//      x^3 + p * x + q = 0
//
//************************ 06.12.2011 *************************//

void root3s ( double p, double q, double & x1, Complex & x2, Complex & x3 );


//************************ 03.01.2012 *************************//
//
//      Решение неполного алгебраического уравнения 3-й степени
//      с комплексными коэффициентами
//      x^3 + p * x + q = 0
//
//************************ 03.01.2012 *************************//

void root3s ( const Complex & p, const Complex & q, Complex & x1, Complex & x2, Complex & x3 );


//************************ 06.12.2011 *************************//
//
//      Решение полного алгебраического уравнения 3-й степени
//      с действительными коэффициентами
//      x^3 + a * x^2 + b * x + c = 0
//
//************************ 06.12.2011 *************************//

void root3 ( double a, double b, double c, double & x1, Complex & x2, Complex & x3 );


//************************ 04.01.2012 *************************//
//
//      Решение полного алгебраического уравнения 3-й степени
//      с комплексными коэффициентами
//      x^3 + a * x^2 + b * x + c = 0
//
//************************ 04.01.2012 *************************//

void root3 ( const Complex & a, const Complex & b, const Complex & c, 
             Complex & x1, Complex & x2, Complex & x3 );


//************************ 10.12.2011 *************************//
//
//      Решение неполного алгебраического уравнения 4-й степени
//      с действительными коэффициентами
//      x^4 + p * x^2 + q * x + r = 0
//
//************************ 10.12.2011 *************************//

void root4s ( double p, double q, double r, 
              Complex & x1, Complex & x2, Complex & x3, Complex & x4 );


//************************ 04.02.2012 *************************//
//
//      Решение неполного алгебраического уравнения 4-й степени
//      с комплексными коэффициентами
//      x^4 + p * x^2 + q * x + r = 0
//
//************************ 04.02.2012 *************************//

void root4s ( const Complex & p, const Complex & q, const Complex & r, 
              Complex & x1, Complex & x2, Complex & x3, Complex & x4 );


//************************ 10.12.2011 *************************//
//
//      Решение полного алгебраического уравнения 4-й степени
//      с действительными коэффициентами
//      x^4 + a * x^3 + b * x^2 + c * x + d = 0
//
//************************ 10.12.2011 *************************//

void root4 ( double a, double b, double c, double d, 
             Complex & x1, Complex & x2, Complex & x3, Complex & x4 );


//************************ 04.02.2012 *************************//
//
//      Решение полного алгебраического уравнения 4-й степени
//      с комплексными коэффициентами
//      x^4 + a * x^3 + b * x^2 + c * x + d = 0
//
//************************ 04.02.2012 *************************//

void root4 ( const Complex & a, const Complex & b, const Complex & c, const Complex & d, 
             Complex & x1, Complex & x2, Complex & x3, Complex & x4 );


//************************ 12.12.2011 *************************//
//
//      Решение алгебраического уравнения 5-ой степени
//      с действительными коэффициентами
//      x^5 + a * x^4 + b * x^3 + c * x^2 + d * x + e = 0
//
//************************ 12.12.2011 *************************//

void root5 ( double a, double b, double c, double d, double e, double & x1,
             Complex & x2, Complex & x3, Complex & x4, Complex & x5 );

