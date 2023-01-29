

namespace Shev
{
    class Polyhedron;
}

bool loadOBJ ( const char * filename,       Shev::Polyhedron & poly );
bool saveOBJ ( const char * filename, const Shev::Polyhedron & poly );