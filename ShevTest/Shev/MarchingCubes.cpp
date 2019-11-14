
#include "lists.h"
#include "func1t.h"
#include "MarchingCubes.h"

//       1     |     2     |     3     |     4     |    5     |     6     |     7     |
//             |           |           |           |          |           |           |
//    o     o  |  o     o  |  o     o  |  x  ^  o  | x  ^  o  |  x  ^  o  |  x  ^  o  |
//             |           |           |    /      |    |     |    /      |      \    |
//    <-       |        /  |  <------  |  /        |    |     |  /     /  |        \  |
//      \      |      /    |           |           |    |     |      /    |           |
//    x  \  o  |  o  v  x  |  x     x  |  o     o  | x  |  o  |  o  v  x  |  x     x  |
//             |           |           |           |          |           |           |
//      14     |    13     |    12     |    11     |   10     |     9     |     8     |
//             |           |           |           |          |           |           |
//    x     x  |  x     x  |  x     x  |  o  /  x  | o  |  x  |  o  \  x  |  o  \  x  |
//             |           |           |    /      |    |     |      \    |      \    |
//    \        |       ->  |  ------>  |  <-       |    |     |  <-   ->  |       ->  |
//      \      |      /    |           |           |    |     |    \      |           |
//    o  v  x  |  x  /  o  |  o     o  |  x     x  | o  v  x  |  x  \  o  |  o     o  |
//             |           |           |           |          |           |           |

void addBones ( const nat p1, const nat p2, const nat p3, const nat p4, const nat code, List2n & res, List2n & stor )
{
    if ( code < 1 || code > 14 ) return;
    Item2n * p = res.addNewAftLas ( stor );
    switch ( code )
    {
    case  1: p->b = p4; p->a = p1; return;
    case  2: p->b = p3; p->a = p4; return;
    case  3: p->b = p3; p->a = p1; return;
    case  4: p->b = p1; p->a = p2; return;
    case  5: p->b = p4; p->a = p2; return;
    case  6: p->b = p1; p->a = p2; break;
    case  7: p->b = p3; p->a = p2; return;
    case  8: p->b = p2; p->a = p3; return;
    case  9: p->b = p2; p->a = p3; break;
    case 10: p->b = p2; p->a = p4; return;
    case 11: p->b = p2; p->a = p1; return;
    case 12: p->b = p1; p->a = p3; return;
    case 13: p->b = p4; p->a = p3; return;
    case 14: p->b = p1; p->a = p4; return;
    }
    p = res.addNewAftLas ( stor );
    if ( code == 6 )
    {
        p->b = p3; p->a = p4;
    }
    else
    {
        p->b = p4; p->a = p1;
    }
}

//          Нумерация вершин и рёбер куба
//
//         4-----7        +-7---+        +-----+
//        /|    /|       4|    6|       /8    /1
//       5-----6 |      +---5-+ |      +-----+ 1
//       | 0---|-3      | +-3-|-+      9 +---1-+
//       |/    |/       |0    |2       |/    0/
//       1-----2        +---1-+        +-----+

MarchingCubes1::MarchingCubes1()
{
    bool flag[8];
    List2n segm, stor;
    List<ListItem<List2n> > plist;
    for ( nat i = 1; i < 255; ++i )
    {
        for ( nat j = 0; j < 8; ++j )
        {
            flag[j] = ( i & ( 1 << j ) ) != 0;
        }
        nat code = 0;
        if ( flag[0] ) code |= 1;
        if ( flag[1] ) code |= 4;
        if ( flag[2] ) code |= 8;
        if ( flag[3] ) code |= 2;
        addBones ( 0, 1, 2, 3, code, segm, stor );
        code = 0;
        if ( flag[1] ) code |= 1;
        if ( flag[5] ) code |= 4;
        if ( flag[6] ) code |= 8;
        if ( flag[2] ) code |= 2;
        addBones ( 9, 5, 10, 1, code, segm, stor );
        code = 0;
        if ( flag[2] ) code |= 1;
        if ( flag[6] ) code |= 4;
        if ( flag[7] ) code |= 8;
        if ( flag[3] ) code |= 2;
        addBones ( 10, 6, 11, 2, code, segm, stor );
        code = 0;
        if ( flag[3] ) code |= 1;
        if ( flag[7] ) code |= 4;
        if ( flag[4] ) code |= 8;
        if ( flag[0] ) code |= 2;
        addBones ( 11, 7, 8, 3, code, segm, stor );
        code = 0;
        if ( flag[0] ) code |= 1;
        if ( flag[4] ) code |= 4;
        if ( flag[5] ) code |= 8;
        if ( flag[1] ) code |= 2;
        addBones ( 8, 4, 9, 0, code, segm, stor );
        code = 0;
        if ( flag[7] ) code |= 1;
        if ( flag[6] ) code |= 4;
        if ( flag[5] ) code |= 8;
        if ( flag[4] ) code |= 2;
        addBones ( 6, 5, 4, 7, code, segm, stor );
        makeClosed ( segm, plist );
        DynArray<DynArray<nat> > & ref = poly[i];
        ref.resize ( plist.size() );
        plist.top();
        for ( nat j = 0; j < ref.size(); ++j )
        {
            List2n & list = *plist.cur();
            DynArray<nat> & array = ref[j];
            array.resize ( list.size() );
            list.top();
            for ( nat k = 0; k < array.size(); ++k )
            {
                array[k] = list.cur()->a;
                list.next();
            }
            list.movAllAftLas ( stor );
            plist.next();
        }
        plist.delAll();
    }
}

#include "trian3d.h"

MarchingCubes2::MarchingCubes2()
{
    MarchingCubes1 mc1;
    Vector3d vertex[12];
    const double d = 0.5;
    vertex[0] = Vector3d ( d, 0, 0 );
    vertex[1] = Vector3d ( 1, d, 0 );
    vertex[2] = Vector3d ( d, 1, 0 );
    vertex[3] = Vector3d ( 0, d, 0 );
    vertex[4] = Vector3d ( d, 0, 1 );
    vertex[5] = Vector3d ( 1, d, 1 );
    vertex[6] = Vector3d ( d, 1, 1 );
    vertex[7] = Vector3d ( 0, d, 1 );
    vertex[8] = Vector3d ( 0, 0, d );
    vertex[9] = Vector3d ( 1, 0, d );
    vertex[10]= Vector3d ( 1, 1, d );
    vertex[11]= Vector3d ( 0, 1, d );
    Suite<Vector3d> vtemp;
    Suite<Set3<nat> > ntemp, itemp;
    for ( nat i = 1; i < 255; ++i )
    {
        CArrRef<DynArray<nat> > arr = mc1[i];
        ntemp.resize();
        for ( nat j = 0; j < arr.size(); ++j )
        {
            CArrRef<nat> index = arr[j];
            if ( index.size() == 3 )
            {
                ntemp.inc() = Set3<nat> ( index[0], index[1], index[2] );
            }
            else
            {
                vtemp.resize ( index.size() );
                for ( nat m = 0; m < vtemp.size(); ++m ) vtemp[m] = vertex[index[m]];
                trian ( vtemp, itemp );
                for ( nat k = 0; k < itemp.size(); ++k )
                {
                    const Set3<nat> & s = itemp[k];
                    ntemp.inc() = Set3<nat> ( index[s.a], index[s.b], index[s.c] );
                }
            }
        }
        poly[i] = ntemp;
    }
}