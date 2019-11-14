
#include "math.h"
#include "float.h"

#include "../Shev/Vector2d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/File.h"
#include "display.h"

Display & Display::operator << ( char c )
{
    file.putc ( & c );
    file.putc ( " " );
    return *this;
}

Display & Display::operator << ( int32 i )
{
    printf ( file, "%d ", i );
    return *this;
}

Display & Display::operator << ( nat32 n )
{
    printf ( file, "%u ", n );
    return *this;
}

Display & Display::operator << ( bool b )
{
    if ( b )
    {
        file.write ( "true ", 1, 5 );
    }
    else
    {
        file.write ( "false ", 1, 6 );
    }
    return *this;
}

Display & Display::operator << ( double d )
{
    if ( _finite ( d ) == 0 )
    {
        printf ( file, "%f", d );
    }
    else
    if ( d == 0 )
    {
        file.putc ( "0" );
    }
    else
    if ( fabs(d) < 0.01 || fabs(d) > 1e10 )
    {
        printf ( file, "%.3e", d );
    }
    else
    {
        printf ( file, "%.3f", d );
    }
    file.putc ( " " );
    return *this;
}

Display & Display::operator << ( float d )
{
    return *this << (double) d;
}

Display & Display::operator << ( const Vector2d & v )
{
    return *this << v.x << v.y;
}

Display & Display::operator << ( const Vector3d & v )
{
    return *this << v.x << v.y << v.z;
}

Display & Display::operator << ( const char * s )
{
    if ( s )
    {
        printf ( file, s );
        file.putc ( " " );
    }
    return *this;
}

Display & Display::operator << ( const Display & )
{
    file.putc ( "\n" );
    file.flush ();
    return *this;
}
