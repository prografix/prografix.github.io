
#include "stdafx.h"
#include "gl\gl.h"
#include "math.h"
#include "ShevTestView.h"

#include "draw.h"
#include "Shev/tune.h"
#include "Shev/func2d.h"
#include "Shev/func3d.h"
#include "Shev/Vector2d.h"
#include "Shev/Vector3d.h"
#include "Shev/Vector3f.h"
#include "Shev/Poly3gon.h"
#include "Shev/Polyhedron.h"

void draw ( const Polyhedron & poly, float r, float g, float b, float a, ViewMode vm )
{
    if ( vm == VM_WIRE )
    {
        glDisable ( GL_LIGHT0 );
        glDisable ( GL_LIGHTING );
        glEnable  ( GL_CULL_FACE );
        glDisable ( GL_NORMALIZE );
        glEnable  ( GL_DEPTH_TEST );
        glDepthFunc ( GL_LEQUAL );
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );

        for ( nat k = 0; k < 2; ++k )
        {
            if ( k == 0 )
            {
                glEnable  ( GL_LINE_STIPPLE );
                glLineStipple ( 1, 0x2222 );
                glCullFace ( GL_FRONT );
                glColor4f ( 0.5f*r, 0.5f*g, 0.5f*b, a );
            }
            else
            {
                glDisable  ( GL_LINE_STIPPLE );
                glCullFace ( GL_BACK );
                glColor4f ( r, g, b, a );
                glPolygonMode ( GL_FRONT, GL_LINE );
            }
            for ( nat i = 0; i < poly.facet.size(); ++i )
            {
                const Facet & facet = poly.facet[i];
                if ( facet.nv == 3 )
                {
                    glBegin ( GL_TRIANGLES );
                    for ( nat j = 0; j < 3; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                if ( facet.nv == 4 )
                {
                    glBegin ( GL_QUADS );
                    for ( nat j = 0; j < 4; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                {
                    glBegin ( GL_POLYGON );
                    for ( nat j = 0; j < facet.nv; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
            }
        }

    }
    else
    {
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glDisable( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );

        float mat[4];
        mat[0] = 0.2f * r;
        mat[1] = 0.2f * g;
        mat[2] = 0.2f * b;
        mat[3] = a;
        glMaterialfv ( GL_FRONT, GL_AMBIENT, mat );
        mat[0] *= 4.0f;
        mat[1] *= 4.0f;
        mat[2] *= 4.0f;
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat );
        if ( vm == VM_FLAT )
        {
	        for ( nat i = 0; i < poly.facet.size(); ++i )
            {
                const Facet & facet = poly.facet[i];
                if ( facet.nv == 3 )
                {
                    glBegin ( GL_TRIANGLES );
			        glNormal3dv ( & facet.plane.norm.x );
                    for ( nat j = 0; j < 3; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                if ( facet.nv == 4 )
                {
                    glBegin ( GL_QUADS );
			        glNormal3dv ( & facet.plane.norm.x );
                    for ( nat j = 0; j < 4; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                {
                    glBegin ( GL_POLYGON );
			        glNormal3dv ( & facet.plane.norm.x );
                    for ( nat j = 0; j < facet.nv; ++j )
                    {
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
            }
        }
        else
        {
	        DynArray<Vector3d> norm ( poly.vertex.size() );
			nat i, j;
	        for ( j = 0; j < poly.vertex.size(); ++j ) norm[j].fill();
            for ( i = 0; i < poly.facet.size(); ++i )
            {
		        Vector3d n ( 0., 0., 0. );
                const Facet & facet = poly.facet[i];
                for ( j = 0; j < facet.nv; ++j )
                {
                    n += poly.vertex[facet.index[j]] % poly.vertex[facet.index[j+1]];
                }
                for ( j = 0; j < facet.nv; ++j ) norm[facet.index[j]] += n;
	        }
            for ( i = 0; i < poly.facet.size(); ++i )
            {
                const Facet & facet = poly.facet[i];
                if ( facet.nv == 3 )
                {
                    glBegin ( GL_TRIANGLES );
                    for ( j = 0; j < 3; ++j )
                    {
			            glNormal3dv ( & norm[facet.index[j]].x );
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                if ( facet.nv == 4 )
                {
                    glBegin ( GL_QUADS );
                    for ( j = 0; j < 4; ++j )
                    {
			            glNormal3dv ( & norm[facet.index[j]].x );
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
                else
                {
                    glBegin ( GL_POLYGON );
                    for ( j = 0; j < facet.nv; ++j )
                    {
			            glNormal3dv ( & norm[facet.index[j]].x );
                        glVertex3dv ( & poly.vertex[facet.index[j]].x );
                    }
                    glEnd ();
                }
            }
        }
    }
}

void drawNewList2d ()
{
    glNewList ( 1, GL_COMPILE );
    glDisable ( GL_LIGHT0 );
    glDisable ( GL_LIGHTING );
    glDisable ( GL_CULL_FACE );
    glDisable ( GL_NORMALIZE );
    glDisable ( GL_DEPTH_TEST );
}

void endNewList ()
{
    glEndList();
    glNewList ( 2, GL_COMPILE );
    glCallList ( 1 );
    glEndList();
    glNewList ( 3, GL_COMPILE );
    glCallList ( 1 );
    glEndList();
    view->Invalidate(TRUE);
}

void draw ( const Def<Segment2d> & seg, float r, float g, float b )
{
    if ( ! seg.isDef ) return;
    glColor3f ( r, g, b );
    glBegin ( GL_LINES );
    glVertex2dv ( & seg.a.x );
    glVertex2dv ( & seg.b.x );
    glEnd ();
}

void draw ( const Def<Triangle2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    glEnable ( GL_DEPTH_TEST );
    glColor3f ( r, g, b );
    glBegin ( GL_LINE_LOOP );
    glVertex2dv ( & fig.a.x );
    glVertex2dv ( & fig.b.x );
    glVertex2dv ( & fig.c.x );
    glEnd ();
}

void drawLine ( CCArrRef<Vector2d> & poly, float r, float g, float b )
{
    glColor3f ( r, g, b );
    glBegin ( GL_LINE_STRIP );
    for ( nat i = 0; i < poly.size(); ++i ) glVertex2dv ( & poly[i].x );
    glEnd ();
}

void drawPolygon ( CCArrRef<Vector2d> & poly, float r, float g, float b )
{
    glColor3f ( r, g, b );
    glBegin ( GL_LINE_LOOP );
    for ( nat i = 0; i < poly.size(); ++i ) glVertex2dv ( & poly[i].x );
    glEnd ();
}

void drawPolygon ( CCArrRef<Vector3d> & poly, float r, float g, float b )
{
    glColor3f ( r, g, b );
    glBegin ( GL_LINE_LOOP );
    for ( nat i = 0; i < poly.size(); ++i ) glVertex3dv ( & poly[i].x );
    glEnd ();
}

void drawCross ( const Vector2d & p, float r, float g, float b )
{
    const double s = 0.02;
    draw ( Segment2d ( Vector2d(p.x-s, p.y), Vector2d(p.x + s, p.y) ), r, g, b );
    draw ( Segment2d ( Vector2d(p.x, p.y-s), Vector2d(p.x, p.y + s) ), r, g, b );
}

void drawPoint ( const Vector2d & point, float r, float g, float b )
{
    glEnable ( GL_POINT_SMOOTH );
    glPointSize ( 4 );
    glColor3f ( r, g, b );
    glBegin ( GL_POINTS );
    glVertex2dv ( & point.x );
    glEnd ();
}

void drawPoints ( CCArrRef<Vector2d> & point, float r, float g, float b )
{
    glEnable ( GL_POINT_SMOOTH );
    glPointSize ( 4 );
    glColor3f ( r, g, b );
    glBegin ( GL_POINTS );
    for ( nat i = 0; i < point.size(); ++i ) glVertex2dv ( & point[i].x );
    glEnd ();
}

void draw ( const Def<Rhombus2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    drawPolygon ( makePolygon ( fig, FixArray<Vector2d, 4>() ), r, g, b );
}

void draw ( const Def<Circle2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    drawPolygon ( regularPolygon ( FixArray<Vector2d, 100>(), fig.r ) += fig.o, r, g, b );
}

void draw ( const Def<Ellipse2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    drawPolygon ( regularPolygon ( FixArray<Vector2d, 100>(), 1 ) *= fig.getAffin2d(), r, g, b );
}

void draw ( const Def<Rectangle2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    drawPolygon ( makePolygon ( fig, FixArray<Vector2d, 4>() ), r, g, b );
}

void draw ( const Def<Parallelogram2d> & fig, float r, float g, float b )
{
    if ( ! fig.isDef ) return;
    drawPolygon ( makePolygon ( fig, FixArray<Vector2d, 4>() ), r, g, b );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawPoint ( const Def<Vector3d> & point, float r, float g, float b )
{
    glEnable ( GL_POINT_SMOOTH );
    glPointSize ( 4 );
    glColor3f ( r, g, b );
    glBegin ( GL_POINTS );
    glVertex3dv ( & point.x );
    glEnd ();
}

void drawPoints ( CCArrRef<Vector3d> & point, float r, float g, float b )
{
    glDisable ( GL_LIGHT0 );
    glDisable ( GL_LIGHTING );
    glDisable ( GL_DEPTH_TEST );
    glEnable ( GL_POINT_SMOOTH );
    glPointSize ( 4 );
    glColor3f ( r, g, b );
    glBegin ( GL_POINTS );
    for ( nat i = 0; i < point.size(); ++i ) glVertex3dv ( & point[i].x );
    glEnd ();
}

void draw ( const Def<Segment3d> & fig, float r, float g, float b )
{
    glEnable ( GL_DEPTH_TEST );
    glColor3f ( r, g, b );
    glBegin ( GL_LINES );
    glVertex3dv ( & fig.a.x );
    glVertex3dv ( & fig.b.x );
    glEnd ();
}

void draw ( const Def<Cuboid3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    draw ( Polyhedron ( fig ), r, g, b, a, vm == VM_SMOOTH ? VM_FLAT : vm );
}

static 
Poly3gon<Set2<Vector3d>, void> & copy ( const Polyhedron & poly1, Poly3gon<Set2<Vector3d>, void> & poly2 )
{
    poly2.vertex.resize ( poly1.vertex.size() );
    poly2.side.resize ( poly1.facet.size() );
    nat i;
    for ( i = 0; i < poly1.facet.size(); ++i )
    {
        CCArrRef<nat> & v = poly1.facet[i].index;
        Trigon<void> & t = poly2.side[i];
        t.a = v[0];
        t.b = v[1];
        t.c = v[2];
    }
    for ( i = 0; i < poly1.vertex.size(); ++i )
    {
        poly2.vertex[i].a = poly2.vertex[i].b = poly1.vertex[i];
    }
    return poly2;
}

static const Polyhedron & sphere1 ()
{
    static Polyhedron stat;
    if ( stat.vertex.size() == 0 ) 
    {
        stat.makeSphere ( 1 );
    }
    return stat;
}

static void draw ( const Poly3gon<Set2<Vector3d>, void> & poly, float r, float g, float b, float a )
{
    glEnable ( GL_LIGHT0 );
    glEnable ( GL_LIGHTING );
    glDisable( GL_CULL_FACE );
    glEnable ( GL_NORMALIZE );
    glEnable ( GL_DEPTH_TEST );
    glPolygonMode ( GL_FRONT, GL_FILL );

    float mat[4];
    mat[0] = 0.2f * r;
    mat[1] = 0.2f * g;
    mat[2] = 0.2f * b;
    mat[3] = a;
    glMaterialfv ( GL_FRONT, GL_AMBIENT, mat );
    mat[0] *= 4.0f;
    mat[1] *= 4.0f;
    mat[2] *= 4.0f;
    glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat );
    if ( a < 1 )
    {
        glEnable ( GL_BLEND );
        glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glEnable ( GL_CULL_FACE );
    }
    for ( nat i = 0; i < poly.side.size(); ++i )
    {
        const Trigon<void> & facet = poly.side[i];
        glBegin ( GL_TRIANGLES );
		glNormal3dv ( & poly.vertex[facet.a].b.x );
        glVertex3dv ( & poly.vertex[facet.a].a.x );
		glNormal3dv ( & poly.vertex[facet.b].b.x );
        glVertex3dv ( & poly.vertex[facet.b].a.x );
		glNormal3dv ( & poly.vertex[facet.c].b.x );
        glVertex3dv ( & poly.vertex[facet.c].a.x );
        glEnd ();
    }
    glDisable ( GL_BLEND );
}

void draw ( const Def<Sphere3d> & sphere, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! sphere.isDef ) return;
    Polyhedron poly;
    poly = sphere1();
    if ( vm != VM_SMOOTH )
    {
        for ( nat i = 0; i < poly.vertex.size(); ++i )
        {
            poly.vertex[i] = poly.vertex[i] * sphere.r + sphere.o;
        }
        draw ( poly, r, g, b, a, vm );
    }
    else
    {
        Poly3gon<Set2<Vector3d>, void> temp ( 0, 0 );
        copy ( poly, temp );
        for ( nat i = 0; i < temp.vertex.size(); ++i )
        {
            temp.vertex[i].a = temp.vertex[i].a * sphere.r + sphere.o;
        }
        draw ( temp, r, g, b, a );
    }
}

void draw ( const Def<Ellipsoid3d> & elli, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! elli.isDef ) return;
    Polyhedron poly;
    poly = sphere1();
    const Affin3d aff = elli.getAffin3d();
    const Vector3d u ( elli.a, elli.b, elli.c );
    if ( vm == VM_SMOOTH && ( u.x == 0 || u.y == 0 || u.z == 0 ) ) vm = VM_FLAT;
    if ( vm != VM_SMOOTH )
    {
        draw ( poly *= aff, r, g, b, a, vm );
    }
    else
    {
        Poly3gon<Set2<Vector3d>, void> temp ( 0, 0 );
        copy ( poly, temp );
        const LinTran3d lt ( elli.spin );
        for ( nat i = 0; i < temp.vertex.size(); ++i )
        {
            temp.vertex[i].a *= aff;
            ( temp.vertex[i].b /= u ).setNorm2() *= lt;
        }
        draw ( temp, r, g, b, a );
    }
}

void draw ( const Def<Cone3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    nat i;
    const nat nv = 80;
    const double s = 2 * M_PI / nv;
    DynArray<Vector3d> vert3d ( nv+1 );
    for ( i = 0; i < nv; ++i )
    {
        const double a = i * s;
        vert3d[i].x = fig.r * cos ( a );
        vert3d[i].y = fig.r * sin ( a );
        vert3d[i].z = - fig.h;
    }
    vert3d[nv] = Vector3d ( 0, 0, fig.h );
    glPushMatrix();
    glTranslated ( fig.o.x, fig.o.y, fig.o.z );
    const Vector3d axis = fig.spin.getAxis();
    glRotated ( fig.spin.getAngle() * RAD, axis.x, axis.y, axis.z );
    if ( vm == VM_WIRE )
    {
        glDisable ( GL_LIGHT0 );
        glDisable ( GL_LIGHTING );
        glEnable  ( GL_CULL_FACE );
        glDisable ( GL_NORMALIZE );
        glEnable  ( GL_DEPTH_TEST );
        glDepthFunc ( GL_LEQUAL );
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
        for ( nat k = 0; k < 2; ++k )
        {
            if ( k == 0 )
            {
                glEnable  ( GL_LINE_STIPPLE );
                glLineStipple ( 1, 0x2222 );
                glCullFace ( GL_FRONT );
                glColor4f ( 0.5f*r, 0.5f*g, 0.5f*b, a );
            }
            else
            {
                glDisable  ( GL_LINE_STIPPLE );
                glCullFace ( GL_BACK );
                glColor4f ( r, g, b, a );
                glBegin ( GL_POLYGON );
                for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv-1-i].x );
                glEnd ();
            }
            glBegin ( GL_TRIANGLES );
            for ( i = 0; i < nv; ++i )
            {
                glVertex3dv ( & vert3d[i].x );
                glVertex3dv ( & vert3d[nv].x );
                if ( i > 0 )
                    glVertex3dv ( & vert3d[i-1].x );
                else
                    glVertex3dv ( & vert3d[nv-1].x );
            }
            glEnd ();
        }
    }
    else
    {
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glDisable( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );

        float mat[4];
        mat[0] = 0.6f * r;
        mat[1] = 0.6f * g;
        mat[2] = 0.6f * b;
        mat[3] = a;
        glMaterialfv ( GL_FRONT, GL_AMBIENT, mat );
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat );
        glBegin ( GL_POLYGON );
		glNormal3f ( 0, 0, -1 );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv-1-i].x );
        glEnd ();
        glBegin ( GL_TRIANGLES );
        for ( i = 0; i < nv; ++i )
        {
            const nat j = i > 0 ? i - 1 : nv - 1;
		    const Vector3d n3 ( vert3d[i].x, vert3d[i].y, fig.h );
		    const Vector3d n2 ( vert3d[j].x, vert3d[j].y, fig.h );
		    const Vector3d n1 ( n3 + n3 );
		    glNormal3dv ( & n1.x );
            glVertex3dv ( & vert3d[nv].x );
		    if ( vm == VM_SMOOTH ) glNormal3dv ( & n2.x );
            glVertex3dv ( & vert3d[j].x );
		    if ( vm == VM_SMOOTH ) glNormal3dv ( & n3.x );
            glVertex3dv ( & vert3d[i].x );
        }
        glEnd ();
    }
    glPopMatrix();
}

void draw ( const Def<Cylinder3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    nat i;
    const nat nv = 40;
    const double s = 2 * M_PI / nv;
    DynArray<Vector3d> vert3d ( nv+nv );
    for ( i = 0; i < nv; ++i )
    {
        const double a = i * s;
        vert3d[i+nv].x = vert3d[i].x = fig.r * cos ( a );
        vert3d[i+nv].y = vert3d[i].y = fig.r * sin ( a );
        vert3d[i+nv].z = fig.h;
        vert3d[i].z = - fig.h;
    }
    glPushMatrix();
    glTranslated ( fig.o.x, fig.o.y, fig.o.z );
    const Vector3d axis = fig.spin.getAxis();
    glRotated ( fig.spin.getAngle() * RAD, axis.x, axis.y, axis.z );
    if ( vm == VM_WIRE || ! fig.r )
    {
        glDisable ( GL_LIGHT0 );
        glDisable ( GL_LIGHTING );
        glEnable  ( GL_CULL_FACE );
        glDisable ( GL_NORMALIZE );
        glEnable  ( GL_DEPTH_TEST );
        glDepthFunc ( GL_LEQUAL );
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
        for ( nat k = 0; k < 2; ++k )
        {
            if ( k == 0 )
            {
                glEnable  ( GL_LINE_STIPPLE );
                glLineStipple ( 1, 0x2222 );
                glCullFace ( GL_FRONT );
                glColor4f ( 0.5f*r, 0.5f*g, 0.5f*b, a );
            }
            else
            {
                glDisable  ( GL_LINE_STIPPLE );
                glCullFace ( GL_BACK );
                glColor4f ( r, g, b, a );
                glBegin ( GL_POLYGON );
                for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv-1-i].x );
                glEnd ();
                glBegin ( GL_POLYGON );
                for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv+i].x );
                glEnd ();
            }
            glBegin ( GL_QUADS );
            for ( i = 0; i < nv; ++i )
            {
                glVertex3dv ( & vert3d[i].x );
                glVertex3dv ( & vert3d[i+nv].x );
                if ( i > 0 )
                {
                    glVertex3dv ( & vert3d[i-1+nv].x );
                    glVertex3dv ( & vert3d[i-1].x );
                }
                else
                {
                    glVertex3dv ( & vert3d[nv-1+nv].x );
                    glVertex3dv ( & vert3d[nv-1].x );
                }
            }
            glEnd ();
        }
    }
    else
    {
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glDisable( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );

        float mat[4];
        mat[0] = 0.2f * r;
        mat[1] = 0.2f * g;
        mat[2] = 0.2f * b;
        mat[3] = a;
        glMaterialfv ( GL_FRONT, GL_AMBIENT, mat );
        mat[0] *= 4.0f;
        mat[1] *= 4.0f;
        mat[2] *= 4.0f;
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat );
        glBegin ( GL_POLYGON );
		glNormal3f ( 0, 0, -1 );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv-1-i].x );
        glEnd ();
        glBegin ( GL_POLYGON );
		glNormal3f ( 0, 0, 1 );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert3d[nv+i].x );
        glEnd ();
        glBegin ( GL_QUADS );
        for ( i = 0; i < nv; ++i )
        {
            const nat j = i > 0 ? i - 1 : nv - 1;
            if ( vm == VM_SMOOTH )
            {
		        glNormal3d ( vert3d[i].x, vert3d[i].y, 0 );
                glVertex3dv ( & vert3d[i].x );
                glVertex3dv ( & vert3d[i+nv].x );
		        glNormal3d ( vert3d[j].x, vert3d[j].y, 0 );
            }
            else
            {
		        glNormal3d ( vert3d[i].x+vert3d[j].x, vert3d[i].y+vert3d[j].y, 0 );
                glVertex3dv ( & vert3d[i].x );
                glVertex3dv ( & vert3d[i+nv].x );
            }
            glVertex3dv ( & vert3d[j+nv].x );
            glVertex3dv ( & vert3d[j].x );
        }
        glEnd ();
    }
    glPopMatrix();
}

static void draw ( nat nv, const Vector3d * vert, const Vector3d & norm, float r, float g, float b, float a, ViewMode vm )
{
    nat i;
    if ( vm == VM_WIRE )
    {
        glDisable ( GL_LIGHT0 );
        glDisable ( GL_LIGHTING );
        glDisable ( GL_CULL_FACE );
        glDisable ( GL_NORMALIZE );
        glEnable  ( GL_DEPTH_TEST );
        glDepthFunc ( GL_LEQUAL );
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
        glDisable  ( GL_LINE_STIPPLE );
        glColor4f ( r, g, b, a );
        glBegin ( GL_LINE_LOOP );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert[i].x );
        glEnd ();
    }
    else
    {
        glEnable ( GL_LIGHT0 );
        glEnable ( GL_LIGHTING );
        glEnable ( GL_CULL_FACE );
        glEnable ( GL_NORMALIZE );
        glEnable ( GL_DEPTH_TEST );
        glPolygonMode ( GL_FRONT, GL_FILL );
        float mat[4];
        mat[0] = 0.2f * r;
        mat[1] = 0.2f * g;
        mat[2] = 0.2f * b;
        mat[3] = a;
        glMaterialfv ( GL_FRONT, GL_AMBIENT, mat );
        mat[0] *= 4.0f;
        mat[1] *= 4.0f;
        mat[2] *= 4.0f;
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat );
        glBegin ( GL_POLYGON );
		glNormal3d ( -norm.x, -norm.y, -norm.z );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert[nv-1-i].x );
        glEnd ();
        glBegin ( GL_POLYGON );
		glNormal3d ( norm.x, norm.y, norm.z );
        for ( i = 0; i < nv; ++i ) glVertex3dv ( & vert[i].x );
        glEnd ();
    }
}

void drawPolygon ( nat nv, const Vector3d * vert, const Vector3d & o, const Spin3d & spin,
                   float r, float g, float b, float a, ViewMode vm )
{
    glPushMatrix();
    glTranslated ( o.x, o.y, o.z );
    const Vector3d axis = spin.getAxis();
    glRotated ( spin.getAngle() * RAD, axis.x, axis.y, axis.z );
    draw ( nv, vert, Vector3d (0,0,1), r, g, b, a, vm );
    glPopMatrix();
}

void draw ( const Def<Circle3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    const nat nv = 40;
    const double s = 2 * M_PI / nv;
    DynArray<Vector3d> vert ( nv );
    for ( nat i = 0; i < nv; ++i )
    {
        const double a = i * s;
        vert[i].x = fig.r * cos ( a );
        vert[i].y = fig.r * sin ( a );
        vert[i].z = 0;
    }
    drawPolygon ( nv, vert(), fig.o, fig.spin, r, g, b, a, vm );
}

void draw ( const Def<Ellipse3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    const nat nv = 40;
    const double s = 2 * M_PI / nv;
    DynArray<Vector3d> vert ( nv );
    for ( nat i = 0; i < nv; ++i )
    {
        const double a = i * s;
        vert[i].x = fig.a * cos ( a );
        vert[i].y = fig.b * sin ( a );
        vert[i].z = 0;
    }
    drawPolygon ( nv, vert(), fig.o, fig.spin, r, g, b, a, vm );
}

void draw ( const Def<Triangle3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    Vector3d vert[3];
    vert[0] = fig.a;
    vert[1] = fig.b;
    vert[2] = fig.c;
    draw ( 3, vert, ( fig.a - fig.b ) % ( fig.b - fig.c ), r, g, b, a, vm );
}

void draw ( const Def<Rectangle3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    Vector3d vert[4];
    fig.getVerts ( vert );
    draw ( 4, vert, fig.getNorm(), r, g, b, a, vm );
}

void draw ( const Def<Rhombus3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    Vector3d vert[4];
    fig.getVerts ( vert );
    draw ( 4, vert, fig.getNorm(), r, g, b, a, vm );
}

void draw ( const Def<Parallelogram3d> & fig, float r, float g, float b, float a, ViewMode vm )
{
    if ( ! fig.isDef ) return;
    Vector3d vert[4];
    fig.getVerts ( vert );
    draw ( 4, vert, fig.getNorm(), r, g, b, a, vm );
}
