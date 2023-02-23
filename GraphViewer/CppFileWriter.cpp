#include "stdio.h"
#include <fcntl.h>
#include <sys/stat.h>
#include "GraphicInterface.h"
#include "CppFileWriter.h"
#include "PseudoFile.h"

CppFileWriter::CppFileWriter ( const char * filename )
{
    nextStartCount = 0;
    intSize = floatSize = intArrCount = floatArrCount = 0;
    tmp = new PseudoWriteFile ( buf );
    file = fopen ( filename, "w" );
    fprintf ( file, "#include \"ShevArray.h\"\n" );
    fprintf ( file, "#include \"PseudoFile.h\"\n" );
    fprintf ( file, "#include \"stdafx.h\"\n" );
    fprintf ( file, "#include \"math.h\"\n" );
    fprintf ( file, "#include \"gl\\gl.h\"\n" );
    fprintf ( file, "#include \"gl\\glu.h\"\n" );
    fprintf ( file, "#include \"btpc/btpc.h\"\n\n" );
    fprintf ( file, "extern void SetTimer(int uElapse);\n\n" );
    fprintf ( file, "%s", "void gra_function ()\n{\n"
                          "\tstatic nat nextStart = 0;\n"
                          "\tstatic Suite<int> intValue;\n"
                          "\tstatic Suite<float> floatValue;\n"
                          "\tstatic GLUquadric * quadObj = gluNewQuadric();\n"
                          "\tgoto start;\nm0:\n" );
}

CppFileWriter::~CppFileWriter ()
{
    fprintf ( file, "\treturn;\nstart:\n" );
    if ( intSize > 0 )
    {
        fprintf ( file, "\tif(intValue.size() == 0) intValue.resize(%d);\n", intSize );
    }
    if ( floatSize > 0 )
    {
        fprintf ( file, "\tif(floatValue.size() == 0) floatValue.resize(%d);\n", floatSize );
    }
    if ( nextStartCount == 0 )
    {
        fprintf ( file, "\tgoto m0;\n" );
    }
    else
    {
        fprintf ( file, "\tswitch(nextStart)\n\t{\n" );
        for ( nat i = 0; i <= nextStartCount; ++i )
        {
            fprintf ( file, "\t\tcase %d: goto m%d;\n", i, i );
        }
        fprintf ( file, "\t}\n" );
    }
    fprintf ( file, "}\n\n" );
    fwrite ( &buf[0], 1, tmp->tell(), file );
    fclose ( file );
    delete tmp;
}

static void putIntArray ( IWriteFile & tmp, int count, int n, const int * p )
{
    writeStr ( tmp, "int intArr" );
    writeIntDec ( tmp, (int32) count );
    tmp.putc ( "[" );
    writeIntDec ( tmp, (int32) n );
    writeStr ( tmp, "] = {" );
    writeIntDec ( tmp, (int32) *p );
//    fprintf ( tmp, "int intArr%d[%d] = {%d", count, n, *p );
    for ( int i = 1; i < n; ++i )
    {
        tmp.putc ( "," );
        writeIntDec ( tmp, (int32) p[i] );
//        fprintf ( tmp, ",%d", p[i] );
        if ( i % 20 == 19 ) tmp.putc ( "\n" ); // fprintf ( tmp, "\n" );
    }
    writeStr ( tmp, "};\n\n" );
}

// Clear

void CppFileWriter::ClearColor ( float r, float g, float b, float a )
{
    fprintf ( file, "\tglClearColor(%+.9ef, %+.9ef, %+.9ef, %+.9ef);\n", r, g, b, a );
}

void CppFileWriter::ClearColorBuffer ()
{
    fprintf ( file, "\tglClear(GL_COLOR_BUFFER_BIT);\n" );
}

void CppFileWriter::ClearDepthBuffer ()
{
    fprintf ( file, "\tglClear(GL_DEPTH_BUFFER_BIT);\n" );
}

void CppFileWriter::ClearStencilBuffer ()
{
    fprintf ( file, "\tglClear(GL_STENCIL_BUFFER_BIT);\n" );
}

// Enable

void CppFileWriter::EnableBlend ()
{
    fprintf ( file, "\tglEnable(GL_BLEND);\n" );
}

void CppFileWriter::EnableColorMaterial ()
{
    fprintf ( file, "\tglEnable(GL_COLOR_MATERIAL);\n" );
}

void CppFileWriter::EnableCullFace ()
{
    fprintf ( file, "\tglEnable(GL_CULL_FACE);\n" );
}

void CppFileWriter::EnableDepthTest ()
{
    fprintf ( file, "\tglEnable(GL_DEPTH_TEST);\n" );
}

void CppFileWriter::EnableLight ( Light l )
{
    fprintf ( file, "\tglEnable(GL_LIGHT%d);\n", l );
}

void CppFileWriter::EnableLighting ()
{
    fprintf ( file, "\tglEnable(GL_LIGHTING);\n" );
}

void CppFileWriter::EnableLineStipple ()
{
    fprintf ( file, "\tglEnable(GL_LINE_STIPPLE);\n" );
}

void CppFileWriter::EnableNormalize ()
{
    fprintf ( file, "\tglEnable(GL_NORMALIZE);\n" );
}

void CppFileWriter::EnablePolygonStipple ()
{
    fprintf ( file, "\tglEnable(GL_POLYGON_STIPPLE);\n" );
}

void CppFileWriter::EnableStencilTest ()
{
    fprintf ( file, "\tglEnable(GL_STENCIL_TEST);\n" );
}

void CppFileWriter::EnableTexture2D ()
{
    fprintf ( file, "\tglEnable(GL_TEXTURE_2D);\n" );
}

void CppFileWriter::EnableTextureGenS ()
{
    fprintf ( file, "\tglEnable(GL_TEXTURE_GEN_S);\n" );
}

void CppFileWriter::EnableTextureGenT ()
{
    fprintf ( file, "\tglEnable(GL_TEXTURE_GEN_T);\n" );
}

// Disable

void CppFileWriter::DisableBlend ()
{
    fprintf ( file, "\tglDisable(GL_BLEND);\n" );
}

void CppFileWriter::DisableColorMaterial ()
{
    fprintf ( file, "\tglDisable(GL_COLOR_MATERIAL);\n" );
}

void CppFileWriter::DisableCullFace ()
{
    fprintf ( file, "\tglDisable(GL_CULL_FACE);\n" );
}

void CppFileWriter::DisableDepthTest ()
{
    fprintf ( file, "\tglDisable(GL_DEPTH_TEST);\n" );
}

void CppFileWriter::DisableLight ( Light l )
{
    fprintf ( file, "\tglDisable(GL_LIGHT%d);\n", l );
}

void CppFileWriter::DisableLighting ()
{
    fprintf ( file, "\tglDisable(GL_LIGHTING);\n" );
}

void CppFileWriter::DisableLineStipple ()
{
    fprintf ( file, "\tglDisable(GL_LINE_STIPPLE);\n" );
}

void CppFileWriter::DisableNormalize ()
{
    fprintf ( file, "\tglDisable(GL_NORMALIZE);\n" );
}

void CppFileWriter::DisablePolygonStipple ()
{
    fprintf ( file, "\tglDisable(GL_POLYGON_STIPPLE);\n" );
}

void CppFileWriter::DisableStencilTest ()
{
    fprintf ( file, "\tglDisable(GL_STENCIL_TEST);\n" );
}

void CppFileWriter::DisableTexture2D ()
{
    fprintf ( file, "\tglDisable(GL_TEXTURE_2D);\n" );
}

void CppFileWriter::DisableTextureGenS ()
{
    fprintf ( file, "\tglDisable(GL_TEXTURE_GEN_S);\n" );
}

void CppFileWriter::DisableTextureGenT ()
{
    fprintf ( file, "\tglDisable(GL_TEXTURE_GEN_T);\n" );
}

// Blend func

void CppFileWriter::BlendFunc ( BlendFuncPar src, BlendFuncPar dst )
{
    const char * psrc = "";
    switch(src)
    {
    case BFP_ZERO:                psrc = "GL_ZERO"; break;
    case BFP_ONE:                 psrc = "GL_ONE"; break;
    case BFP_DST_COLOR:           psrc = "GL_DST_COLOR"; break;
    case BFP_ONE_MINUS_DST_COLOR: psrc = "GL_ONE_MINUS_DST_COLOR"; break;
    case BFP_SRC_ALPHA:           psrc = "GL_SRC_ALPHA"; break;
    case BFP_ONE_MINUS_SRC_ALPHA: psrc = "GL_ONE_MINUS_SRC_ALPHA"; break;
    case BFP_DST_ALPHA:           psrc = "GL_DST_ALPHA"; break;
    case BFP_ONE_MINUS_DST_ALPHA: psrc = "GL_ONE_MINUS_DST_ALPHA"; break;
    case BFP_SRC_ALPHA_SATURATE:  psrc = "GL_SRC_ALPHA_SATURATE"; break;
    }
    const char * pdst = "";
    switch(dst)
    {
    case BFP_ZERO:                pdst = "GL_ZERO"; break;
    case BFP_ONE:                 pdst = "GL_ONE"; break;
    case BFP_DST_COLOR:           pdst = "GL_DST_COLOR"; break;
    case BFP_ONE_MINUS_DST_COLOR: pdst = "GL_ONE_MINUS_DST_COLOR"; break;
    case BFP_SRC_ALPHA:           pdst = "GL_SRC_ALPHA"; break;
    case BFP_ONE_MINUS_SRC_ALPHA: pdst = "GL_ONE_MINUS_SRC_ALPHA"; break;
    case BFP_DST_ALPHA:           pdst = "GL_DST_ALPHA"; break;
    case BFP_ONE_MINUS_DST_ALPHA: pdst = "GL_ONE_MINUS_DST_ALPHA"; break;
    case BFP_SRC_ALPHA_SATURATE:  pdst = "GL_SRC_ALPHA_SATURATE"; break;
    }
    fprintf ( file, "\tglBlendFunc(%s, %s);\n", psrc, pdst );
}

// Depth func

void CppFileWriter::DepthFuncNever ()
{
    fprintf ( file, "\tglDepthFunc(GL_NEVER);\n" );
}

void CppFileWriter::DepthFuncLess ()
{
    fprintf ( file, "\tglDepthFunc(GL_LESS);\n" );
}

void CppFileWriter::DepthFuncLEqual ()
{
    fprintf ( file, "\tglDepthFunc(GL_LEQUAL);\n" );
}

void CppFileWriter::DepthFuncEqual ()
{
    fprintf ( file, "\tglDepthFunc(GL_EQUAL);\n" );
}

void CppFileWriter::DepthFuncGreater ()
{
    fprintf ( file, "\tglDepthFunc(GL_GREATER);\n" );
}

void CppFileWriter::DepthFuncNotEqual ()
{
    fprintf ( file, "\tglDepthFunc(GL_NOTEQUAL);\n" );
}

void CppFileWriter::DepthFuncGEqual ()
{
    fprintf ( file, "\tglDepthFunc(GL_GEQUAL);\n" );
}

void CppFileWriter::DepthFuncAlways ()
{
    fprintf ( file, "\tglDepthFunc(GL_ALWAYS);\n" );
}

// Stencil func

void CppFileWriter::StencilFuncNever ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_NEVER, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncLess ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_LESS, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncLEqual ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_LEQUAL, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncEqual ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_EQUAL, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncGreater ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_GREATER, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncNotEqual ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_NOTEQUAL, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncGEqual ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_GEQUAL, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilFuncAlways ( GIuint ref, GIuint mask )
{
    fprintf ( file, "\tglStencilFunc(GL_ALWAYS, %d, %d);\n", ref, mask );
}

void CppFileWriter::StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass )
{
    const char * p1 = "";
    switch(fail)
    {
    case SOP_KEEP:    p1 = "GL_KEEP"; break;
    case SOP_ZERO:    p1 = "GL_ONE"; break;
    case SOP_REPLACE: p1 = "GL_REPLACE"; break;
    case SOP_INCR:    p1 = "GL_INCR"; break;
    case SOP_DECR:    p1 = "GL_DECR"; break;
    case SOP_INVERT:  p1 = "GL_INVERT"; break;
    }
    const char * p2 = "";
    switch(zfail)
    {
    case SOP_KEEP:    p2 = "GL_KEEP"; break;
    case SOP_ZERO:    p2 = "GL_ONE"; break;
    case SOP_REPLACE: p2 = "GL_REPLACE"; break;
    case SOP_INCR:    p2 = "GL_INCR"; break;
    case SOP_DECR:    p2 = "GL_DECR"; break;
    case SOP_INVERT:  p2 = "GL_INVERT"; break;;
    }
    const char * p3 = "";
    switch(zpass)
    {
    case SOP_KEEP:    p3 = "GL_KEEP"; break;
    case SOP_ZERO:    p3 = "GL_ONE"; break;
    case SOP_REPLACE: p3 = "GL_REPLACE"; break;
    case SOP_INCR:    p3 = "GL_INCR"; break;
    case SOP_DECR:    p3 = "GL_DECR"; break;
    case SOP_INVERT:  p3 = "GL_INVERT"; break;
    }
    fprintf ( file, "\tglStencilOp(%s, %s, %s);\n", p1, p2, p3 );
}

// Begin, End

void CppFileWriter::BeginPoints ()
{
    fprintf ( file, "\tglBegin(GL_POINTS);\n" );
}

void CppFileWriter::BeginLines ()
{
    fprintf ( file, "\tglBegin(GL_LINES);\n" );
}

void CppFileWriter::BeginLineStrip ()
{
    fprintf ( file, "\tglBegin(GL_LINE_STRIP);\n" );
}

void CppFileWriter::BeginLineLoop ()
{
    fprintf ( file, "\tglBegin(GL_LINE_LOOP);\n" );
}

void CppFileWriter::BeginTriangles ()
{
    fprintf ( file, "\tglBegin(GL_TRIANGLES);\n" );
}

void CppFileWriter::BeginTriangleStrip ()
{
    fprintf ( file, "\tglBegin(GL_TRIANGLE_STRIP);\n" );
}

void CppFileWriter::BeginTriangleFan ()
{
    fprintf ( file, "\tglBegin(GL_TRIANGLE_FAN);\n" );
}

void CppFileWriter::BeginQuads ()
{
    fprintf ( file, "\tglBegin(GL_QUADS);\n" );
}

void CppFileWriter::BeginQuadStrip ()
{
    fprintf ( file, "\tglBegin(GL_QUAD_STRIP);\n" );
}

void CppFileWriter::BeginPolygon ()
{
    fprintf ( file, "\tglBegin(GL_POLYGON);\n" );
}

void CppFileWriter::End ()
{
    fprintf ( file, "\tglEnd();\n" );
}

// Cull face

void CppFileWriter::CullFaceBack ()
{
    fprintf ( file, "\tglCullFace(GL_BACK);\n" );
}

void CppFileWriter::CullFaceFront ()
{
    fprintf ( file, "\tglCullFace(GL_FRONT);\n" );
}

void CppFileWriter::CullFaceFrontAndBack ()
{
    fprintf ( file, "\tglCullFace(GL_FRONT_AND_BACK);\n" );
}

// Light

void CppFileWriter::LightAmbient ( Light l, float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglLightfv(GL_LIGHT%d, GL_AMBIENT, data);\n\t}\n", r, g, b, a, l );
}

void CppFileWriter::LightDiffuse ( Light l, float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglLightfv(GL_LIGHT%d, GL_DIFFUSE, data);\n\t}\n", r, g, b, a, l );
}

void CppFileWriter::LightSpecular ( Light l, float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglLightfv(GL_LIGHT%d, GL_SPECULAR, data);\n\t}\n", r, g, b, a, l );
}

void CppFileWriter::LightPosition ( Light l, float x, float y, float z, float w )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglLightfv(GL_LIGHT%d, GL_POSITION, data);\n\t}\n", x, y, z, w, l );
}

void CppFileWriter::LightPosition4v ( Light l, GIuint index )
{
    fprintf ( file, "\tglLightfv(GL_LIGHT0+%d, GL_POSITION, &floatValue[%d]);\n", l, index );
}

void CppFileWriter::LightQuadraticAttenuation ( Light l, float a )
{
    fprintf ( file, "\tglLightf(GL_LIGHT0+%d, GL_QUADRATIC_ATTENUATION, %+.9ef);\n", l, a );
}

// Stipple

void CppFileWriter::LineStipple ( int factor, int pattern )
{
    fprintf ( file, "\tglLineStipple(%d, %d);\n", factor, pattern );
}

void CppFileWriter::PolygonStipple ( const unsigned char mask[128] )
{
    fprintf ( file, "\t{\n\t\tunsigned char mask[] = {%d", mask[0] );
    for ( int i = 1; i < 128; ++i ) fprintf ( file, ",%d", mask[i] );
    fprintf ( file, "};\n\t\tglPolygonStipple(mask);\n\t}\n" );
}

// Material

void CppFileWriter::MaterialBackAmbient ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_BACK, GL_AMBIENT, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialBackDiffuse ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_BACK, GL_DIFFUSE, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialBackEmission ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_BACK, GL_EMISSION, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialBackSpecular ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_BACK, GL_SPECULAR, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialBackShininess ( float s )
{
    fprintf ( file, "\tglMaterialf(GL_BACK, GL_SHININESS, %+.9ef);\n", s );
}

void CppFileWriter::MaterialFrontAmbient ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_FRONT, GL_AMBIENT, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialFrontDiffuse ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_FRONT, GL_DIFFUSE, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialFrontEmission ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_FRONT, GL_EMISSION, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialFrontSpecular ( float r, float g, float b, float a )
{
    fprintf ( file, "\t{\n\t\tfloat data[] = {%+.9ef,%+.9ef,%+.9ef,%+.9ef};\n"
                    "\t\tglMaterialfv(GL_FRONT, GL_SPECULAR, data);\n\t}\n", r, g, b, a );
}

void CppFileWriter::MaterialFrontShininess ( float s )
{
    fprintf ( file, "\tglMaterialf(GL_FRONT, GL_SHININESS, %+.9ef);\n", s );
}

// Polygon mode

void CppFileWriter::PolygonMode ( Face face, enum PolygonMode mode )
{
    const char * pface = "";
    switch(face)
    {
    case FACE_FRONT         : pface = "GL_FRONT"         ; break;
    case FACE_BACK          : pface = "GL_BACK"          ; break;
    case FACE_FRONT_AND_BACK: pface = "GL_FRONT_AND_BACK"; break;
    }
    const char * pmode = "";
    switch(mode)
    {
    case PM_POINT: pmode = "GL_POINT"; break;
    case PM_LINE : pmode = "GL_LINE" ; break;
    case PM_FILL : pmode = "GL_FILL" ; break;
    }
    fprintf ( file, "\tglPolygonMode(%s, %s);\n", pface, pmode );
}

// Color

void CppFileWriter::Color ( float r, float g, float b )
{
    fprintf ( file, "\tglColor3f(%+.9ef, %+.9ef, %+.9ef);\n", r, g, b );
}

void CppFileWriter::Color ( float r, float g, float b, float a )
{
    fprintf ( file, "\tglColor4f(%+.9ef, %+.9ef, %+.9ef, %+.9ef);\n", r, g, b, a );
}

void CppFileWriter::Color3v ( GIuint index )
{
    fprintf ( file, "\tglColor3f(floatValue[%d], floatValue[%d], floatValue[%d]);\n", 
                        index, index+1, index+2 );
}

void CppFileWriter::Color4v ( GIuint index )
{
    fprintf ( file, "\tglColor4f(floatValue[%d], floatValue[%d], floatValue[%d], floatValue[%d]);\n", 
                        index, index+1, index+2, index+3 );
}

// Normal

void CppFileWriter::Normal ( float x, float y, float z )
{
    fprintf ( file, "\tglNormal3f(%+.9ef, %+.9ef, %+.9ef);\n", x, y, z );
}

void CppFileWriter::Normal3vI ( GIuint index )
{
    fprintf ( file, "\t{\n\t\tint i = intValue[%d];\n", index );
    fprintf ( file, "\t\tglNormal3f(floatValue[i], floatValue[i+1], floatValue[i+2]);\n\t}\n" );
}

// Vertex

void CppFileWriter::Vertex ( float x, float y )
{
    fprintf ( file, "\tglVertex2f(%+.9ef, %+.9ef);\n", x, y );
}

void CppFileWriter::Vertex ( float x, float y, float z )
{
    fprintf ( file, "\tglVertex3f(%+.9ef, %+.9ef, %+.9ef);\n", x, y, z );
}

void CppFileWriter::Vertex2v ( GIuint index )
{
    fprintf ( file, "\tglVertex2f(floatValue[%d], floatValue[%d]);\n", index, index+1 );
}

void CppFileWriter::Vertex3v ( GIuint index )
{
    fprintf ( file, "\tglVertex3f(floatValue[%d], floatValue[%d], floatValue[%d]);\n", 
                        index, index+1, index+2 );
}

void CppFileWriter::Vertex2vI ( GIuint index )
{
    fprintf ( file, "\t{\n\t\tint i = intValue[%d];\n", index );
    fprintf ( file, "\t\tglVertex2f(floatValue[i], floatValue[i+1]);\n\t}\n" );
}

void CppFileWriter::Vertex3vI ( GIuint index )
{
    fprintf ( file, "\t{\n\t\tint i = intValue[%d];\n", index );
    fprintf ( file, "\t\tglVertex3f(floatValue[i], floatValue[i+1], floatValue[i+2]);\n\t}\n" );
}

// Display list

void CppFileWriter::CallList ( int list )
{
    fprintf ( file, "\tglCallList(%d);\n", list );
}

void CppFileWriter::CallListIndex ( GIuint index )
{
    fprintf ( file, "\tglCallList(intValue[%d]);\n", index );
}

void CppFileWriter::EndList ()
{
    fprintf ( file, "\tglEndList();\n" );
}

void CppFileWriter::NewListCompile ( int list )
{
    fprintf ( file, "\tglNewList(%d, GL_COMPILE);\n", list );
}

void CppFileWriter::NewListCompileAndExecute ( int list )
{
    fprintf ( file, "\tglNewList(%d, GL_COMPILE_AND_EXECUTE);\n", list );
}

// Matrix

void CppFileWriter::Frustum4v ( GIuint i )
{
    fprintf ( file, "\tglFrustum(floatValue[%d], floatValue[%d], floatValue[%d], "
	                            "floatValue[%d], floatValue[%d], floatValue[%d]);\n",
                                i, i+1, i+2, i+3, i+4, i+5 );
}

void CppFileWriter::LoadIdentity ()
{
    fprintf ( file, "\tglLoadIdentity();\n" );
}

void CppFileWriter::LookAt ( GIuint i )
{
    fprintf ( file, "\tgluLookAt(floatValue[%d], floatValue[%d], floatValue[%d], "
	                            "floatValue[%d], floatValue[%d], floatValue[%d], "
	                            "floatValue[%d], floatValue[%d], floatValue[%d]);\n",
                                i, i+1, i+2, i+3, i+4, i+5, i+6, i+7, i+8 );
}

void CppFileWriter::MatrixModeModelView ()
{
    fprintf ( file, "\tglMatrixMode(GL_MODELVIEW);\n" );
}

void CppFileWriter::MatrixModeProjection ()
{
    fprintf ( file, "\tglMatrixMode(GL_PROJECTION);\n" );
}

void CppFileWriter::MatrixModeTexture ()
{
    fprintf ( file, "\tglMatrixMode(GL_TEXTURE);\n" );
}

void CppFileWriter::MultMatrix ( GIuint index )
{
    fprintf ( file, "\tglMultMatrixf(&floatValue[%d]);\n", index );
}

void CppFileWriter::Perspective ( float fovy, float aspect, float zNear, float zFar )
{
    fprintf ( file, "\tgluPerspective(%+.9ef, %+.9ef, %+.9ef, %+.9ef);\n", 
                        fovy, aspect, zNear, zFar);
}

void CppFileWriter::Perspective4v ( GIuint index )
{
    fprintf ( file, "\tgluPerspective(floatValue[%d], floatValue[%d], "
	                 "floatValue[%d], floatValue[%d] );\n", index, index+1, index+2, index+3);
}

void CppFileWriter::PopMatrix ()
{
    fprintf ( file, "\tglPopMatrix();\n" );
}

void CppFileWriter::PushMatrix ()
{
    fprintf ( file, "\tglPushMatrix();\n" );
}

void CppFileWriter::Rotate ( float angle, float x, float y, float z )
{
    fprintf ( file, "\tglRotatef(%+.9ef, %+.9ef, %+.9ef, %+.9ef);\n", angle, x, y, z );
}

void CppFileWriter::Rotate4v ( GIuint index )
{
    fprintf ( file, "\tglRotatef(floatValue[%d], floatValue[%d], "
	                 "floatValue[%d], floatValue[%d]);\n", index, index+1, index+2, index+3 );
}

void CppFileWriter::Scale ( float x, float y, float z )
{
    fprintf ( file, "\tglScalef(%+.9ef, %+.9ef, %+.9ef);\n", x, y, z );
}

void CppFileWriter::Scale3v ( GIuint index )
{
    fprintf ( file, "\tglScalef(floatValue[%d], floatValue[%d], "
	                 "floatValue[%d]);\n", index, index+1, index+2 );
}

void CppFileWriter::Translate ( float x, float y, float z )
{
    fprintf ( file, "\tglTranslatef(%+.9ef, %+.9ef, %+.9ef);\n", x, y, z );
}

void CppFileWriter::Translate3v ( GIuint index )
{
    fprintf ( file, "\tglTranslatef(floatValue[%d], floatValue[%d], "
	                 "floatValue[%d]);\n", index, index+1, index+2 );
}

// Texture

void CppFileWriter::BindTexture2D ( GIuint index )
{
    fprintf ( file, "\tglBindTexture(GL_TEXTURE_2D, intValue[%d]);\n", index );
}

void CppFileWriter::Build2DMipmapsBGR ( int width, int height, const unsigned char * pData )
{
    fprintf ( file, "\t{\n\t\textern int intArr%d[];\n", ++intArrCount );
    fprintf ( file, "\t\tgluBuild2DMipmaps(GL_TEXTURE_2D, 3, %d, %d, GL_BGR_EXT, "
                    "GL_UNSIGNED_BYTE, (const unsigned char *)intArr%d);\n\t}\n",
                     width, height, intArrCount );
    const int n = (3*width*height+3)/4;
    putIntArray ( *tmp, intArrCount, n, (const int *) pData );
}

void CppFileWriter::Build2DMipmapsBGR_BTPC ( int lenght, const unsigned char * pData )
{
    fprintf ( file, "\t{\n\t\textern int intArr%d[];\n", ++intArrCount );
    fprintf ( file, "\t\tconst bit8 * p = (const bit8 *) intArr%d;\n", intArrCount );
    fprintf ( file, "\t\tint nx, ny, bpp;\n" );
    fprintf ( file, "\t\tunsigned char * data = 0;\n" );
    fprintf ( file, "\t\tPseudoReadSeekFile pf ( CArrRef<bit8> ( p, %d ) );\n", lenght );
    fprintf ( file, "\t\tif ( dbtpc ( pf, nx, ny, data, bpp ) )\n" );
    fprintf ( file, "\t\t\tgluBuild2DMipmaps(GL_TEXTURE_2D, 3, nx, ny, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);\n" );
    fprintf ( file, "\t\tdelete[] data;\n\t}\n" );
    const int n = (lenght+3)/4;
    putIntArray ( *tmp, intArrCount, n, (const int *) pData );
}

void CppFileWriter::GenTextures ( GIuint n, GIuint index )
{
    if ( intSize < index + n ) intSize = index + n;
    fprintf ( file, "\tglGenTextures(%d, (GLuint*) &intValue[%d]);\n", n, index );
}

void CppFileWriter::TexCoord2 ( float s, float t )
{
    fprintf ( file, "\tglTexCoord2f(%+.9ef, %+.9ef);\n", s, t );
}

void CppFileWriter::TexParameter2DMinFilterLinearMipmapNearest()
{
	fprintf ( file, "\tglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);\n" );
}

void CppFileWriter::TexParameter2DWrapSRepeat()
{
    fprintf ( file, "\tglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );\n" );
}

void CppFileWriter::TexParameter2DWrapTRepeat()
{
    fprintf ( file, "\tglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );\n" );
}

// Get

void CppFileWriter::GetFloatViewport ( GIuint index )
{
    if ( floatSize < index + 4 ) floatSize = index + 4;
    fprintf ( file, "\tglGetFloatv(GL_VIEWPORT, &floatValue[%d]);\n", index );
}

void CppFileWriter::GetIntViewport ( GIuint index )
{
    if ( floatSize < index + 4 ) floatSize = index + 4;
    fprintf ( file, "\tglGetIntegerv(GL_VIEWPORT, intValue + %d);\n", index );
}

// Quadric objects

void CppFileWriter::QuadricTexture ( bool b )
{
    fprintf ( file, "\tgluQuadricTexture(quadObj, %d);\n", int(b) );
}

void CppFileWriter::Disk ( float innerRadius, float outerRadius, int slices, int loops )
{
    fprintf ( file, "\tgluDisk(quadObj, %+.9ef, %+.9ef, %d, %d);\n", innerRadius, outerRadius, slices, loops );
}

void CppFileWriter::Sphere ( float radius, int slices, int stacks )
{
    fprintf ( file, "\tgluSphere(quadObj, %+.9ef, %d, %d);\n", radius, slices, stacks );
}

// Integer values

void CppFileWriter::IntAstDA ( GIuint index, int n, const int * p )
{
    if ( n < 1 ) return;
    if ( intSize < index + n ) intSize = index + n;
    fprintf ( file, "\t{\n\t\textern int intArr%d[];"
                       "\n\t\tfor(int i = 0; i < %d; ++i) intValue[i", ++intArrCount, n );
    if ( index > 0 ) fprintf ( file, "+%d", index );
    fprintf ( file, "] = intArr%d[i];\n\t}\n", intArrCount );
    putIntArray ( *tmp, intArrCount, n, p );
}

void CppFileWriter::IntAstDC ( GIuint index, int i )
{
    if ( intSize <= index ) intSize = index + 1;
    fprintf ( file, "\tintValue[%d] = %d;\n", index, i );
}

void CppFileWriter::IntAstDD ( GIuint index1, GIuint index2 )
{
    if ( intSize <= index1 ) intSize = index1 + 1;
    fprintf ( file, "\tintValue[%d] = intValue[%d];\n", index1, index2 );
}

void CppFileWriter::IntAstDI ( GIuint index1, GIuint index2 )
{
    if ( intSize <= index1 ) intSize = index1 + 1;
    fprintf ( file, "\tintValue[%d] = intValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::IntAddDC ( GIuint index, int i )
{
    fprintf ( file, "\tintValue[%d] += %d;\n", index, i );
}

void CppFileWriter::IntAddDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tintValue[%d] += intValue[%d];\n", index1, index2 );
}

void CppFileWriter::IntSubDC ( GIuint index, int i )
{
    fprintf ( file, "\tintValue[%d] -= %d;\n", index, i );
}

void CppFileWriter::IntSubDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tintValue[%d] -= intValue[%d];\n", index1, index2 );
}

void CppFileWriter::IntMulDC ( GIuint index, int i )
{
    fprintf ( file, "\tintValue[%d] *= %d;\n", index, i );
}

void CppFileWriter::IntMulDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tintValue[%d] *= intValue[%d];\n", index1, index2 );
}

void CppFileWriter::IntDivDC ( GIuint index, int i )
{
    fprintf ( file, "\tintValue[%d] /= %d;\n", index, i );
}

void CppFileWriter::IntDivDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tintValue[%d] /= intValue[%d];\n", index1, index2 );
}

void CppFileWriter::IntModDC ( GIuint index, int i )
{
    fprintf ( file, "\tintValue[%d] %%= %d;\n", index, i );
}

void CppFileWriter::IntModDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tintValue[%d] %%= intValue[%d];\n", index1, index2 );
}

// Float values

void CppFileWriter::FloatAstDA ( GIuint index, int n, const float * p )
{
    if ( n < 1 ) return;
    if ( floatSize < index + n ) floatSize = index + n;
    fprintf ( file, "\t{\n\t\textern float floatArr%d[];"
                       "\n\t\tfor(int i = 0; i < %d; ++i) floatValue[i", ++floatArrCount, n );
    if ( index > 0 ) fprintf ( file, "+%d", index );
    fprintf ( file, "] = floatArr%d[i];\n\t}\n", floatArrCount );
    writeStr ( *tmp, "float floatArr" );
    writeIntDec ( *tmp, (int32) floatArrCount );
    tmp->putc ( "[" );
    writeIntDec ( *tmp, (int32) n );
    writeStr ( *tmp, "] = {\n " );
    writeExpDec ( *tmp, *p, 9, true );
    tmp->putc ( "f" );
    for ( int i = 1; i < n; ++i )
    {
        tmp->putc ( "," );
        writeExpDec ( *tmp, p[i], 9, true );
        tmp->putc ( "f" );
        if ( i % 10 == 9 ) tmp->putc ( "\n" );
    }
    writeStr ( *tmp, "\n};\n\n" );
}

void CppFileWriter::FloatAstDC ( GIuint index, float f )
{
    if ( floatSize <= index ) floatSize = index + 1;
    fprintf ( file, "\tfloatValue[%d] = %+.9ef;\n", index, f );
}

void CppFileWriter::FloatAstDD ( GIuint index1, GIuint index2 )
{
    if ( floatSize <= index1 ) floatSize = index1 + 1;
    fprintf ( file, "\tfloatValue[%d] = floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatAstDI ( GIuint index1, GIuint index2 )
{
    if ( floatSize <= index1 ) floatSize = index1 + 1;
    fprintf ( file, "\tfloatValue[%d] = floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatAstIC ( GIuint index, float f )
{
    fprintf ( file, "\t{\n\t\tconst nat i = intValue[%d];\n", index );
    fprintf ( file, "\t\tif(i >= floatValue.size()) floatValue.resize(i+100);\n" );
    fprintf ( file, "\t\tfloatValue[i] = %+.9ef;\n\t}\n", f );
}

void CppFileWriter::FloatAstID ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\t{\n\t\tconst nat i = intValue[%d];\n", index1 );
    fprintf ( file, "\t\tif(i >= floatValue.size()) floatValue.resize(i+100);\n" );
    fprintf ( file, "\t\tfloatValue[i] = floatValue[%d];\n\t}\n", index2 );
}

void CppFileWriter::FloatAstII ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\t{\n\t\tconst nat i = intValue[%d];\n", index1 );
    fprintf ( file, "\t\tif(i >= floatValue.size()) floatValue.resize(i+100);\n" );
    fprintf ( file, "\t\tfloatValue[i] = floatValue[intValue[%d]];\n\t}\n", index2 );
}

void CppFileWriter::FloatAddDC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[%d] += %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatAddDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] += floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatAddDI ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] += floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatAddIC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] += %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatAddID ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] += floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatAddII ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] += floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatSubDC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[%d] -= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatSubDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] -= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatSubDI ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] -= floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatSubIC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] -= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatSubID ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] -= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatSubII ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] -= floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatMulDC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[%d] *= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatMulDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] *= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatMulDI ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] *= floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatMulIC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] *= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatMulID ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] *= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatMulII ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] *= floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatDivDC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[%d] /= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatDivDD ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] /= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatDivDI ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] /= floatValue[intValue[%d]];\n", index1, index2 );
}

void CppFileWriter::FloatDivIC ( GIuint index1, float f )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] /= %+.9ef;\n", index1, f );
}

void CppFileWriter::FloatDivID ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] /= floatValue[%d];\n", index1, index2 );
}

void CppFileWriter::FloatDivII ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[intValue[%d]] /= floatValue[intValue[%d]];\n", index1, index2 );
}

// Mix commands

void CppFileWriter::IntAstFromFloat ( GIuint index1, GIuint index2 )
{
    if ( intSize <= index1 ) intSize = index1 + 1;
    fprintf ( file, "\tintValue[%d] = int(floatValue[%d]);\n", index1, index2 );
}

void CppFileWriter::FloatAstFromInt ( GIuint index1, GIuint index2 )
{
    if ( floatSize <= index1 ) floatSize = index1 + 1;
    fprintf ( file, "\tfloatValue[%d] = float(intValue[%d]);\n", index1, index2 );
}

// Conditional commands

bool CppFileWriter::IfIntDC ( GIuint index, Relation rel, int i )
{
    const char * str = "";
    switch ( rel )
    {
        case EQ: str = "=="; break;
        case NE: str = "!="; break;
        case LT: str = "<" ; break;
        case LE: str = "<="; break;
        case GT: str = ">" ; break;
        case GE: str = ">="; break;
    }
    fprintf ( file, "\tif(intValue[%d] %s %d)\n\t{\n", index, str, i );
    return 1;
}

bool CppFileWriter::IfIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    const char * str = "";
    switch ( rel )
    {
        case EQ: str = "=="; break;
        case NE: str = "!="; break;
        case LT: str = "<" ; break;
        case LE: str = "<="; break;
        case GT: str = ">" ; break;
        case GE: str = ">="; break;
    }
    fprintf ( file, "\tif(intValue[%d] %s intValue[%d])\n\t{\n", index1, str, index2 );
    return 1;
}

bool CppFileWriter::IfFloatDC ( GIuint index, Relation rel, float f )
{
    const char * str = "";
    switch ( rel )
    {
        case EQ: str = "=="; break;
        case NE: str = "!="; break;
        case LT: str = "<" ; break;
        case LE: str = "<="; break;
        case GT: str = ">" ; break;
        case GE: str = ">="; break;
    }
    fprintf ( file, "\tif(floatValue[%d] %s %+.9ef)\n\t{\n", index, str, f );
    return 1;
}

bool CppFileWriter::IfFloatDD ( GIuint index1, Relation rel, GIuint index2 )
{
    const char * str = "";
    switch ( rel )
    {
        case EQ: str = "=="; break;
        case NE: str = "!="; break;
        case LT: str = "<" ; break;
        case LE: str = "<="; break;
        case GT: str = ">" ; break;
        case GE: str = ">="; break;
    }
    fprintf ( file, "\tif(floatValue[%d] %s floatValue[%d])\n\t{\n", index1, str, index2 );
    return 1;
}

bool CppFileWriter::Else ()
{
    fprintf ( file, "\t}\n\telse\n\t{\n" );
    return 1;
}

void CppFileWriter::EndIf ()
{
    fprintf ( file, "\t} // endif\n" );
}

// Loop

void CppFileWriter::Do ()
{
    fprintf ( file, "\tdo\n\t{\n" );
}

bool CppFileWriter::WhileIntDC ( GIuint index1, Relation rel, int i )
{
    const char * str = "";
    switch ( rel )
    {
    case EQ: str = "=="; break;
    case NE: str = "!="; break;
    case LT: str = "<" ; break;
    case LE: str = "<="; break;
    case GT: str = ">" ; break;
    case GE: str = ">="; break;
    }
    fprintf ( file, "\t}\n\twhile(intValue[%d] %s %d);\n", index1, str, i );
    return false;
}

bool CppFileWriter::WhileIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    const char * str = "";
    switch ( rel )
    {
    case EQ: str = "=="; break;
    case NE: str = "!="; break;
    case LT: str = "<" ; break;
    case LE: str = "<="; break;
    case GT: str = ">" ; break;
    case GE: str = ">="; break;
    }
    fprintf ( file, "\t}\n\twhile(intValue[%d] %s intValue[%d]);\n", index1, str, index2 );
    return false;
}

// Mathematics

void CppFileWriter::FloatAbs ( GIuint index )
{
    fprintf ( file, "\tfloatValue[%d] = fabsf(floatValue[%d]);\n", index, index );
}

void CppFileWriter::FloatAtan2 ( GIuint index1, GIuint index2 )
{
    fprintf ( file, "\tfloatValue[%d] = atan2f(floatValue[%d],floatValue[%d]);\n", index1, index1, index2 );
}

void CppFileWriter::FloatCos ( GIuint index )
{
    fprintf ( file, "\tfloatValue[%d] = cosf(floatValue[%d]);\n", index, index );
}

void CppFileWriter::FloatSin ( GIuint index )
{
    fprintf ( file, "\tfloatValue[%d] = sinf(floatValue[%d]);\n", index, index );
}

void CppFileWriter::FloatSqrt ( GIuint index )
{
    fprintf ( file, "\tfloatValue[%d] = sqrtf(floatValue[%d]);\n", index, index );
}

// Start position

void CppFileWriter::NextStart ( const GraphicInterface::Node * node )
{
    ++nextStartCount;
    fprintf ( file, "\tnextStart = %d;\nm%d:\n", nextStartCount, nextStartCount );
}

bool CppFileWriter::NextStartAndReturn ( const GraphicInterface::Node * node )
{
    ++nextStartCount;
    fprintf ( file, "\tnextStart = %d;\n\treturn;\nm%d:\n", nextStartCount, nextStartCount );
    return 0;
}

void CppFileWriter::NextStartFirst ()
{
    fprintf ( file, "\tnextStart = 0;\n" );
}

// Time

void CppFileWriter::Clock ( GIuint index )
{
    if ( intSize <= index ) intSize = index + 1;
    fprintf ( file, "\tintValue[%d] = clock();\n", index );
}

void CppFileWriter::Time ( GIuint index )
{
    if ( floatSize < index + 3 ) floatSize = index + 3;
    fprintf ( file, "\t{\n\t\tlong ltime;\n\t\ttime(&ltime);\n"
                    "\t\tstruct tm * p = localtime(&ltime);\n"
                    "\t\tintValue[%d] = p->tm_hour;\n"
                    "\t\tintValue[%d] = p->tm_min;\n"
                    "\t\tintValue[%d] = p->tm_sec;\n\t}\n", index, index+1, index+2 );
}

void CppFileWriter::SetTimer ( int p )
{
    fprintf ( file, "\tSetTimer(%d);\n", p );
}

