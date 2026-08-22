
#pragma once

#include "ShevArray.h"

class ICutPolygonGuru
{
public:
    virtual CCArrRef<int> & getStatus () = 0;
    virtual bool isOrder ( nat, nat ) = 0;
    virtual nat  newVert ( nat, nat ) = 0;
};

//**************************** 20.08.2026 *********************************//
//
//               Отсечение положительной части многоугольника
//
//**************************** 20.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus );

//**************************** 20.08.2026 *********************************//
//
//               Разрезание многоугольника на две части
//
//**************************** 20.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & plus, SuiteRef< Suite<nat> > & minus );
