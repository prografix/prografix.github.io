#include "io.h"
#include <fcntl.h>
#include <sys/stat.h>
#include "GraphicInterface.h"
#include "FileWriter.h"

FileWriter::FileWriter ( const char * filename, const char * comment )
{
    h = _open ( filename, _O_TRUNC | _O_CREAT | _O_WRONLY | _O_BINARY, _S_IWRITE );
    int flags = 0;
    _write ( h, &flags, 4 );
    int num = 0;
    while ( comment[num] ) ++num;
    _write ( h, &num, 4 );
    _write ( h, comment, num );
    int version = 4;
    _write ( h, &version, 4 );
}

FileWriter::~FileWriter ()
{
    _close ( h );
}

// Clear

void FileWriter::ClearColor ( float r, float g, float b, float a )
{
    int i = CLEAR_COLOR;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::ClearColorBuffer ()
{
    int i = CLEAR_COLOR_BUFFER;
    _write ( h, &i, 4 );
}

void FileWriter::ClearDepthBuffer ()
{
    int i = CLEAR_DEPTH_BUFFER;
    _write ( h, &i, 4 );
}

void FileWriter::ClearStencilBuffer ()
{
    int i = CLEAR_STENCIL_BUFFER;
    _write ( h, &i, 4 );
}

// Enable

void FileWriter::EnableBlend ()
{
    int i = ENABLE_BLEND;
    _write ( h, &i, 4 );
}

void FileWriter::EnableColorMaterial ()
{
    int i = ENABLE_COLOR_MATERIAL;
    _write ( h, &i, 4 );
}

void FileWriter::EnableCullFace ()
{
    int i = ENABLE_CULL_FACE;
    _write ( h, &i, 4 );
}

void FileWriter::EnableDepthTest ()
{
    int i = ENABLE_DEPTH_TEST;
    _write ( h, &i, 4 );
}

void FileWriter::EnableLight ( Light l )
{
    int i = ENABLE_LIGHT;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
}

void FileWriter::EnableLighting ()
{
    int i = ENABLE_LIGHTING;
    _write ( h, &i, 4 );
}

void FileWriter::EnableLineStipple ()
{
    int i = ENABLE_LINE_STIPPLE;
    _write ( h, &i, 4 );
}

void FileWriter::EnableNormalize ()
{
    int i = ENABLE_NORMALIZE;
    _write ( h, &i, 4 );
}

void FileWriter::EnablePolygonStipple ()
{
    int i = ENABLE_POLYGON_STIPPLE;
    _write ( h, &i, 4 );
}

void FileWriter::EnableStencilTest ()
{
    int i = ENABLE_STENCIL_TEST;
    _write ( h, &i, 4 );
}

void FileWriter::EnableTexture2D ()
{
    int i = ENABLE_TEXTURE_2D;
    _write ( h, &i, 4 );
}

void FileWriter::EnableTextureGenS ()
{
    int i = ENABLE_TEXTURE_GEN_S;
    _write ( h, &i, 4 );
}

void FileWriter::EnableTextureGenT ()
{
    int i = ENABLE_TEXTURE_GEN_T;
    _write ( h, &i, 4 );
}

// Disable

void FileWriter::DisableBlend ()
{
    int i = DISABLE_BLEND;
    _write ( h, &i, 4 );
}

void FileWriter::DisableColorMaterial ()
{
    int i = DISABLE_COLOR_MATERIAL;
    _write ( h, &i, 4 );
}

void FileWriter::DisableCullFace ()
{
    int i = DISABLE_CULL_FACE;
    _write ( h, &i, 4 );
}

void FileWriter::DisableDepthTest ()
{
    int i = DISABLE_DEPTH_TEST;
    _write ( h, &i, 4 );
}

void FileWriter::DisableLight ( Light l )
{
    int i = DISABLE_LIGHT;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
}

void FileWriter::DisableLighting ()
{
    int i = DISABLE_LIGHTING;
    _write ( h, &i, 4 );
}

void FileWriter::DisableLineStipple ()
{
    int i = DISABLE_LINE_STIPPLE;
    _write ( h, &i, 4 );
}

void FileWriter::DisableNormalize ()
{
    int i = DISABLE_NORMALIZE;
    _write ( h, &i, 4 );
}

void FileWriter::DisablePolygonStipple ()
{
    int i = DISABLE_POLYGON_STIPPLE;
    _write ( h, &i, 4 );
}

void FileWriter::DisableStencilTest ()
{
    int i = DISABLE_STENCIL_TEST;
    _write ( h, &i, 4 );
}

void FileWriter::DisableTexture2D ()
{
    int i = DISABLE_TEXTURE_2D;
    _write ( h, &i, 4 );
}

void FileWriter::DisableTextureGenS ()
{
    int i = DISABLE_TEXTURE_GEN_S;
    _write ( h, &i, 4 );
}

void FileWriter::DisableTextureGenT ()
{
    int i = DISABLE_TEXTURE_GEN_T;
    _write ( h, &i, 4 );
}

// Blend func

void FileWriter::BlendFunc ( BlendFuncPar src, BlendFuncPar dst )
{
    int i = BLEND_FUNC;
    _write ( h, &i, 4 );
    _write ( h, &src, 4 );
    _write ( h, &dst, 4 );
}

// Depth func

void FileWriter::DepthFuncNever ()
{
    int i = DEPTH_FUNC_NEVER;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncLess ()
{
    int i = DEPTH_FUNC_LESS;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncLEqual ()
{
    int i = DEPTH_FUNC_LEQUAL;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncEqual ()
{
    int i = DEPTH_FUNC_EQUAL;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncGreater ()
{
    int i = DEPTH_FUNC_GREATER;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncNotEqual ()
{
    int i = DEPTH_FUNC_NOTEQUAL;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncGEqual ()
{
    int i = DEPTH_FUNC_GEQUAL;
    _write ( h, &i, 4 );
}

void FileWriter::DepthFuncAlways ()
{
    int i = DEPTH_FUNC_ALWAYS;
    _write ( h, &i, 4 );
}

// Stencil func

void FileWriter::StencilFuncNever ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_NEVER;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncLess ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_LESS;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncLEqual ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_LEQUAL;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncEqual ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_EQUAL;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncGreater ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_GREATER;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncNotEqual ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_NOTEQUAL;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncGEqual ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_GEQUAL;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilFuncAlways ( GIuint ref, GIuint mask )
{
    int i = STENCIL_FUNC_ALWAYS;
    _write ( h, &i, 4 );
    _write ( h, &ref, 4 );
    _write ( h, &mask, 4 );
}

void FileWriter::StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass )
{
    int i = STENCIL_OP;
    _write ( h, &i, 4 );
    _write ( h, &fail, 4 );
    _write ( h, &zfail, 4 );
    _write ( h, &zpass, 4 );
}

// Begin, End

void FileWriter::BeginPoints ()
{
    int i = BEGIN_POINTS;
    _write ( h, &i, 4 );
}

void FileWriter::BeginLines ()
{
    int i = BEGIN_LINES;
    _write ( h, &i, 4 );
}

void FileWriter::BeginLineStrip ()
{
    int i = BEGIN_LINE_STRIP;
    _write ( h, &i, 4 );
}

void FileWriter::BeginLineLoop ()
{
    int i = BEGIN_LINE_LOOP;
    _write ( h, &i, 4 );
}

void FileWriter::BeginTriangles ()
{
    int i = BEGIN_TRIANGLES;
    _write ( h, &i, 4 );
}

void FileWriter::BeginTriangleStrip ()
{
    int i = BEGIN_TRIANGLE_STRIP;
    _write ( h, &i, 4 );
}

void FileWriter::BeginTriangleFan ()
{
    int i = BEGIN_TRIANGLE_FAN;
    _write ( h, &i, 4 );
}

void FileWriter::BeginQuads ()
{
    int i = BEGIN_QUADS;
    _write ( h, &i, 4 );
}

void FileWriter::BeginQuadStrip ()
{
    int i = BEGIN_QUAD_STRIP;
    _write ( h, &i, 4 );
}

void FileWriter::BeginPolygon ()
{
    int i = BEGIN_POLYGON;
    _write ( h, &i, 4 );
}

void FileWriter::End ()
{
    int i = END;
    _write ( h, &i, 4 );
}

// Cull face

void FileWriter::CullFaceBack ()
{
    int i = CULL_FACE_BACK;
    _write ( h, &i, 4 );
}

void FileWriter::CullFaceFront ()
{
    int i = CULL_FACE_FRONT;
    _write ( h, &i, 4 );
}

void FileWriter::CullFaceFrontAndBack ()
{
    int i = CULL_FACE_FRONT_AND_BACK;
    _write ( h, &i, 4 );
}

// Light

void FileWriter::LightAmbient ( Light l, float r, float g, float b, float a )
{
    int i = LIGHT_AMBIENT;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::LightDiffuse ( Light l, float r, float g, float b, float a )
{
    int i = LIGHT_DIFFUSE;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::LightSpecular ( Light l, float r, float g, float b, float a )
{
    int i = LIGHT_SPECULAR;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::LightPosition ( Light l, float r, float g, float b, float a )
{
    int i = LIGHT_POSITION;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::LightPosition4v ( Light l, GIuint index )
{
    int i = LIGHT_POSITION_4V;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::LightQuadraticAttenuation ( Light l, float a )
{
    int i = LIGHT_QUADRATIC_ATTENUATION;
    _write ( h, &i, 4 );
    _write ( h, &l, 4 );
    _write ( h, &a, 4 );
}

// Stipple

void FileWriter::LineStipple ( int factor, int pattern )
{
    int i = LINE_STIPPLE;
    _write ( h, &i, 4 );
    _write ( h, &factor, 4 );
    _write ( h, &pattern, 4 );
}

void FileWriter::PolygonStipple ( const unsigned char mask[128] )
{
    int i = POLYGON_STIPPLE_ARR;
    _write ( h, &i, 4 );
    _write ( h, mask, 128 );
}

// Material

void FileWriter::MaterialBackAmbient ( float r, float g, float b, float a )
{
    int i = MATERIAL_BACK_AMBIENT;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialBackDiffuse ( float r, float g, float b, float a )
{
    int i = MATERIAL_BACK_DIFFUSE;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialBackEmission ( float r, float g, float b, float a )
{
    int i = MATERIAL_BACK_EMISSION;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialBackSpecular ( float r, float g, float b, float a )
{
    int i = MATERIAL_BACK_SPECULAR;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialBackShininess ( float s )
{
    int i = MATERIAL_BACK_SHININESS;
    _write ( h, &i, 4 );
    _write ( h, &s, 4 );
}

void FileWriter::MaterialFrontAmbient ( float r, float g, float b, float a )
{
    int i = MATERIAL_FRONT_AMBIENT;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialFrontDiffuse ( float r, float g, float b, float a )
{
    int i = MATERIAL_FRONT_DIFFUSE;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialFrontEmission ( float r, float g, float b, float a )
{
    int i = MATERIAL_FRONT_EMISSION;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialFrontSpecular ( float r, float g, float b, float a )
{
    int i = MATERIAL_FRONT_SPECULAR;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::MaterialFrontShininess ( float s )
{
    int i = MATERIAL_FRONT_SHININESS;
    _write ( h, &i, 4 );
    _write ( h, &s, 4 );
}

// Polygon mode

void FileWriter::PolygonMode ( Face face, enum PolygonMode mode )
{
    int i = POLYGON_MODE;
    _write ( h, &i, 4 );
    _write ( h, &face, 4 );
    _write ( h, &mode, 4 );
}

// Color

void FileWriter::Color ( float r, float g, float b )
{
    int i = COLOR3;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
}

void FileWriter::Color ( float r, float g, float b, float a )
{
    int i = COLOR4;
    _write ( h, &i, 4 );
    _write ( h, &r, 4 );
    _write ( h, &g, 4 );
    _write ( h, &b, 4 );
    _write ( h, &a, 4 );
}

void FileWriter::Color3v ( GIuint index )
{
    int i = COLOR_3V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Color4v ( GIuint index )
{
    int i = COLOR_4V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Normal

void FileWriter::Normal ( float x, float y, float z )
{
    int i = NORMAL;
    _write ( h, &i, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
    _write ( h, &z, 4 );
}

void FileWriter::Normal3vI ( GIuint index )
{
    int i = NORMAL_3V_I;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Vertex

void FileWriter::Vertex ( float x, float y )
{
    int i = VERTEX2;
    _write ( h, &i, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
}

void FileWriter::Vertex ( float x, float y, float z )
{
    int i = VERTEX3;
    _write ( h, &i, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
    _write ( h, &z, 4 );
}

void FileWriter::Vertex2v ( GIuint index )
{
    int i = VERTEX_2V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Vertex3v ( GIuint index )
{
    int i = VERTEX_3V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Vertex2vI ( GIuint index )
{
    int i = VERTEX_2V_I;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Vertex3vI ( GIuint index )
{
    int i = VERTEX_3V_I;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Display list

void FileWriter::CallList ( int list )
{
    int i = CALL_LIST;
    _write ( h, &i, 4 );
    _write ( h, &list, 4 );
}

void FileWriter::CallListIndex ( GIuint index )
{
    int i = CALL_LIST_INDEX;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::EndList ()
{
    int i = END_LIST;
    _write ( h, &i, 4 );
}

void FileWriter::NewListCompile ( int list )
{
    int i = NEW_LIST_COMPILE;
    _write ( h, &i, 4 );
    _write ( h, &list, 4 );
}

void FileWriter::NewListCompileAndExecute ( int list )
{
    int i = NEW_LIST_COMPILE_AND_EXECUTE;
    _write ( h, &i, 4 );
    _write ( h, &list, 4 );
}

// Matrix

void FileWriter::Frustum4v ( GIuint index )
{
    int i = FRUSTUM_4V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::LoadIdentity ()
{
    int i = LOAD_IDENTITY;
    _write ( h, &i, 4 );
}

void FileWriter::LookAt ( GIuint index )
{
    int i = LOOK_AT;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::MatrixModeModelView ()
{
    int i = MATRIX_MODE_MODEL_VIEW;
    _write ( h, &i, 4 );
}

void FileWriter::MatrixModeProjection ()
{
    int i = MATRIX_MODE_PROJECTION;
    _write ( h, &i, 4 );
}

void FileWriter::MatrixModeTexture ()
{
    int i = MATRIX_MODE_TEXTURE;
    _write ( h, &i, 4 );
}

void FileWriter::MultMatrix ( GIuint index )
{
    int i = MULT_MATRIX;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Perspective ( float fovy, float aspect, float zNear, float zFar )
{
    int i = PERSPECTIVE;
    _write ( h, &i, 4 );
    _write ( h, &fovy, 4 );
    _write ( h, &aspect, 4 );
    _write ( h, &zNear, 4 );
    _write ( h, &zFar, 4 );
}

void FileWriter::Perspective4v ( GIuint index )
{
    int i = PERSPECTIVE_4V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::PopMatrix ()
{
    int i = POP_MATRIX;
    _write ( h, &i, 4 );
}

void FileWriter::PushMatrix ()
{
    int i = PUSH_MATRIX;
    _write ( h, &i, 4 );
}

void FileWriter::Rotate ( float angle, float x, float y, float z )
{
    int i = ROTATE;
    _write ( h, &i, 4 );
    _write ( h, &angle, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
    _write ( h, &z, 4 );
}

void FileWriter::Rotate4v ( GIuint index )
{
    int i = ROTATE_4V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Scale ( float x, float y, float z )
{
    int i = SCALE;
    _write ( h, &i, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
    _write ( h, &z, 4 );
}

void FileWriter::Scale3v ( GIuint index )
{
    int i = SCALE_3V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Translate ( float x, float y, float z )
{
    int i = TRANSLATE;
    _write ( h, &i, 4 );
    _write ( h, &x, 4 );
    _write ( h, &y, 4 );
    _write ( h, &z, 4 );
}

void FileWriter::Translate3v ( GIuint index )
{
    int i = TRANSLATE_3V;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Texture

void FileWriter::BindTexture2D ( GIuint index )
{
    int i = BIND_TEXTURE_2D;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Build2DMipmapsBGR ( int width, int height, const unsigned char * pData )
{
    int i = BUILD_2D_MIPMAPS_BGR_ARR;
    _write ( h, &i, 4 );
    _write ( h, &width, 4 );
    _write ( h, &height, 4 );
    _write ( h, pData, 3*width*height );
}

void FileWriter::Build2DMipmapsBGR_BTPC ( int lenght, const unsigned char * pData )
{
    int i = BUILD_2D_MIPMAPS_BGR_BTPC_ARR;
    _write ( h, &i, 4 );
    _write ( h, &lenght, 4 );
    _write ( h, pData, (lenght+3) & ~3 );
}

void FileWriter::GenTextures ( GIuint n, GIuint index )
{
    int i = GEN_TEXTURES;
    _write ( h, &i, 4 );
    _write ( h, &n, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::TexCoord2 ( float s, float t )
{
    int i = TEX_COORD2;
    _write ( h, &i, 4 );
    _write ( h, &s, 4 );
    _write ( h, &t, 4 );
}

void FileWriter::TexParameter2DMinFilterLinearMipmapNearest()
{
    int i = TEX_PAR_2D_MIN_FILTER_MIPMAP_NEARST;
    _write ( h, &i, 4 );
}

void FileWriter::TexParameter2DWrapSRepeat()
{
    int i = TEX_PAR_2D_WRAP_S_REPEAT;
    _write ( h, &i, 4 );
}

void FileWriter::TexParameter2DWrapTRepeat()
{
    int i = TEX_PAR_2D_WRAP_T_REPEAT;
    _write ( h, &i, 4 );
}

// Get

void FileWriter::GetFloatViewport ( GIuint index )
{
    int i = GET_FLOAT_VIEWPORT;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::GetIntViewport ( GIuint index )
{
    int i = GET_INT_VIEWPORT;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Quadric objects

void FileWriter::QuadricTexture ( bool b )
{
    int i = QUADRIC_TEXTURE;
    _write ( h, &i, 4 );
    i = b;
    _write ( h, &i, 4 );
}

void FileWriter::Disk ( float innerRadius, float outerRadius, int slices, int loops )
{
    int i = DISK;
    _write ( h, &i, 4 );
    _write ( h, &innerRadius, 4 );
    _write ( h, &outerRadius, 4 );
    _write ( h, &slices, 4 );
    _write ( h, &loops, 4 );
}

void FileWriter::Sphere ( float radius, int slices, int stakes )
{
    int i = SPHERE;
    _write ( h, &i, 4 );
    _write ( h, &radius, 4 );
    _write ( h, &slices, 4 );
    _write ( h, &stakes, 4 );
}

// Integer values

void FileWriter::IntAstDA ( GIuint index, int n, const int * p )
{
    int i = INT_AST_DA_ARR;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &n, 4 );
    _write ( h, p, 4*n );
}

void FileWriter::IntAstDC ( GIuint index, int p )
{
    int i = INT_AST_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntAstDD ( GIuint index1, GIuint index2 )
{
    int i = INT_AST_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntAstDI ( GIuint index1, GIuint index2 )
{
    int i = INT_AST_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntAddDC ( GIuint index, int p )
{
    int i = INT_ADD_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntAddDD ( GIuint index1, GIuint index2 )
{
    int i = INT_ADD_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntSubDC ( GIuint index, int p )
{
    int i = INT_SUB_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntSubDD ( GIuint index1, GIuint index2 )
{
    int i = INT_SUB_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntMulDC ( GIuint index, int p )
{
    int i = INT_MUL_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntMulDD ( GIuint index1, GIuint index2 )
{
    int i = INT_MUL_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntDivDC ( GIuint index, int p )
{
    int i = INT_DIV_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntDivDD ( GIuint index1, GIuint index2 )
{
    int i = INT_DIV_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::IntModDC ( GIuint index, int p )
{
    int i = INT_MOD_DC;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &p, 4 );
}

void FileWriter::IntModDD ( GIuint index1, GIuint index2 )
{
    int i = INT_MOD_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

// Float values

void FileWriter::FloatAstDA ( GIuint index, int n, const float * p )
{
    int i = FLOAT_AST_DA_ARR;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
    _write ( h, &n, 4 );
    _write ( h, p, 4*n );
}

void FileWriter::FloatAstDC ( GIuint u, float f )
{
    int i = FLOAT_AST_DC;
    _write ( h, &i, 4 );
    _write ( h, &u, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatAstDD ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_AST_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAstDI ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_AST_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAstIC ( GIuint u, float f )
{
    int i = FLOAT_AST_IC;
    _write ( h, &i, 4 );
    _write ( h, &u, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatAstID ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_AST_ID;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAstII ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_AST_II;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAddDC ( GIuint index1, float f )
{
    int i = FLOAT_ADD_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatAddDD ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_ADD_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAddDI ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_ADD_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAddIC ( GIuint index1, float f )
{
    int i = FLOAT_ADD_IC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatAddID ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_ADD_ID;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAddII ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_ADD_II;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatSubDC ( GIuint index1, float f )
{
    int i = FLOAT_SUB_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatSubDD ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_SUB_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatSubDI ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_SUB_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatSubIC ( GIuint index1, float f )
{
    int i = FLOAT_SUB_IC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatSubID ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_SUB_ID;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatSubII ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_SUB_II;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatMulDC ( GIuint index1, float f )
{
    int i = FLOAT_MUL_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatMulDD ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_MUL_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatMulDI ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_MUL_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatMulIC ( GIuint index1, float f )
{
    int i = FLOAT_MUL_IC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatMulID ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_MUL_ID;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatMulII ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_MUL_II;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatDivDC ( GIuint index1, float f )
{
    int i = FLOAT_DIV_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatDivDD ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_DIV_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatDivDI ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_DIV_DI;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatDivIC ( GIuint index1, float f )
{
    int i = FLOAT_DIV_IC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &f, 4 );
}

void FileWriter::FloatDivID ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_DIV_ID;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatDivII ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_DIV_II;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

// Mix commands

void FileWriter::IntAstFromFloat ( GIuint index1, GIuint index2 )
{
    int i = INT_AST_FROM_FLOAT;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatAstFromInt ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_AST_FROM_INT;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

// Conditional commands

bool FileWriter::IfIntDC ( GIuint index1, Relation rel, int p )
{
    int i = IF_INT_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel   , 4 );
    _write ( h, &p, 4 );
    return 1;
}

bool FileWriter::IfIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    int i = IF_INT_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel   , 4 );
    _write ( h, &index2, 4 );
    return 1;
}

bool FileWriter::IfFloatDC ( GIuint index1, Relation rel, float f )
{
    int i = IF_FLOAT_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel   , 4 );
    _write ( h, &f, 4 );
    return 1;
}

bool FileWriter::IfFloatDD ( GIuint index1, Relation rel, GIuint index2 )
{
    int i = IF_FLOAT_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel   , 4 );
    _write ( h, &index2, 4 );
    return 1;
}

bool FileWriter::Else ()
{
    int i = ELSE;
    _write ( h, &i, 4 );
    return 1;
}

void FileWriter::EndIf ()
{
    int i = END_IF;
    _write ( h, &i, 4 );
}

// Loop

void FileWriter::Do ()
{
    int i = DO;
    _write ( h, &i, 4 );
}

bool FileWriter::WhileIntDC ( GIuint index1, Relation rel, int p )
{
    int i = WHILE_INT_DC;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel, 4 );
    _write ( h, &p, 4 );
    return false;
}

bool FileWriter::WhileIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    int i = WHILE_INT_DD;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &rel, 4 );
    _write ( h, &index2, 4 );
    return false;
}

// Mathematics

void FileWriter::FloatAbs ( GIuint index )
{
    int i = FLOAT_ABS;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::FloatAtan2 ( GIuint index1, GIuint index2 )
{
    int i = FLOAT_ATAN2;
    _write ( h, &i, 4 );
    _write ( h, &index1, 4 );
    _write ( h, &index2, 4 );
}

void FileWriter::FloatCos ( GIuint index )
{
    int i = FLOAT_COS;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::FloatSin ( GIuint index )
{
    int i = FLOAT_SIN;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::FloatSqrt ( GIuint index )
{
    int i = FLOAT_SQRT;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

// Start position

void FileWriter::NextStart ( const GraphicInterface::Node * node )
{
    int i = NEXT_START;
    _write ( h, &i, 4 );
}

bool FileWriter::NextStartAndReturn ( const GraphicInterface::Node * node )
{
    int i = NEXT_START_AND_RETURN;
    _write ( h, &i, 4 );
    return 0;
}

void FileWriter::NextStartFirst ()
{
    int i = NEXT_START_FIRST;
    _write ( h, &i, 4 );
}

// Time

void FileWriter::Clock ( GIuint index )
{
    int i = CLOCK;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::Time ( GIuint index )
{
    int i = TIME;
    _write ( h, &i, 4 );
    _write ( h, &index, 4 );
}

void FileWriter::SetTimer ( int p )
{
    int i = SET_TIMER;
    _write ( h, &i, 4 );
    _write ( h, &p, 4 );
}
