
#pragma once

#include "ShevArray.h"
#include "Vector2d.h"

void drawNewList2d ();
void endNewList ();

void drawCross ( const Vector2d         & fig, float r, float g, float b );
void drawPoint ( const Vector2d         & fig, float r, float g, float b );
void drawPoints  ( CCArrRef<Vector2d>   & fig, float r, float g, float b );
void drawLine    ( CCArrRef<Vector2d>   & fig, float r, float g, float b );
void drawPolygon ( CCArrRef<Vector2d>   & fig, float r, float g, float b );
void draw ( const Def<Segment2d>        & fig, float r, float g, float b );
void draw ( const Def<Circle2d>         & fig, float r, float g, float b );
void draw ( const Def<Rhombus2d>        & fig, float r, float g, float b );
void draw ( const Def<Ellipse2d>        & fig, float r, float g, float b );
void draw ( const Def<Triangle2d>       & fig, float r, float g, float b );
void draw ( const Def<Rectangle2d>      & fig, float r, float g, float b );
void draw ( const Def<Parallelogram2d>  & fig, float r, float g, float b );

#include "Vector3d.h"

namespace Shev
{
    class Polyhedron;
}

enum ViewMode
{
    VM_WIRE,
    VM_FLAT,
    VM_SMOOTH
};

void drawPoints  ( CCArrRef<Vector3d>   & fig, float r, float g, float b );
void drawPolygon ( CCArrRef<Vector3d>   & fig, float r, float g, float b );
void drawPoint ( const Def<Vector3d>    & fig, float r, float g, float b );
void draw ( const Def<Segment3d>        & fig, float r, float g, float b );
void draw ( const Def<Circle3d>         & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Ellipse3d>        & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Triangle3d>       & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Rectangle3d>      & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Rhombus3d>        & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Parallelogram3d>  & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Shev::Polyhedron      & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Tetrahedron>      & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Cuboid3d>         & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Ellipsoid3d>      & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Cylinder3d>       & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Sphere3d>         & fig, float r, float g, float b, float a, ViewMode vm );
void draw ( const Def<Cone3d>           & fig, float r, float g, float b, float a, ViewMode vm );
