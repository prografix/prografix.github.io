
#include <math.h>
#include "../shev/ShevList.h"
#include "../shev/ShevArray.h"
#include "../shev/Vector2i.h"

#define SCALE 16
#define HALF  8

class Zveno : public ShevItem
{
public:
    Vector2i a, b;

    Zveno ( const Vector2i & x,  const Vector2i & y ) : a ( x ), b ( y ) {}

    int qmod () const
    {
        return ::qmod ( b - a );
    }

    int operator == ( const Zveno & r ) const
    {
        return r.a == a && r.b == b;
    }

};

class Cep : public ListItem< List<Zveno> >
{
public:
    int  square();
    Cep & invert();
};

class LevelSource
{
public:
    virtual int getValue ( int x, int y ) const = 0;
};

class MapContext
{
    int nx, ny, x1, x2, y1, y2;
    const unsigned char * data;
    const LevelSource & ls;
public:
    MapContext::MapContext ( int sizeX, int sizeY, const unsigned char * map,
                             const int g[4], const LevelSource & pls ) : ls ( pls )
    {
        nx = sizeX;
        ny = sizeY;
        data = map;
        x1 = g[0];
        x2 = nx - g[1];
        y1 = g[2];
        y2 = ny - g[3];
    }
    void getBoundX ( int & i1, int & i2 ) const
    {
        i1 = x1;
        i2 = x2;
    }
    void getBoundY ( int & j1, int & j2 ) const
    {
        j1 = y1;
        j2 = y2;
    }
    bool isBody ( const Vector2i & v ) const
    {
        const int d = getValue(v.x,v.y);
        return d >= 0 && d <= ls.getValue(v.x,v.y);
    }
    bool isBody ( int x, int y ) const
    {
        const int d = getValue(x,y);
        return d >= 0 && d <= ls.getValue(x,y);
    }
    int getDist ( const Vector2i & a, const Vector2i & b ) const
    {
        int sa = getValue ( a.x, a.y );
        if ( sa < 0 ) return HALF;
        int sb = getValue ( b.x, b.y );
        if ( sb < 0 ) return HALF;
        Vector2i la ( 0, ls.getValue(a.x,a.y) ), lb ( 1, ls.getValue(b.x,b.y) );
        Vector2i va ( 0, sa ), vb ( 1, sb );
        Vector2i ld = lb - la;
        int d = ( vb - va ) % ld;
        if ( !d ) return HALF;
        int r = ( lb - va ) % ld;
        r = ( SCALE * r + d/2 ) / d;
        if ( r < 1 ) r = 1; else
        if ( r >= SCALE ) r = SCALE - 1;
        return r;
    }
    int getValue ( int x, int y ) const
    {
        if ( x < x1 || x >= x2 || y < y1 || y >= y2 ) return -1;
        return data[nx*y+x];
    }
};

static void addZveno ( const Vector2i & a, const Vector2i & b,
                       List<Cep> & list, List<Cep> & itog )
{
    Cep * c1 = 0;
    Cep * c2 = 0;
    if ( list.top() )
    do
    {
        Cep * cep = list.cur();
        cep->top();
        if ( cep->cur()->a == b )
        {
            c1 = cep;
            break;
        }
    }
    while ( list.next() );
    if ( list.top() )
    do
    {
        Cep * cep = list.cur();
        cep->end();
        if ( cep->cur()->b == a )
        {
            c2 = cep;
            break;
        }
    }
    while ( list.next() );
    if(c2)
    {
        c2->addAftLas(new Zveno( a, b ));
        if(c1)
        {
            list.jump(c2);
            if(c1==c2)
            {
                list.movCurAftCur(itog);
            }
            else
            {
                c2->movAllBefFir(*c1);
                list.delCur();
            }
        }
        return;
    }
    if(c1)
    {
        c1->addBefFir(new Zveno( a, b ));
    }
    else
    {
        c1 = new Cep;
        c1->addBefFir(new Zveno( a, b ));
        list.addAftCur(c1);
    }
}

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

void borderAll ( MapContext & mc, List<Cep> & itog )
{
    Vector2i a, b;
    List<Cep> list;
    int i1, i2, j1, j2;
    mc.getBoundX ( i1, i2 );
    mc.getBoundY ( j1, j2 );
    for ( int i = i1+1; i < i2; ++i )
    {
        int x = SCALE * (i-1);
        for ( int j = j1+1; j < j2; ++j )
        {
            int k = 0;
            Vector2i v3 (i  ,j  );
            Vector2i v2 (i-1,j  );
            Vector2i v1 (i  ,j-1);
            Vector2i v0 (i-1,j-1);
            if ( mc.isBody(v0) ) k |= 1;
            if ( mc.isBody(v1) ) k |= 2;
            if ( mc.isBody(v2) ) k |= 4;
            if ( mc.isBody(v3) ) k |= 8;
            int y = SCALE * (j-1);
            int zveno = 1;
            switch ( k )
            {
            case 1: a.x = x + mc.getDist(v0,v1);
                    a.y = y;
                    b.x = x;
                    b.y = y + mc.getDist(v0,v2);
                    break;
            case 2: a.x = x + SCALE;
                    a.y = y + mc.getDist(v1,v3);
                    b.x = x + mc.getDist(v0,v1);
                    b.y = y;
                    break;
            case 3: a.x = x + SCALE;
                    a.y = y + mc.getDist(v1,v3);
                    b.x = x;
                    b.y = y + mc.getDist(v0,v2);
                    break;
            case 4: a.x = x;
                    a.y = y + mc.getDist(v0,v2);
                    b.x = x + mc.getDist(v2,v3);
                    b.y = y + SCALE;
                    break;
            case 5: a.x = x + mc.getDist(v0,v1);
                    a.y = y;
                    b.x = x + mc.getDist(v2,v3);
                    b.y = y + SCALE;
                    break;
            case 6: a.x = x;
                    a.y = y + mc.getDist(v0,v2);
                    b.x = x + mc.getDist(v2,v3);
                    b.y = y + SCALE;
                    addZveno ( a, b, list, itog );
                    a.x = x + SCALE;
                    a.y = y + mc.getDist(v1,v3);
                    b.x = x + mc.getDist(v0,v1);
                    b.y = y;
                    break;
            case 7: a.x = x + SCALE;
                    a.y = y + mc.getDist(v1,v3);
                    b.x = x + mc.getDist(v2,v3);
                    b.y = y + SCALE;
                    break;
            case 8: b.x = x + SCALE;
                    b.y = y + mc.getDist(v1,v3);
                    a.x = x + mc.getDist(v2,v3);
                    a.y = y + SCALE;
                    break;
            case 9: a.x = x + mc.getDist(v0,v1);
                    a.y = y;
                    b.x = x;
                    b.y = y + mc.getDist(v0,v2);
                    addZveno ( a, b, list, itog );
                    b.x = x + SCALE;
                    b.y = y + mc.getDist(v1,v3);
                    a.x = x + mc.getDist(v2,v3);
                    a.y = y + SCALE;
                    break;
            case 10:b.x = x + mc.getDist(v0,v1);
                    b.y = y;
                    a.x = x + mc.getDist(v2,v3);
                    a.y = y + SCALE;
                    break;
            case 11:b.x = x;
                    b.y = y + mc.getDist(v0,v2);
                    a.x = x + mc.getDist(v2,v3);
                    a.y = y + SCALE;
                    break;
            case 12:b.x = x + SCALE;
                    b.y = y + mc.getDist(v1,v3);
                    a.x = x;
                    a.y = y + mc.getDist(v0,v2);
                    break;
            case 13:b.x = x + SCALE;
                    b.y = y + mc.getDist(v1,v3);
                    a.x = x + mc.getDist(v0,v1);
                    a.y = y;
                    break;
            case 14:b.x = x + mc.getDist(v0,v1);
                    b.y = y;
                    a.x = x;
                    a.y = y + mc.getDist(v0,v2);
                    break;
            default:zveno = 0;
            }
            if(zveno) addZveno ( a, b, list, itog );
        }
    }
}

void testFunc ( int sizeX, int sizeY, const unsigned char * data, const int g[4], const LevelSource & ls )
{
    MapContext map ( sizeX, sizeY, data, g, ls );
    List<Cep> itog;
    borderAll ( map, itog );
}
