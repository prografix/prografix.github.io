
#pragma once

class IReadFile;
class IWriteFile;

namespace Shev
{
    class Polyhedron;
}

bool savePYH ( IWriteFile & file, const Shev::Polyhedron & poly, const char * text = 0 );
bool loadPYH ( IReadFile  & file,       Shev::Polyhedron & poly,       char * text = 0 ); // text[80]

bool savePYH ( const char * filename, const Shev::Polyhedron & poly, const char * text = 0 );
bool loadPYH ( const char * filename,       Shev::Polyhedron & poly,       char * text = 0 ); // text[80]
