
const double MICRO  = 1e-9;   //   1. - 1e-16 != 1. && 1. - 1e-17 == 1.

inline bool micro ( const double & d )
{
    return d < MICRO && d + MICRO > 0;
}


class Vector2d;

bool micro ( const Vector2d & v );

bool near_ ( const Vector2d & v1, const Vector2d & v2 );

bool operator || ( const Vector2d & v1, const Vector2d & v2 );


class Vector3d;

bool micro ( const Vector3d & v );

bool near_ ( const Vector3d & v1, const Vector3d & v2 );

bool operator || ( const Vector3d & v1, const Vector3d & v2 );


class Plane3d;

bool near_ ( const Plane3d & p1, const Plane3d & p2 );

