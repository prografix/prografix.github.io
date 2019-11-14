
#include "math.h"
#include "micro.h"
#include "Vector2d.h"
#include "Vector3d.h"

bool micro ( const Vector2d & v )
{
    return fabs ( v.x ) < MICRO && fabs ( v.y ) < MICRO;
}

bool near_ ( const Vector2d & v1, const Vector2d & v2 )
{
    return fabs ( v1.x - v2.x ) < MICRO &&
           fabs ( v1.y - v2.y ) < MICRO;
}

bool operator || ( const Vector2d & v1, const Vector2d & v2 )
{
   return micro ( v1 % v2 );
}

bool micro ( const Vector3d & v )
{
    return fabs ( v.x ) < MICRO &&
           fabs ( v.y ) < MICRO &&
           fabs ( v.z ) < MICRO;
}

bool near_ ( const Vector3d & v1, const Vector3d & v2 )
{
    return fabs ( v1.x - v2.x ) < MICRO &&
           fabs ( v1.y - v2.y ) < MICRO &&
           fabs ( v1.z - v2.z ) < MICRO;
}

bool operator || ( const Vector3d & v1, const Vector3d & v2 )
{
   return micro ( v1 % v2 );
}

bool near_ ( const Plane3d & p1, const Plane3d & p2 )
{
   return near_ ( p1.norm, p2.norm ) && micro ( p1.dist - p2.dist );
}
