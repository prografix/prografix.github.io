
#include "stdafx.h"
#include "gl\gl.h"
#include "gl\glu.h"
#include "math.h"
#include "time.h"
#include "GraphicInterface.h"
#include "GLDevice.h"
#include "PseudoFile.h"
#include "btpc/btpc.h"
#include "GrClock.h"

// Clear

void GLDevice::ClearColor ( float r, float g, float b, float a )
{
    glClearColor ( r, g, b, a );
}

void GLDevice::ClearColorBuffer  () { glClear ( GL_COLOR_BUFFER_BIT ); }
void GLDevice::ClearDepthBuffer  () { glClear ( GL_DEPTH_BUFFER_BIT ); }
void GLDevice::ClearStencilBuffer() { glClear (GL_STENCIL_BUFFER_BIT); }

// Enable

void GLDevice::EnableBlend         () { glEnable(GL_BLEND          ); }
void GLDevice::EnableColorMaterial () { glEnable(GL_COLOR_MATERIAL ); }
void GLDevice::EnableCullFace      () { glEnable(GL_CULL_FACE      ); }
void GLDevice::EnableDepthTest     () { glEnable(GL_DEPTH_TEST     ); }
void GLDevice::EnableLight  (Light i) { glEnable(GL_LIGHT0 + i     ); }
void GLDevice::EnableLighting      () { glEnable(GL_LIGHTING       ); }
void GLDevice::EnableLineStipple   () { glEnable(GL_LINE_STIPPLE   ); }
void GLDevice::EnableNormalize     () { glEnable(GL_NORMALIZE      ); }
void GLDevice::EnablePolygonStipple() { glEnable(GL_POLYGON_STIPPLE); }
void GLDevice::EnableStencilTest   () { glEnable(GL_STENCIL_TEST   ); }
void GLDevice::EnableTexture2D     () { glEnable(GL_TEXTURE_2D     ); }
void GLDevice::EnableTextureGenS   () { glEnable(GL_TEXTURE_GEN_S  ); }
void GLDevice::EnableTextureGenT   () { glEnable(GL_TEXTURE_GEN_T  ); }

// Disable

void GLDevice::DisableBlend         () { glDisable(GL_BLEND          ); }
void GLDevice::DisableColorMaterial () { glDisable(GL_COLOR_MATERIAL ); }
void GLDevice::DisableCullFace      () { glDisable(GL_CULL_FACE      ); }
void GLDevice::DisableDepthTest     () { glDisable(GL_DEPTH_TEST     ); }
void GLDevice::DisableLight  (Light i) { glDisable(GL_LIGHT0 + i     ); }
void GLDevice::DisableLighting      () { glDisable(GL_LIGHTING       ); }
void GLDevice::DisableLineStipple   () { glDisable(GL_LINE_STIPPLE   ); }
void GLDevice::DisableNormalize     () { glDisable(GL_NORMALIZE      ); }
void GLDevice::DisablePolygonStipple() { glDisable(GL_POLYGON_STIPPLE); }
void GLDevice::DisableStencilTest   () { glDisable(GL_STENCIL_TEST   ); }
void GLDevice::DisableTexture2D     () { glDisable(GL_TEXTURE_2D     ); }
void GLDevice::DisableTextureGenS   () { glDisable(GL_TEXTURE_GEN_S  ); }
void GLDevice::DisableTextureGenT   () { glDisable(GL_TEXTURE_GEN_T  ); }

// Blend func

void GLDevice::BlendFunc ( BlendFuncPar src, BlendFuncPar dst )
{
    int isrc;
    switch(src)
    {
    case BFP_ZERO:                isrc = GL_ZERO; break;
    case BFP_ONE:                 isrc = GL_ONE; break;
    case BFP_DST_COLOR:           isrc = GL_DST_COLOR; break;
    case BFP_ONE_MINUS_DST_COLOR: isrc = GL_ONE_MINUS_DST_COLOR; break;
    case BFP_SRC_ALPHA:           isrc = GL_SRC_ALPHA; break;
    case BFP_ONE_MINUS_SRC_ALPHA: isrc = GL_ONE_MINUS_SRC_ALPHA; break;
    case BFP_DST_ALPHA:           isrc = GL_DST_ALPHA; break;
    case BFP_ONE_MINUS_DST_ALPHA: isrc = GL_ONE_MINUS_DST_ALPHA; break;
    case BFP_SRC_ALPHA_SATURATE:  isrc = GL_SRC_ALPHA_SATURATE; break;
    }
    int idst;
    switch(dst)
    {
    case BFP_ZERO:                idst = GL_ZERO; break;
    case BFP_ONE:                 idst = GL_ONE; break;
    case BFP_DST_COLOR:           idst = GL_DST_COLOR; break;
    case BFP_ONE_MINUS_DST_COLOR: idst = GL_ONE_MINUS_DST_COLOR; break;
    case BFP_SRC_ALPHA:           idst = GL_SRC_ALPHA; break;
    case BFP_ONE_MINUS_SRC_ALPHA: idst = GL_ONE_MINUS_SRC_ALPHA; break;
    case BFP_DST_ALPHA:           idst = GL_DST_ALPHA; break;
    case BFP_ONE_MINUS_DST_ALPHA: idst = GL_ONE_MINUS_DST_ALPHA; break;
    case BFP_SRC_ALPHA_SATURATE:  idst = GL_SRC_ALPHA_SATURATE; break;
    }
    glBlendFunc(isrc, idst);
}

// Deph func

void GLDevice::DepthFuncNever   () { glDepthFunc(GL_NEVER   ); }
void GLDevice::DepthFuncLess    () { glDepthFunc(GL_LESS    ); }
void GLDevice::DepthFuncLEqual  () { glDepthFunc(GL_LEQUAL  ); }
void GLDevice::DepthFuncEqual   () { glDepthFunc(GL_EQUAL   ); }
void GLDevice::DepthFuncGreater () { glDepthFunc(GL_GREATER ); }
void GLDevice::DepthFuncNotEqual() { glDepthFunc(GL_NOTEQUAL); }
void GLDevice::DepthFuncGEqual  () { glDepthFunc(GL_GEQUAL  ); }
void GLDevice::DepthFuncAlways  () { glDepthFunc(GL_ALWAYS  ); }

// Stencil func

void GLDevice::StencilFuncNever   ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_NEVER, ref, mask );
}

void GLDevice::StencilFuncLess    ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_LESS, ref, mask );
}

void GLDevice::StencilFuncLEqual  ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_LEQUAL, ref, mask );
}

void GLDevice::StencilFuncEqual   ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_EQUAL, ref, mask );
}

void GLDevice::StencilFuncGreater ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_GREATER, ref, mask );
}

void GLDevice::StencilFuncNotEqual( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_NOTEQUAL, ref, mask );
}

void GLDevice::StencilFuncGEqual  ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_GEQUAL, ref, mask );
}

void GLDevice::StencilFuncAlways  ( GIuint ref, GIuint mask )
{
    glStencilFunc ( GL_ALWAYS, ref, mask );
}

void GLDevice::StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass )
{
    int i1;
    switch(fail)
    {
    case SOP_KEEP:    i1 = GL_KEEP; break;
    case SOP_ZERO:    i1 = GL_ONE; break;
    case SOP_REPLACE: i1 = GL_REPLACE; break;
    case SOP_INCR:    i1 = GL_INCR; break;
    case SOP_DECR:    i1 = GL_DECR; break;
    case SOP_INVERT:  i1 = GL_INVERT; break;
    }
    int i2;
    switch(zfail)
    {
    case SOP_KEEP:    i2 = GL_KEEP; break;
    case SOP_ZERO:    i2 = GL_ONE; break;
    case SOP_REPLACE: i2 = GL_REPLACE; break;
    case SOP_INCR:    i2 = GL_INCR; break;
    case SOP_DECR:    i2 = GL_DECR; break;
    case SOP_INVERT:  i2 = GL_INVERT; break;
    }
    int i3;
    switch(zpass)
    {
    case SOP_KEEP:    i3 = GL_KEEP; break;
    case SOP_ZERO:    i3 = GL_ONE; break;
    case SOP_REPLACE: i3 = GL_REPLACE; break;
    case SOP_INCR:    i3 = GL_INCR; break;
    case SOP_DECR:    i3 = GL_DECR; break;
    case SOP_INVERT:  i3 = GL_INVERT; break;
    }
    glStencilOp ( i1, i2, i3 );
}

// Begin, End

void GLDevice::BeginPoints       () { glBegin(GL_POINTS         ); }
void GLDevice::BeginLines        () { glBegin(GL_LINES          ); }
void GLDevice::BeginLineStrip    () { glBegin(GL_LINE_STRIP     ); }
void GLDevice::BeginLineLoop     () { glBegin(GL_LINE_LOOP      ); }
void GLDevice::BeginTriangles    () { glBegin(GL_TRIANGLES      ); }
void GLDevice::BeginTriangleStrip() { glBegin(GL_TRIANGLE_STRIP ); }
void GLDevice::BeginTriangleFan  () { glBegin(GL_TRIANGLE_FAN   ); }
void GLDevice::BeginQuads        () { glBegin(GL_QUADS          ); }
void GLDevice::BeginQuadStrip    () { glBegin(GL_QUAD_STRIP     ); }
void GLDevice::BeginPolygon      () { glBegin(GL_POLYGON        ); }
void GLDevice::End               () { glEnd(); }

// Cull face

void GLDevice::CullFaceBack        () { glCullFace(GL_BACK          ); }
void GLDevice::CullFaceFront       () { glCullFace(GL_FRONT         ); }
void GLDevice::CullFaceFrontAndBack() { glCullFace(GL_FRONT_AND_BACK); }

// Light

void GLDevice::LightAmbient ( Light id, float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glLightfv ( GL_LIGHT0 + id, GL_AMBIENT, data );
}

void GLDevice::LightDiffuse ( Light id, float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glLightfv ( GL_LIGHT0 + id, GL_DIFFUSE, data );
}

void GLDevice::LightSpecular ( Light id, float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glLightfv ( GL_LIGHT0 + id, GL_SPECULAR, data );
}

void GLDevice::LightPosition ( Light id, float x, float y, float z, float w )
{
    float data[4];
    data[0] = x;    data[1] = y;
    data[2] = z;    data[3] = w;
    glLightfv ( GL_LIGHT0 + id, GL_POSITION, data );
}

void GLDevice::LightPosition4v ( Light id, GIuint index )
{
    glLightfv ( GL_LIGHT0 + id, GL_POSITION, floatValue + index );
}

void GLDevice::LightQuadraticAttenuation ( Light id, float f )
{
    glLightf ( GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, f );
}

// Stipple

void GLDevice::LineStipple ( int factor, int pattern )
{
    glLineStipple ( factor, pattern );
}

void GLDevice::PolygonStipple ( const unsigned char * mask  )
{
    glPolygonStipple ( mask );
}

// Material

void GLDevice::MaterialBackAmbient  ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_BACK, GL_AMBIENT, data );
}

void GLDevice::MaterialBackDiffuse  ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_BACK, GL_DIFFUSE, data );
}

void GLDevice::MaterialBackEmission ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_BACK, GL_EMISSION, data );
}

void GLDevice::MaterialBackSpecular ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_BACK, GL_SPECULAR, data );
}

void GLDevice::MaterialBackShininess ( float s )
{
    glMaterialf ( GL_BACK, GL_SHININESS, s );
}

void GLDevice::MaterialFrontAmbient  ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_FRONT, GL_AMBIENT, data );
}

void GLDevice::MaterialFrontDiffuse  ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_FRONT, GL_DIFFUSE, data );
}

void GLDevice::MaterialFrontEmission ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_FRONT, GL_EMISSION, data );
}

void GLDevice::MaterialFrontSpecular ( float r, float g, float b, float a )
{
    float data[4];
    data[0] = r;    data[1] = g;
    data[2] = b;    data[3] = a;
    glMaterialfv ( GL_FRONT, GL_SPECULAR, data );
}

void GLDevice::MaterialFrontShininess ( float s )
{
    glMaterialf ( GL_FRONT, GL_SHININESS, s );
}

// Polygon mode

void GLDevice::PolygonMode ( Face face, enum PolygonMode mode )
{
    int iface;
    switch(face)
    {
    case FACE_FRONT         : iface = GL_FRONT         ; break;
    case FACE_BACK          : iface = GL_BACK          ; break;
    case FACE_FRONT_AND_BACK: iface = GL_FRONT_AND_BACK; break;
    }
    int imode;
    switch(mode)
    {
    case PM_POINT: imode = GL_POINT; break;
    case PM_LINE : imode = GL_LINE ; break;
    case PM_FILL : imode = GL_FILL ; break;
    }
    glPolygonMode ( iface, imode );
}

// Color

void GLDevice::Color ( float r, float g, float b )
{
    glColor3f ( r, g, b );
}

void GLDevice::Color ( float r, float g, float b, float a )
{
    glColor4f ( r, g, b, a );
}

void GLDevice::Color3v ( GIuint index )
{
    glColor3f ( floatValue[index], floatValue[index+1], floatValue[index+2] );
}

void GLDevice::Color4v ( GIuint index )
{
    glColor4f ( floatValue[index], floatValue[index+1], floatValue[index+2], floatValue[index+3] );
}

// Normal

void GLDevice::Normal ( float x, float y, float z ) { glNormal3f ( x, y, z ); }

void GLDevice::Normal3vI ( GIuint index )
{
    int i = intValue[index];
    glNormal3f ( floatValue[i], floatValue[i+1], floatValue[i+2] );
}

// Vertex

void GLDevice::Vertex ( float x, float y          ) { glVertex2f ( x, y    ); }
void GLDevice::Vertex ( float x, float y, float z ) { glVertex3f ( x, y, z ); }

void GLDevice::Vertex2v ( GIuint index )
{
    glVertex2f ( floatValue[index], floatValue[index+1] );
}

void GLDevice::Vertex3v ( GIuint index )
{
    glVertex3f ( floatValue[index], floatValue[index+1], floatValue[index+2] );
}

void GLDevice::Vertex2vI ( GIuint index )
{
    int i = intValue[index];
    glVertex2f ( floatValue[i], floatValue[i+1] );
}

void GLDevice::Vertex3vI ( GIuint index )
{
    int i = intValue[index];
    glVertex3f ( floatValue[i], floatValue[i+1], floatValue[i+2] );
}

// Display list

void GLDevice::CallList      ( int list     ) { glCallList ( list            ); }
void GLDevice::CallListIndex ( GIuint index ) { glCallList ( intValue[index] ); }
void GLDevice::EndList () { glEndList(); }

void GLDevice::NewListCompile ( int list )
{
    glNewList ( list, GL_COMPILE );
}

void GLDevice::NewListCompileAndExecute ( int list )
{
    glNewList ( list, GL_COMPILE_AND_EXECUTE);
}

// Matrix

void GLDevice::Frustum4v ( GIuint i )
{
    glFrustum ( floatValue[i  ], floatValue[i+1], floatValue[i+2],
	            floatValue[i+3], floatValue[i+4], floatValue[i+5] );
}

void GLDevice::LoadIdentity() { glLoadIdentity(); }

void GLDevice::LookAt ( GIuint i )
{
    gluLookAt ( floatValue[i  ], floatValue[i+1], floatValue[i+2],
	            floatValue[i+3], floatValue[i+4], floatValue[i+5],
	            floatValue[i+6], floatValue[i+7], floatValue[i+8] );
}

void GLDevice::MatrixModeModelView () { glMatrixMode ( GL_MODELVIEW  ); }
void GLDevice::MatrixModeProjection() { glMatrixMode ( GL_PROJECTION ); }
void GLDevice::MatrixModeTexture   () { glMatrixMode ( GL_TEXTURE    ); }
void GLDevice::MultMatrix ( GIuint index ) { glMultMatrixf ( floatValue+index ); }
void GLDevice::PopMatrix () { glPopMatrix (); }
void GLDevice::PushMatrix() { glPushMatrix(); }

void GLDevice::Perspective ( float fovy, float aspect, float zNear, float zFar )
{
    gluPerspective ( fovy, aspect, zNear, zFar );
}

void GLDevice::Perspective4v ( GIuint index )
{
    gluPerspective ( floatValue[index  ], floatValue[index+1],
	                 floatValue[index+2], floatValue[index+3] );
}

void GLDevice::Rotate ( float angle, float x, float y, float z )
{
    glRotatef ( angle, x, y, z );
}

void GLDevice::Rotate4v ( GIuint index )
{
    glRotatef ( floatValue[index  ], floatValue[index+1],
                floatValue[index+2], floatValue[index+3] );
}

void GLDevice::Scale ( float x, float y, float z )
{
    glScalef ( x, y, z );
}

void GLDevice::Scale3v ( GIuint index )
{
    glScalef ( floatValue[index], floatValue[index+1], floatValue[index+2] );
}

void GLDevice::Translate ( float x, float y, float z )
{
    glTranslatef ( x, y, z );
}

void GLDevice::Translate3v ( GIuint index )
{
    glTranslatef ( floatValue[index], floatValue[index+1], floatValue[index+2] );
}

// Texture

void GLDevice::BindTexture2D ( GIuint index )
{
    glBindTexture ( GL_TEXTURE_2D, intValue[index] );
}

void GLDevice::Build2DMipmapsBGR ( int width, int height, const unsigned char * pData )
{
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pData);
}

void GLDevice::Build2DMipmapsBGR_BTPC ( int length, const unsigned char * pData )
{
    int nx, ny, bpp;
    unsigned char * data = 0;
    PseudoReadSeekFile pf ( CArrRef<bit8> ( (const bit8*) pData, length ) );
    if ( dbtpc ( pf, nx, ny, data, bpp ) ) Build2DMipmapsBGR ( nx, ny, data );
    delete[] data;
}

void GLDevice::GenTextures ( GIuint n, GIuint index )
{
    glGenTextures ( n, ( GIuint * ) intValue + index );
}

void GLDevice::TexCoord2 ( float s, float t ) { glTexCoord2f ( s, t ); }

void GLDevice::TexParameter2DMinFilterLinearMipmapNearest()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void GLDevice::TexParameter2DWrapSRepeat()
{
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
}

void GLDevice::TexParameter2DWrapTRepeat()
{
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

// Get

void GLDevice::GetFloatModelViewMatrix ( GIuint i )
{
    if ( i + 16 > floatValueSize ) resizeFloatValue ( i + 16 );
    glGetFloatv ( GL_MODELVIEW_MATRIX, floatValue + i );
}

void GLDevice::GetFloatProjectionMatrix ( GIuint i )
{
    if ( i + 16 > floatValueSize ) resizeFloatValue ( i + 16 );
    glGetFloatv ( GL_PROJECTION_MATRIX, floatValue + i );
}

void GLDevice::GetFloatViewport ( GIuint i )
{
    if ( i + 4 > floatValueSize ) resizeFloatValue ( i + 4 );
    glGetFloatv ( GL_VIEWPORT, floatValue + i );
}

void GLDevice::GetIntViewport ( GIuint i )
{
    if ( i + 4 > intValueSize ) resizeIntValue ( i + 4 );
    glGetIntegerv ( GL_VIEWPORT, intValue + i );
}

// Quadric objects

void GLDevice::QuadricTexture ( bool x )
{
    gluQuadricTexture ( quadObj, x );
}

void GLDevice::Disk ( float innerRadius, float outerRadius, int slices, int loops )
{
    gluDisk ( quadObj, innerRadius, outerRadius, slices, loops );
}

void GLDevice::Sphere ( float radius, int slices, int stacks )
{
    gluSphere ( quadObj, radius, slices, stacks );
}

// Integer commands

void GLDevice::IntAstDA ( GIuint index, int n, const int * p )
{
    if ( index + n > intValueSize ) resizeIntValue ( index+n );
    for ( int i = n; --i >= 0; ) intValue[index+i] = p[i];
}

void GLDevice::IntAstDC ( GIuint index, int i )
{ 
    if ( index >= intValueSize ) resizeIntValue ( index+1 );
    intValue[index] = i;
}

void GLDevice::IntAstDD ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= intValueSize ) resizeIntValue ( index1+1 );
    intValue[index1] = intValue[index2];
}

void GLDevice::IntAstDI ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= intValueSize ) resizeIntValue ( index1+1 );
    intValue[index1] = intValue[intValue[index2]];
}

void GLDevice::IntAddDC ( GIuint index, int i ) { intValue[index] += i; }
void GLDevice::IntAddDD ( GIuint index1, GIuint index2 ) { intValue[index1] += intValue[index2]; }

void GLDevice::IntSubDC ( GIuint index, int i ) { intValue[index] -= i; }
void GLDevice::IntSubDD ( GIuint index1, GIuint index2 ) { intValue[index1] -= intValue[index2]; }

void GLDevice::IntMulDC ( GIuint index, int i ) { intValue[index] *= i; }
void GLDevice::IntMulDD ( GIuint index1, GIuint index2 ) { intValue[index1] *= intValue[index2]; }

void GLDevice::IntDivDC ( GIuint index, int i ) { intValue[index] /= i; }
void GLDevice::IntDivDD ( GIuint index1, GIuint index2 ) { intValue[index1] /= intValue[index2]; }

void GLDevice::IntModDC ( GIuint index, int i ) { intValue[index] %= i; }
void GLDevice::IntModDD ( GIuint index1, GIuint index2 ) { intValue[index1] %= intValue[index2]; }

// Float commands

void GLDevice::FloatAstDA ( GIuint index, int n, const float * p )
{
    if ( index + n > floatValueSize ) resizeFloatValue ( index+n );
    for ( int i = n; --i >= 0; ) floatValue[index+i] = p[i];
}

void GLDevice::FloatAstDC ( GIuint index, float f )
{ 
    if ( index >= floatValueSize ) resizeFloatValue ( index+1 );
    floatValue[index] = f;
}

void GLDevice::FloatAstDD ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= floatValueSize ) resizeFloatValue ( index1+1 );
    floatValue[index1] = floatValue[index2];
}

void GLDevice::FloatAstDI ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= floatValueSize ) resizeFloatValue ( index1+1 );
    floatValue[index1] = floatValue[intValue[index2]];
}

void GLDevice::FloatAstIC ( GIuint index, float f )
{ 
    GIuint i = intValue[index];
    if ( i >= floatValueSize ) resizeFloatValue ( i+1 );
    floatValue[i] = f;
}

void GLDevice::FloatAstID ( GIuint index1, GIuint index2 )
{ 
    GIuint i = intValue[index1];
    if ( i >= floatValueSize ) resizeFloatValue ( i+1 );
    floatValue[i] = floatValue[index2];
}

void GLDevice::FloatAstII ( GIuint index1, GIuint index2 )
{ 
    GIuint i = intValue[index1];
    if ( i >= floatValueSize ) resizeFloatValue ( i+1 );
    floatValue[i] = floatValue[intValue[index2]];
}

void GLDevice::FloatAddDC ( GIuint index, float f ) { floatValue[index] += f; }
void GLDevice::FloatAddDD ( GIuint index1, GIuint index2 ) { floatValue[index1] += floatValue[index2]; }
void GLDevice::FloatAddDI ( GIuint index1, GIuint index2 ) { floatValue[index1] += floatValue[intValue[index2]]; }
void GLDevice::FloatAddIC ( GIuint index, float f ) { floatValue[intValue[index]] += f; }
void GLDevice::FloatAddID ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] += floatValue[index2]; }
void GLDevice::FloatAddII ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] += floatValue[intValue[index2]]; }

void GLDevice::FloatSubDC ( GIuint index, float f ) { floatValue[index] -= f; }
void GLDevice::FloatSubDD ( GIuint index1, GIuint index2 ) { floatValue[index1] -= floatValue[index2]; }
void GLDevice::FloatSubDI ( GIuint index1, GIuint index2 ) { floatValue[index1] -= floatValue[intValue[index2]]; }
void GLDevice::FloatSubIC ( GIuint index, float f ) { floatValue[intValue[index]] -= f; }
void GLDevice::FloatSubID ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] -= floatValue[index2]; }
void GLDevice::FloatSubII ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] -= floatValue[intValue[index2]]; }

void GLDevice::FloatMulDC ( GIuint index, float f ) { floatValue[index] *= f; }
void GLDevice::FloatMulDD ( GIuint index1, GIuint index2 ) { floatValue[index1] *= floatValue[index2]; }
void GLDevice::FloatMulDI ( GIuint index1, GIuint index2 ) { floatValue[index1] *= floatValue[intValue[index2]]; }
void GLDevice::FloatMulIC ( GIuint index, float f ) { floatValue[intValue[index]] *= f; }
void GLDevice::FloatMulID ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] *= floatValue[index2]; }
void GLDevice::FloatMulII ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] *= floatValue[intValue[index2]]; }

void GLDevice::FloatDivDC ( GIuint index, float f ) { floatValue[index] /= f; }
void GLDevice::FloatDivDD ( GIuint index1, GIuint index2 ) { floatValue[index1] /= floatValue[index2]; }
void GLDevice::FloatDivDI ( GIuint index1, GIuint index2 ) { floatValue[index1] /= floatValue[intValue[index2]]; }
void GLDevice::FloatDivIC ( GIuint index, float f ) { floatValue[intValue[index]] /= f; }
void GLDevice::FloatDivID ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] /= floatValue[index2]; }
void GLDevice::FloatDivII ( GIuint index1, GIuint index2 ) { floatValue[intValue[index1]] /= floatValue[intValue[index2]]; }

// Mix commands

void GLDevice::IntAstFromFloat ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= intValueSize ) resizeIntValue ( index1+1 );
    intValue[index1] = int(floatValue[index2]);
}

void GLDevice::FloatAstFromInt ( GIuint index1, GIuint index2 )
{ 
    if ( index1 >= floatValueSize ) resizeFloatValue ( index1+1 );
    floatValue[index1] = float(intValue[index2]);
}

// Conditional commands

bool GLDevice::IfIntDC ( GIuint index1, Relation rel, int i )
{
    bool ok = false;
    switch ( rel )
    {
        case EQ: ok = intValue[index1] == i; break;
        case NE: ok = intValue[index1] != i; break;
        case LT: ok = intValue[index1] <  i; break;
        case LE: ok = intValue[index1] <= i; break;
        case GT: ok = intValue[index1] >  i; break;
        case GE: ok = intValue[index1] >= i; break;
    }
    return ok;
}

bool GLDevice::IfIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    bool ok = false;
    switch ( rel )
    {
        case EQ: ok = intValue[index1] == intValue[index2]; break;
        case NE: ok = intValue[index1] != intValue[index2]; break;
        case LT: ok = intValue[index1] <  intValue[index2]; break;
        case LE: ok = intValue[index1] <= intValue[index2]; break;
        case GT: ok = intValue[index1] >  intValue[index2]; break;
        case GE: ok = intValue[index1] >= intValue[index2]; break;
    }
    return ok;
}

bool GLDevice::IfFloatDC ( GIuint index1, Relation rel, float f )
{
    bool ok = false;
    switch ( rel )
    {
        case EQ: ok = floatValue[index1] == f; break;
        case NE: ok = floatValue[index1] != f; break;
        case LT: ok = floatValue[index1] <  f; break;
        case LE: ok = floatValue[index1] <= f; break;
        case GT: ok = floatValue[index1] >  f; break;
        case GE: ok = floatValue[index1] >= f; break;
    }
    return ok;
}

bool GLDevice::IfFloatDD ( GIuint index1, Relation rel, GIuint index2 )
{
    bool ok = false;
    switch ( rel )
    {
        case EQ: ok = floatValue[index1] == floatValue[index2]; break;
        case NE: ok = floatValue[index1] != floatValue[index2]; break;
        case LT: ok = floatValue[index1] <  floatValue[index2]; break;
        case LE: ok = floatValue[index1] <= floatValue[index2]; break;
        case GT: ok = floatValue[index1] >  floatValue[index2]; break;
        case GE: ok = floatValue[index1] >= floatValue[index2]; break;
    }
    return ok;
}

bool GLDevice::Else () { return 0; }
void GLDevice::EndIf () {}

// Loop

void GLDevice::Do () {}

bool GLDevice::WhileIntDC ( GIuint index1, Relation rel, int i )
{
    switch ( rel )
    {
    case EQ: return intValue[index1] == i;
    case NE: return intValue[index1] != i;
    case LT: return intValue[index1] <  i;
    case LE: return intValue[index1] <= i;
    case GT: return intValue[index1] >  i;
    case GE: return intValue[index1] >= i;
    }
    return false;
}

bool GLDevice::WhileIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    switch ( rel )
    {
    case EQ: return intValue[index1] == intValue[index2];
    case NE: return intValue[index1] != intValue[index2];
    case LT: return intValue[index1] <  intValue[index2];
    case LE: return intValue[index1] <= intValue[index2];
    case GT: return intValue[index1] >  intValue[index2];
    case GE: return intValue[index1] >= intValue[index2];
    }
    return false;
}

// Mathematics

void GLDevice::FloatAbs ( GIuint index ) { floatValue[index] = fabsf(floatValue[index]); }
void GLDevice::FloatAtan2 ( GIuint index1, GIuint index2 ) { floatValue[index1] = atan2f(floatValue[index1],floatValue[index2]); }
void GLDevice::FloatCos  ( GIuint index ) { floatValue[index] = cosf (floatValue[index]); }
void GLDevice::FloatSin  ( GIuint index ) { floatValue[index] = sinf (floatValue[index]); }
void GLDevice::FloatSqrt ( GIuint index ) { floatValue[index] = sqrtf(floatValue[index]); }

// Start position

void GLDevice::NextStart ( const Node * node ) { nextStart = node; }
bool GLDevice::NextStartAndReturn ( const Node * node ) { nextStart = node; return 1; }
void GLDevice::NextStartFirst () { nextStart = 0; }
const GraphicInterface::Node * GLDevice::getNextStart() const { return nextStart; }

// Time

void GLDevice::Clock ( GIuint index )
{
    if ( index >= intValueSize ) resizeIntValue ( index + 1 );
    intValue[index] = grClock.get();
}

void GLDevice::Time ( GIuint index )
{
    if ( index + 3 > intValueSize ) resizeIntValue ( index + 3 );
    time_t ltime;
    time( &ltime );
    struct tm * p = localtime ( &ltime );
    intValue[index++] = p->tm_hour;
    intValue[index++] = p->tm_min;
    intValue[index  ] = p->tm_sec;
}

void SetTimer(int);

void GLDevice::SetTimer ( int elapse ) { ::SetTimer ( elapse ); }

// Constructor & destructor

GLDevice::GLDevice () : floatValue(0), floatValueSize(0),
intValue(0), intValueSize(0), nextStart(0)
{
    quadObj = gluNewQuadric();
}

GLDevice::~GLDevice ()
{
    delete[] intValue;
    delete[] floatValue;
    gluDeleteQuadric(quadObj);
}

void GLDevice::resizeFloatValue ( GIuint n )
{
    if ( floatValueSize > 0 )
    {
        GIuint k = floatValueSize;
        while ( n > k ) k *= 2;
        int * p = new int[k];
        for ( int i = floatValueSize; --i >= 0; )
        {
            p[i] = ((int*)floatValue)[i];
        }
        delete[] floatValue;
        floatValue = (float*) p;
        floatValueSize = k;
    }
    else
    {
        floatValueSize = 16;
        while ( n > floatValueSize ) floatValueSize *= 2;
        floatValue = new float[floatValueSize];
    }
}

void GLDevice::resizeIntValue ( GIuint n )
{
    if ( intValueSize > 0 )
    {
        GIuint k = intValueSize;
        while ( n > k ) k *= 2;
        int * p = new int[k];
        for ( int i = intValueSize; --i >= 0; )
        {
            p[i] = intValue[i];
        }
        delete[] intValue;
        intValue = p;
        intValueSize = k;
    }
    else
    {
        intValueSize = 16;
        while ( n > intValueSize ) intValueSize *= 2;
        intValue = new int[intValueSize];
    }
}
