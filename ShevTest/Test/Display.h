
#ifndef DISPLAY_H
#define DISPLAY_H

#include "../Shev/typedef.h"

class Display
{
public:
	class IWriteFile & file;

	explicit Display ( class IWriteFile & p ) : file ( p ) {}

    Display & operator << ( bool );
    Display & operator << ( char );
    Display & operator << ( const char * );
    Display & operator << ( int32 );
    Display & operator << ( nat32 );
    Display & operator << ( float );
    Display & operator << ( double );
    Display & operator << ( const class Vector2d & );
    Display & operator << ( const class Vector3d & );
    Display & operator << ( const Display & );
};

extern Display display;

#define NL display

#if _MSC_VER == 1200 // MS VC 6.0

inline Display & operator << ( Display & disp, signed int i )
{
    return disp << ( (int32) i );
}

inline Display & operator << ( Display & disp, unsigned int n )
{
    return disp << ( (nat32) n );
}

#endif

#endif