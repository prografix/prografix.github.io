
#pragma once

#include "ShevArray.h"

//********************** 16.10.2004 *********************************//
//
//      Аппроксимация функции полиномом.
//      Обратите внимание, что m - это не степень полинома, а к-во коэффициентов.
//      Степень полинома равна m-1. 
//
//********************** 10.09.2015 *********************************//

bool apprLSS_H ( nat n, CArrRef<double> x, CArrRef<double> y, nat m, ArrRef<double> c );

bool apprLSS_O ( nat n, CArrRef<double> x, CArrRef<double> y, nat m, ArrRef<double> c );

// С весами w

bool apprLSS_H ( nat n, CArrRef<double> x, CArrRef<double> y, CArrRef<double> w, nat m, ArrRef<double> c );

bool apprLSS_O ( nat n, CArrRef<double> x, CArrRef<double> y, CArrRef<double> w, nat m, ArrRef<double> c );


//************************* 03.12.2016 ******************************//
//
//      Робастный метод аппроксимации.
//      Заполняет массив весов (mass) значениями от 0 до 1.
//
//************************* 03.12.2016 ******************************//

template<class In, class Out> void approx2 ( CArrRef<In> arr, Def<Out> & res );

template<class In, class Out> void approx2 ( CArrRef<In> arr, ArrRef<double> mass, Def<Out> & res );

template<class In, class Out> void approxR ( CArrRef<In> arr, ArrRef<double> mass, Def<Out> & res )
{
    approx2 ( arr, res );
    if ( ! res.isDef ) return;
    const nat n = arr.size();
    for ( nat j = 0; j < n; ++j ) mass[j] = 1.;
    if ( n > 3 )
    {
        DynArray<double> u ( n );
        for ( nat i = 0; i < 20; ++i )
        {
            double mean = 0.;
            nat j;
            for ( j = 0; j < n; ++j )
            {
                u[j] = mass[j];
                mean += ( mass[j] = fabs ( res % arr[j] ) );
            }
            if ( mean == 0 )
            {
                for ( j = 0; j < n; ++j ) mass[j] = 1.;
                break;
            }
            mean /= n;
            for ( j = 0; j < n; ++j )
            {
                const double t = mass[j] / mean;
                mass[j] = t < 2. ? 0.25 * _pow2 ( 2. - t ) : 0.;
            }
            approx2 ( arr, mass, res );
            mean = 0.;
            for ( j = 0; j < n; ++j ) mean += fabs ( mass[j] - u[j] );
            if ( mean < 0.01 * n ) break;
        }
    }
}