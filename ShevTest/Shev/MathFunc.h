
#pragma once

/********************** MathFunc1 ************************/

class MathFunc1
{
public:
    virtual double operator () ( double ) const = 0;
};

class Polynom3 : public MathFunc1
{
public:
    double a, b, c, d;

    double operator () ( double x ) const
    {
        return ( ( a * x + b ) * x + c ) * x + d;
    }
};

class Polynom5 : public MathFunc1
{
public:
    double a, b, c, d, e, f;

    double operator () ( double x ) const
    {
        return ( ( ( ( a * x + b ) * x + c ) * x + d ) * x + e ) * x + f;
    }
};


//************************* 28.01.2010 **************************************//
//
//                  Функции двух аргументов
//
//************************* 28.01.2010 **************************************//

class MathFunc2
{
public:
    virtual double operator () ( double, double ) const = 0;
};

class MathAdd : public MathFunc2
{
public:
    virtual double operator () ( double a, double b ) const { return a + b; }
};

class MathMul : public MathFunc2
{
public:
    virtual double operator () ( double a, double b ) const { return a * b; }
};
