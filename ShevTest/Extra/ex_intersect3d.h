
#ifndef EX_INTERSECT3D_H
#define EX_INTERSECT3D_H

namespace Shev
{
    class Polyhedron;
}

using namespace Shev;

//**************************** 28.09.2013 *********************************//
//
//                  ����������� ������� ��������������
//              eps - ���������� ��� ������� ������� �� �����������
//
//**************************** 16.10.2013 *********************************//

bool intersect ( const Polyhedron & poly1, const Polyhedron & poly2, double eps, Polyhedron & sect );

#endif