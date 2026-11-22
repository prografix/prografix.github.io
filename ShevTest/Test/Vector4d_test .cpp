
#include "math.h"

#include "../Shev/template.h"
#include "../Shev/Vector4d.h"
#include "../Shev/rand.h"

#include "display.h"

double timeInSec();

namespace
{
	
void convexHull_test ()
{
	PRandVector4d rand;
	DynArray<Vector4d> point, vert;
	const nat n = 99;
	point.resize(n);
	for ( nat i = 0; i < n; ++i ) point[i] = rand();
	convexHull ( point, vert );
	display << n << vert.size() << NL;
}

} // namespace

void vector4d_test ()
{
	convexHull_test ();
}
