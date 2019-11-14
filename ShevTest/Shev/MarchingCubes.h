
#pragma once

#include "ShevArray.h"

//          Нумерация вершин и рёбер куба
//
//         4-----7        +-7---+        +-----+
//        /|    /|       4|    6|       /8    /1
//       5-----6 |      +---5-+ |      +-----+ 1
//       | 0---|-3      | +-3-|-+      9 +---1-+
//       |/    |/       |0    |2       |/    0/
//       1-----2        +---1-+        +-----+

class MarchingCubes1
{
    DynArray<DynArray<nat> > poly[256];
public:
    MarchingCubes1();
    CArrRef<DynArray<nat> > operator[] ( nat8 i ) const
    {
        return poly[i];
    }
};

class MarchingCubes2
{
    DynArray<Set3<nat> > poly[256];
public:
    MarchingCubes2();
    CArrRef<Set3<nat> > operator[] ( nat8 i ) const
    {
        return poly[i];
    }
};
