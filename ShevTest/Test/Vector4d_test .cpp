
#include "math.h"

#include "../Shev/template.h"
#include "../Shev/Vector4d.h"
#include "../Shev/rand.h"

#include "display.h"

double timeInSec();

namespace
{
/*
3 1 4 5 2 
5 8 0 1 7 
8 7 1 0 2 
5 0 1 2 6 
6 7 3 1 2  
*/
void convexHull_test ()
{
	static PRandVector4d rand;
	DynArray<Vector4d> point;
	DynArray<nat> vert;
	DynArray<Facet4d> facet;
	const nat n = 99;
	point.resize(n);
	for ( nat i = 0; i < n; ++i ) point[i] = rand();
	point += rand();
	convexHull ( point, vert, facet );
display << n << vert.size() << facet.size() << NL;
	for ( nat i = 0; i < facet.size(); ++i )
	{
		const Facet4d & fi = facet[i];
		const Plane4d & pi = fi.plane;
		double max = pi % point[vert[0]];
		for ( nat j = 1; j < vert.size(); ++j )
		{
			_maxa ( max, pi % point[vert[j]] );
		}
		if ( max > 1e-12 )
			display << i << max << NL;
		max = 0;
		for ( nat j = 0; j < 4; ++j )
		{
			//display << j << pi % point[fi.vertex[j]] << NL;
			_maxa ( max, fabs ( pi % point[fi.vertex[j]] ) );
		}
		if ( max > 1e-12 )
			display << i << max << NL;
	}
	display << "end" << NL;
}

} // namespace

void vector4d_test ()
{
	convexHull_test ();
}
