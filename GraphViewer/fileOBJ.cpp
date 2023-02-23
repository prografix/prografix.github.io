
#include "File.h"
#include "FileOBJ.h"

bool loadOBJ ( IReadFile & file, PutWavefrontOBJ & obj )
{
    char c;
    float x, y, z;
    const nat nsize = 128;
    nat ivert[nsize], inorm[nsize];
    while ( file.getc ( &c ) )
    {
        switch ( c )
        {
        case '\v':
        case '\t':
        case ' ': continue; // если в начале строки пробелы, то пропускаем их
        case 'v': // описание вершины
            {
                if ( file.getc ( &c ) == false ) return false;
                switch ( c )
                {
                case '\v':
                case '\t':
                case ' ': // читаем координаты вершины
                    {
                        if ( ! readFltDec ( file, c, x ) || 
                             ! readFltDec ( file, c, y ) || 
                             ! readFltDec ( file, c, z ) ) return false;
                        obj.putVertex ( x, y, z );
						while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
						break;
                    }
                case 'n': // читаем координаты нормали
					{
						if ( file.getc ( &c ) == false ) return false;
						switch ( c )
						{
						case '\t':
						case ' ':
							{
								if ( ! readFltDec ( file, c, x ) ||
                                     ! readFltDec ( file, c, y ) || 
                                     ! readFltDec ( file, c, z ) ) return false;
								obj.putNormal ( x, y, z );
								while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
								break;
							}
						default: return false;
						}
						break;
					}
                default: while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
                }
                break;
            }
        case 'f': // описание грани
            {
                if ( file.getc ( &c ) == false ) return false;
                switch ( c )
                {
                case '\v':
                case '\t':
                case ' ': // читаем номера вершин
                    {
                        nat nv = 0;
                        nat nn = 0;
                        do
                        {
                            nat32 i;
                            if ( !readIntDec ( file, c, i ) ) break;
                            ivert[nv] = i;
                            if ( ++nv > nsize ) return false;
                            if ( c == '/' )
                            {
                                readIntDec ( file, c, i );
                                if ( c == '/' )
                                {
                                    if ( readIntDec ( file, c, i ) )
                                    {
                                        inorm[nn] = i;
                                        ++nn;
                                    }
                                }
                            }
                        }
                        while ( c != '\n' && c != '\r' );
                        obj.putFacet ( nv, ivert, nv == nn ? inorm : 0 );
                    }
                default: while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
                }
                break;
            }
        default: while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool saveOBJ ( IWriteFile & file, const GetWavefrontOBJ & obj, nat prec )
{
    nat nv, nn, nf; 
    obj.start ( nv, nn, nf );
    if ( nv > 0 )
    {
        if ( ! writeStr ( file, "\n# " ) || ! writeIntDec ( file, (nat32) nv ) || ! writeStr ( file, " vertices\n\n" ) ) return false;
        for ( nat i = 0; i < nv; ++i )
        {
            float x, y, z;
            if ( ! obj.getVertex ( i, x, y, z ) ) return false;
            if ( ! writeStr ( file, "v " ) || 
                 ! writeFltDec ( file, x, prec ) || ! file.putc ( " " ) || 
                 ! writeFltDec ( file, y, prec ) || ! file.putc ( " " ) || 
                 ! writeFltDec ( file, z, prec ) || ! file.putc ( "\n" ) ) return false;
        }

    }
    if ( nn > 0 )
    {
        if ( ! writeStr ( file, "\n# " ) || ! writeIntDec ( file, (nat32) nn ) || ! writeStr ( file, " normals\n\n" ) ) return false;
        for ( nat i = 0; i < nn; ++i )
        {
            float x, y, z;
            if ( ! obj.getNormal ( i, x, y, z ) ) return false;
            if ( ! writeStr ( file, "vn " ) ||
                 ! writeFltDec ( file, x, prec ) || ! file.putc ( " " ) || 
                 ! writeFltDec ( file, y, prec ) || ! file.putc ( " " ) || 
                 ! writeFltDec ( file, z, prec ) || ! file.putc ( "\n" ) ) return false;
        }

    }
    if ( nf > 0 )
    {
        if ( ! writeStr ( file, "\n# " ) || ! writeIntDec ( file, (nat32) nf ) || ! writeStr ( file, " faces\n\n" ) ) return false;
        for ( nat i = 0; i < nf; ++i )
        {
            nat n;
            const nat * vert, * norm;
            if ( ! obj.getFacet ( i, n, vert, norm ) ) return false;
            if ( ! file.putc ( "f" ) ) return false;
            for ( nat j = 0; j < n; ++j )
            {
                if (  ! file.putc ( " " ) || ! writeIntDec ( file, (nat32) vert[j] ) ) return false;
                if ( norm )
                {
                    if (  ! writeStr ( file, "//" ) || ! writeIntDec ( file, (nat32) norm[j] ) ) return false;
                }
            }
            if ( ! file.putc ( "\n" ) ) return false;
        }

    }
    return true;
}