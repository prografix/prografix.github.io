
#include "stdafx.h"
#include "GraphicInterface.h"

static void dummy ()
{
    AfxMessageBox ( "This GraphicInterface function is not realized!" );
}

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                   Graphical commands                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////

// Clear
void GraphicInterface::ClearColor ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::ClearColorBuffer  () { dummy(); }
void GraphicInterface::ClearDepthBuffer  () { dummy(); }
void GraphicInterface::ClearStencilBuffer() { dummy(); }
// Enable
void GraphicInterface::EnableBlend         () { dummy(); }
void GraphicInterface::EnableColorMaterial () { dummy(); }
void GraphicInterface::EnableCullFace      () { dummy(); }
void GraphicInterface::EnableDepthTest     () { dummy(); }
void GraphicInterface::EnableLight    (Light) { dummy(); }
void GraphicInterface::EnableLighting      () { dummy(); }
void GraphicInterface::EnableLineStipple   () { dummy(); }
void GraphicInterface::EnableNormalize     () { dummy(); }
void GraphicInterface::EnablePolygonStipple() { dummy(); }
void GraphicInterface::EnableStencilTest   () { dummy(); }
void GraphicInterface::EnableTexture2D     () { dummy(); }
void GraphicInterface::EnableTextureGenS   () { dummy(); }
void GraphicInterface::EnableTextureGenT   () { dummy(); }
// Disable
void GraphicInterface::DisableBlend         () { dummy(); }
void GraphicInterface::DisableColorMaterial () { dummy(); }
void GraphicInterface::DisableCullFace      () { dummy(); }
void GraphicInterface::DisableDepthTest     () { dummy(); }
void GraphicInterface::DisableLight    (Light) { dummy(); }
void GraphicInterface::DisableLighting      () { dummy(); }
void GraphicInterface::DisableLineStipple   () { dummy(); }
void GraphicInterface::DisableNormalize     () { dummy(); }
void GraphicInterface::DisablePolygonStipple() { dummy(); }
void GraphicInterface::DisableStencilTest   () { dummy(); }
void GraphicInterface::DisableTexture2D     () { dummy(); }
void GraphicInterface::DisableTextureGenS   () { dummy(); }
void GraphicInterface::DisableTextureGenT   () { dummy(); }
// Blend func
void GraphicInterface::BlendFunc ( BlendFuncPar src, BlendFuncPar dst ) { dummy(); }
// Depth func
void GraphicInterface::DepthFuncNever   () { dummy(); }
void GraphicInterface::DepthFuncLess    () { dummy(); }
void GraphicInterface::DepthFuncLEqual  () { dummy(); }
void GraphicInterface::DepthFuncEqual   () { dummy(); }
void GraphicInterface::DepthFuncGreater () { dummy(); }
void GraphicInterface::DepthFuncNotEqual() { dummy(); }
void GraphicInterface::DepthFuncGEqual  () { dummy(); }
void GraphicInterface::DepthFuncAlways  () { dummy(); }
// Stencil func
void GraphicInterface::StencilFuncNever   ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncLess    ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncLEqual  ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncEqual   ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncGreater ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncNotEqual( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncGEqual  ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilFuncAlways  ( GIuint ref, GIuint mask ) { dummy(); }
void GraphicInterface::StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass ) { dummy(); }
// Begin, End
void GraphicInterface::BeginPoints       () { dummy(); }
void GraphicInterface::BeginLines        () { dummy(); }
void GraphicInterface::BeginLineStrip    () { dummy(); }
void GraphicInterface::BeginLineLoop     () { dummy(); }
void GraphicInterface::BeginTriangles    () { dummy(); }
void GraphicInterface::BeginTriangleStrip() { dummy(); }
void GraphicInterface::BeginTriangleFan  () { dummy(); }
void GraphicInterface::BeginQuads        () { dummy(); }
void GraphicInterface::BeginQuadStrip    () { dummy(); }
void GraphicInterface::BeginPolygon      () { dummy(); }
void GraphicInterface::End               () { dummy(); }
// Cull face
void GraphicInterface::CullFaceBack        () { dummy(); }
void GraphicInterface::CullFaceFront       () { dummy(); }
void GraphicInterface::CullFaceFrontAndBack() { dummy(); }
// Light
void GraphicInterface::LightAmbient   ( Light, float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::LightDiffuse   ( Light, float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::LightSpecular  ( Light, float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::LightPosition  ( Light, float x, float y, float z, float w ) { dummy(); }
void GraphicInterface::LightPosition4v( Light, GIuint index ) { dummy(); }
void GraphicInterface::LightQuadraticAttenuation ( Light, float ) { dummy(); }
// Stipple
void GraphicInterface::LineStipple ( int factor, int pattern ) { dummy(); }
void GraphicInterface::PolygonStipple ( const unsigned char mask[128] ) { dummy(); }
// Material
void GraphicInterface::MaterialBackAmbient  ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialBackDiffuse  ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialBackEmission ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialBackSpecular ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialBackShininess( float ) { dummy(); }
void GraphicInterface::MaterialFrontAmbient  ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialFrontDiffuse  ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialFrontEmission ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialFrontSpecular ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::MaterialFrontShininess( float ) { dummy(); }
// Polygon mode
void GraphicInterface::PolygonMode ( Face face, enum PolygonMode mode ) { dummy(); }
// Color
void GraphicInterface::Color ( float r, float g, float b ) { dummy(); }
void GraphicInterface::Color ( float r, float g, float b, float a ) { dummy(); }
void GraphicInterface::Color3v ( GIuint index ) { dummy(); }
void GraphicInterface::Color4v ( GIuint index ) { dummy(); }
// Normal
void GraphicInterface::Normal ( float x, float y, float z ) { dummy(); }
void GraphicInterface::Normal3vI ( GIuint index ) { dummy(); }
// Vertex
void GraphicInterface::Vertex ( float x, float y ) { dummy(); }
void GraphicInterface::Vertex ( float x, float y, float z ) { dummy(); }
void GraphicInterface::Vertex2v ( GIuint index ) { dummy(); }
void GraphicInterface::Vertex3v ( GIuint index ) { dummy(); }
void GraphicInterface::Vertex2vI ( GIuint index ) { dummy(); }
void GraphicInterface::Vertex3vI ( GIuint index ) { dummy(); }
// Display list
void GraphicInterface::CallList ( int ) { dummy(); }
void GraphicInterface::CallListIndex ( GIuint ) { dummy(); }
void GraphicInterface::EndList () { dummy(); }
void GraphicInterface::NewListCompile ( int ) { dummy(); }
void GraphicInterface::NewListCompileAndExecute ( int ) { dummy(); }
// Matrix
void GraphicInterface::Frustum4v ( GIuint index ) { dummy(); }
void GraphicInterface::LoadIdentity() { dummy(); }
void GraphicInterface::LookAt ( GIuint index ) { dummy(); }
void GraphicInterface::MatrixModeModelView() { dummy(); }
void GraphicInterface::MatrixModeProjection() { dummy(); }
void GraphicInterface::MatrixModeTexture() { dummy(); }
void GraphicInterface::MultMatrix ( GIuint index ) { dummy(); }
void GraphicInterface::Perspective ( float fovy, float aspect, float zNear, float zFar ) { dummy(); }
void GraphicInterface::Perspective4v ( GIuint index ) { dummy(); }
void GraphicInterface::PopMatrix () { dummy(); }
void GraphicInterface::PushMatrix() { dummy(); }
void GraphicInterface::Rotate ( float angle, float x, float y, float z ) { dummy(); }
void GraphicInterface::Rotate4v ( GIuint index ) { dummy(); }
void GraphicInterface::Scale ( float x, float y, float z ) { dummy(); }
void GraphicInterface::Scale3v ( GIuint index ) { dummy(); }
void GraphicInterface::Translate ( float x, float y, float z ) { dummy(); }
void GraphicInterface::Translate3v ( GIuint index ) { dummy(); }
// Texture
void GraphicInterface::BindTexture2D ( GIuint index ) { dummy(); }
void GraphicInterface::Build2DMipmapsBGR ( int width, int height, const unsigned char * pData ) { dummy(); }
void GraphicInterface::Build2DMipmapsBGR_BTPC ( int length, const unsigned char * pData ) { dummy(); }
void GraphicInterface::GenTextures ( GIuint n, GIuint index ) { dummy(); }
void GraphicInterface::TexCoord2 ( float s, float t ) { dummy(); }
void GraphicInterface::TexParameter2DMinFilterLinearMipmapNearest() { dummy(); }
void GraphicInterface::TexParameter2DWrapSRepeat () { dummy(); }
void GraphicInterface::TexParameter2DWrapTRepeat () { dummy(); }
// Get
void GraphicInterface::GetFloatModelViewMatrix ( GIuint index ) { dummy(); }
void GraphicInterface::GetFloatProjectionMatrix ( GIuint index ) { dummy(); }
void GraphicInterface::GetFloatViewport ( GIuint index ) { dummy(); }
void GraphicInterface::GetIntViewport ( GIuint index ) { dummy(); }
// Quadric objects
void GraphicInterface::QuadricTexture ( bool ) { dummy(); }
void GraphicInterface::Disk ( float innerRadius, float outerRadius, int slices, int loops ) { dummy(); }
void GraphicInterface::Sphere ( float radius, int slices, int stakes ) { dummy(); }

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 Non graphical commands                          //
//                                                                 //
/////////////////////////////////////////////////////////////////////

// Integer commands
void GraphicInterface::IntAstDA ( GIuint index, int n, const int * ) { dummy(); }
void GraphicInterface::IntAstDC ( GIuint index, int i          ) { dummy(); } // =
void GraphicInterface::IntAstDD ( GIuint index1, GIuint index2 ) { dummy(); } // =
void GraphicInterface::IntAstDI ( GIuint index1, GIuint index2 ) { dummy(); } // =

void GraphicInterface::IntAddDC ( GIuint index1, int i         ) { dummy(); } // +=
void GraphicInterface::IntAddDD ( GIuint index1, GIuint index2 ) { dummy(); } // +=

void GraphicInterface::IntSubDC ( GIuint index1, int i         ) { dummy(); } // -=
void GraphicInterface::IntSubDD ( GIuint index1, GIuint index2 ) { dummy(); } // -=

void GraphicInterface::IntMulDC ( GIuint index1, int i         ) { dummy(); } // *=
void GraphicInterface::IntMulDD ( GIuint index1, GIuint index2 ) { dummy(); } // *=

void GraphicInterface::IntDivDC ( GIuint index1, int i         ) { dummy(); } // /=
void GraphicInterface::IntDivDD ( GIuint index1, GIuint index2 ) { dummy(); } // /=

void GraphicInterface::IntModDC ( GIuint index1, int i         ) { dummy(); } // %=
void GraphicInterface::IntModDD ( GIuint index1, GIuint index2 ) { dummy(); } // %=
// Float commands
void GraphicInterface::FloatAstDA ( GIuint index, int n, const float * ) { dummy(); }
void GraphicInterface::FloatAstDC ( GIuint index1, float f       ) { dummy(); } // =
void GraphicInterface::FloatAstDD ( GIuint index1, GIuint index2 ) { dummy(); } // =
void GraphicInterface::FloatAstDI ( GIuint index1, GIuint index2 ) { dummy(); } // =
void GraphicInterface::FloatAstIC ( GIuint index1, float f       ) { dummy(); } // =
void GraphicInterface::FloatAstID ( GIuint index1, GIuint index2 ) { dummy(); } // =
void GraphicInterface::FloatAstII ( GIuint index1, GIuint index2 ) { dummy(); } // =

void GraphicInterface::FloatAddDC ( GIuint index1, float f       ) { dummy(); } // +=
void GraphicInterface::FloatAddDD ( GIuint index1, GIuint index2 ) { dummy(); } // +=
void GraphicInterface::FloatAddDI ( GIuint index1, GIuint index2 ) { dummy(); } // +=
void GraphicInterface::FloatAddIC ( GIuint index1, float f       ) { dummy(); } // +=
void GraphicInterface::FloatAddID ( GIuint index1, GIuint index2 ) { dummy(); } // +=
void GraphicInterface::FloatAddII ( GIuint index1, GIuint index2 ) { dummy(); } // +=

void GraphicInterface::FloatSubDC ( GIuint index1, float f       ) { dummy(); } // -=
void GraphicInterface::FloatSubDD ( GIuint index1, GIuint index2 ) { dummy(); } // -=
void GraphicInterface::FloatSubDI ( GIuint index1, GIuint index2 ) { dummy(); } // -=
void GraphicInterface::FloatSubIC ( GIuint index1, float f       ) { dummy(); } // -=
void GraphicInterface::FloatSubID ( GIuint index1, GIuint index2 ) { dummy(); } // -=
void GraphicInterface::FloatSubII ( GIuint index1, GIuint index2 ) { dummy(); } // -=

void GraphicInterface::FloatMulDC ( GIuint index1, float f       ) { dummy(); } // *=
void GraphicInterface::FloatMulDD ( GIuint index1, GIuint index2 ) { dummy(); } // *=
void GraphicInterface::FloatMulDI ( GIuint index1, GIuint index2 ) { dummy(); } // *=
void GraphicInterface::FloatMulIC ( GIuint index1, float f       ) { dummy(); } // *=
void GraphicInterface::FloatMulID ( GIuint index1, GIuint index2 ) { dummy(); } // *=
void GraphicInterface::FloatMulII ( GIuint index1, GIuint index2 ) { dummy(); } // *=

void GraphicInterface::FloatDivDC ( GIuint index1, float f       ) { dummy(); } // /=
void GraphicInterface::FloatDivDD ( GIuint index1, GIuint index2 ) { dummy(); } // /=
void GraphicInterface::FloatDivDI ( GIuint index1, GIuint index2 ) { dummy(); } // /=
void GraphicInterface::FloatDivIC ( GIuint index1, float f       ) { dummy(); } // /=
void GraphicInterface::FloatDivID ( GIuint index1, GIuint index2 ) { dummy(); } // /=
void GraphicInterface::FloatDivII ( GIuint index1, GIuint index2 ) { dummy(); } // /=
// Mix commands
void GraphicInterface::IntAstFromFloat ( GIuint index1, GIuint index2 ) { dummy(); } // =
void GraphicInterface::FloatAstFromInt ( GIuint index1, GIuint index2 ) { dummy(); } // =
// Conditional commands
bool GraphicInterface::IfIntDC   ( GIuint index1, Relation rel, int i ) { dummy(); return 0; }
bool GraphicInterface::IfIntDD   ( GIuint index1, Relation rel, GIuint index2 ) { dummy(); return 0; }

bool GraphicInterface::IfFloatDC ( GIuint index1, Relation rel, float f ) { dummy(); return 0; }
bool GraphicInterface::IfFloatDD ( GIuint index1, Relation rel, GIuint index2 ) { dummy(); return 0; }

bool GraphicInterface::Else  () { dummy(); return 0; }
void GraphicInterface::EndIf () { dummy(); }
// Loop
void GraphicInterface::Do () { dummy(); }
bool GraphicInterface::WhileIntDC ( GIuint index1, Relation rel, int i ) { dummy(); return false; }
bool GraphicInterface::WhileIntDD ( GIuint index1, Relation rel, GIuint index2 ) { dummy(); return false; }
// Mathematics
void GraphicInterface::FloatAbs   ( GIuint ) { dummy(); }
void GraphicInterface::FloatAtan2 ( GIuint, GIuint ) { dummy(); }
void GraphicInterface::FloatCos   ( GIuint ) { dummy(); }
void GraphicInterface::FloatSin   ( GIuint ) { dummy(); }
void GraphicInterface::FloatSqrt  ( GIuint ) { dummy(); }
// Start position
void GraphicInterface::NextStart ( const Node * node ) { dummy(); }
bool GraphicInterface::NextStartAndReturn ( const Node * node ) { dummy(); return 0; }
void GraphicInterface::NextStartFirst () { dummy(); }
// Time
void GraphicInterface::Clock ( GIuint ) { dummy(); }
void GraphicInterface::Time  ( GIuint ) { dummy(); }
void GraphicInterface::SetTimer ( int ) { dummy(); }
// Destructor
GraphicInterface::~GraphicInterface () {}

const GraphicInterface::Node * GraphicInterface::getFirstNode() const { return 0; }
const GraphicInterface::Node * GraphicInterface::getFreeNode () const { return 0; }
const GraphicInterface::Node * GraphicInterface::getNextStart() const { return 0; }

bool GraphicInterface::operator >> ( GraphicInterface & gi ) const
{
    const Node * loopPtr[8];
    int loopNum = -1;
    const Node * node = gi.getNextStart();
    if ( node == 0 ) node = getFirstNode();
    const Node * freeNode = getFreeNode();
    while ( node != freeNode )
    {
        switch ( node->code )
        {
// Clear
        case CLEAR_COLOR:
            gi.ClearColor ( node[1].f, node[2].f, node[3].f, node[4].f );
            node += 5; break;
        case CLEAR_COLOR_BUFFER:
            gi.ClearColorBuffer(); ++node; break;
        case CLEAR_DEPTH_BUFFER:
            gi.ClearDepthBuffer(); ++node; break;
        case CLEAR_STENCIL_BUFFER:
            gi.ClearStencilBuffer(); ++node; break;
// Enable
        case ENABLE_BLEND:
            gi.EnableBlend(); ++node; break;
        case ENABLE_CULL_FACE:
            gi.EnableCullFace(); ++node; break;
        case ENABLE_DEPTH_TEST:
            gi.EnableDepthTest(); ++node; break;
        case ENABLE_LIGHT:
            gi.EnableLight(node[1].light); node+=2; break;
        case ENABLE_LIGHTING:
            gi.EnableLighting(); ++node; break;
        case ENABLE_LINE_STIPPLE:
            gi.EnableLineStipple(); ++node; break;
        case ENABLE_NORMALIZE:
            gi.EnableNormalize(); ++node; break;
        case ENABLE_POLYGON_STIPPLE:
            gi.EnablePolygonStipple(); ++node; break;
        case ENABLE_STENCIL_TEST:
            gi.EnableStencilTest(); ++node; break;
        case ENABLE_TEXTURE_2D:
            gi.EnableTexture2D(); ++node; break;
        case ENABLE_TEXTURE_GEN_S:
            gi.EnableTextureGenS(); ++node; break;
        case ENABLE_TEXTURE_GEN_T:
            gi.EnableTextureGenT(); ++node; break;
// Disable
        case DISABLE_BLEND:
            gi.DisableBlend(); ++node; break;
        case DISABLE_CULL_FACE:
            gi.DisableCullFace(); ++node; break;
        case DISABLE_DEPTH_TEST:
            gi.DisableDepthTest(); ++node; break;
        case DISABLE_LIGHT:
            gi.DisableLight(node[1].light); node+=2; break;
        case DISABLE_LIGHTING:
            gi.DisableLighting(); ++node; break;
        case DISABLE_LINE_STIPPLE:
            gi.DisableLineStipple(); ++node; break;
        case DISABLE_NORMALIZE:
            gi.DisableNormalize(); ++node; break;
        case DISABLE_POLYGON_STIPPLE:
            gi.DisablePolygonStipple(); ++node; break;
        case DISABLE_STENCIL_TEST:
            gi.DisableStencilTest(); ++node; break;
        case DISABLE_TEXTURE_2D:
            gi.DisableTexture2D(); ++node; break;
        case DISABLE_TEXTURE_GEN_S:
            gi.DisableTextureGenS(); ++node; break;
        case DISABLE_TEXTURE_GEN_T:
            gi.DisableTextureGenT(); ++node; break;
// Blend func
        case BLEND_FUNC:
            gi.BlendFunc(node[1].bfp, node[2].bfp); node+=3; break;
// Depth func
        case DEPTH_FUNC_NEVER:
            gi.DepthFuncNever(); ++node; break;
        case DEPTH_FUNC_LESS:
            gi.DepthFuncLess(); ++node; break;
        case DEPTH_FUNC_LEQUAL:
            gi.DepthFuncLEqual(); ++node; break;
        case DEPTH_FUNC_EQUAL:
            gi.DepthFuncEqual(); ++node; break;
        case DEPTH_FUNC_GREATER:
            gi.DepthFuncGreater(); ++node; break;
        case DEPTH_FUNC_NOTEQUAL:
            gi.DepthFuncNotEqual(); ++node; break;
        case DEPTH_FUNC_GEQUAL:
            gi.DepthFuncGEqual(); ++node; break;
        case DEPTH_FUNC_ALWAYS:
            gi.DepthFuncAlways(); ++node; break;
// Stencil func
        case STENCIL_FUNC_ALWAYS:
            gi.StencilFuncAlways(node[1].i, node[2].i); node+=3; break;
        case STENCIL_FUNC_EQUAL:
            gi.StencilFuncEqual(node[1].i, node[2].i); node+=3; break;
        case STENCIL_OP:
            gi.StencilOp(node[1].sop, node[2].sop, node[3].sop); node+=4; break;
// Begin, End
        case BEGIN_POINTS:
            gi.BeginPoints(); ++node; break;
        case BEGIN_LINES:
            gi.BeginLines(); ++node; break;
        case BEGIN_LINE_STRIP:
            gi.BeginLineStrip(); ++node; break;
        case BEGIN_LINE_LOOP:
            gi.BeginLineLoop(); ++node; break;
        case BEGIN_TRIANGLES:
            gi.BeginTriangles(); ++node; break;
        case BEGIN_TRIANGLE_STRIP:
            gi.BeginTriangleStrip(); ++node; break;
        case BEGIN_TRIANGLE_FAN:
            gi.BeginTriangleFan(); ++node; break;
        case BEGIN_QUADS:
            gi.BeginQuads(); ++node; break;
        case BEGIN_QUAD_STRIP:
            gi.BeginQuadStrip(); ++node; break;
        case BEGIN_POLYGON:
            gi.BeginPolygon(); ++node; break;
        case END:
            gi.End(); ++node; break;
// Cull face
        case CULL_FACE_BACK:
            gi.CullFaceBack (); ++node; break;
        case CULL_FACE_FRONT:
            gi.CullFaceFront (); ++node; break;
        case CULL_FACE_FRONT_AND_BACK:
            gi.CullFaceFrontAndBack (); ++node; break;
// Light
        case LIGHT_AMBIENT:
            gi.LightAmbient(node[1].light, node[2].f, node[3].f, node[4].f, node[5].f); node+=6; break;
        case LIGHT_DIFFUSE:
            gi.LightDiffuse(node[1].light, node[2].f, node[3].f, node[4].f, node[5].f); node+=6; break;
        case LIGHT_SPECULAR:
            gi.LightSpecular(node[1].light, node[2].f, node[3].f, node[4].f, node[5].f); node+=6; break;
        case LIGHT_POSITION:
            gi.LightPosition(node[1].light, node[2].f, node[3].f, node[4].f, node[5].f); node+=6; break;
        case LIGHT_POSITION_4V:
            gi.LightPosition4v(node[1].light, node[2].i); node+=3; break;
// Stipple
        case LINE_STIPPLE:
            gi.LineStipple(node[1].i, node[2].i); node+=3; break;
        case POLYGON_STIPPLE_ARR:
            gi.PolygonStipple(&node[1].uc); node+=1+32; break;
        case POLYGON_STIPPLE_PTR:
            gi.PolygonStipple(node[1].ptr); node+=2; break;
// Material
        case MATERIAL_BACK_AMBIENT:
            gi.MaterialBackAmbient(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_BACK_DIFFUSE:
            gi.MaterialBackDiffuse(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_BACK_EMISSION:
            gi.MaterialBackEmission(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_BACK_SPECULAR:
            gi.MaterialBackSpecular(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_BACK_SHININESS:
            gi.MaterialBackShininess(node[1].f); node+=2; break;
        case MATERIAL_FRONT_AMBIENT:
            gi.MaterialFrontAmbient(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_FRONT_DIFFUSE:
            gi.MaterialFrontDiffuse(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_FRONT_EMISSION:
            gi.MaterialFrontEmission(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_FRONT_SPECULAR:
            gi.MaterialFrontSpecular(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case MATERIAL_FRONT_SHININESS:
            gi.MaterialFrontShininess(node[1].f); node+=2; break;
// Polygon mode
        case POLYGON_MODE:
            gi.PolygonMode ( node[1].face, node[2].mode ); node+=3; break;
// Color
        case COLOR3:
            gi.Color(node[1].f, node[2].f, node[3].f); node+=4; break;
        case COLOR4:
            gi.Color(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case COLOR_3V:
            gi.Color3v(node[1].i); node+=2; break;
        case COLOR_4V:
            gi.Color4v(node[1].i); node+=2; break;
// Normal
        case NORMAL:
            gi.Normal(node[1].f, node[2].f, node[3].f); node+=4; break;
// Vertex
        case VERTEX2:
            gi.Vertex(node[1].f, node[2].f); node+=3; break;
        case VERTEX3:
            gi.Vertex(node[1].f, node[2].f, node[3].f); node+=4; break;
        case VERTEX_2V:
            gi.Vertex2v(node[1].i); node+=2; break;
        case VERTEX_3V:
            gi.Vertex3v(node[1].i); node+=2; break;
// Display list
        case CALL_LIST:
            gi.CallList(node[1].i); node+=2; break;
        case CALL_LIST_INDEX:
            gi.CallListIndex(node[1].i); node+=2; break;
        case END_LIST:
            gi.EndList(); ++node; break;
        case NEW_LIST_COMPILE:
            gi.NewListCompile(node[1].i); node+=2; break;
        case NEW_LIST_COMPILE_AND_EXECUTE:
            gi.NewListCompileAndExecute(node[1].i); node+=2; break;
// Matrix
        case LOAD_IDENTITY:
            gi.LoadIdentity(); ++node; break;
        case LOOK_AT:
            gi.LookAt(node[1].i); node+=2; break;
        case MATRIX_MODE_MODEL_VIEW:
            gi.MatrixModeModelView(); ++node; break;
        case MATRIX_MODE_PROJECTION:
            gi.MatrixModeProjection(); ++node; break;
        case MATRIX_MODE_TEXTURE:
            gi.MatrixModeTexture(); ++node; break;
        case MULT_MATRIX:
            gi.MultMatrix(node[1].i); node+=2; break;
        case PERSPECTIVE:
            gi.Perspective(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case PERSPECTIVE_4V:
            gi.Perspective4v(node[1].i); node+=2; break;
        case POP_MATRIX:
            gi.PopMatrix(); ++node; break;
        case PUSH_MATRIX:
            gi.PushMatrix(); ++node; break;
        case ROTATE:
            gi.Rotate(node[1].f, node[2].f, node[3].f, node[4].f); node+=5; break;
        case ROTATE_4V:
            gi.Rotate4v(node[1].i); node+=2; break;
        case SCALE:
            gi.Scale(node[1].f, node[2].f, node[3].f); node+=4; break;
        case SCALE_3V:
            gi.Scale3v(node[1].i); node+=2; break;
        case TRANSLATE:
            gi.Translate(node[1].f, node[2].f, node[3].f); node+=4; break;
        case TRANSLATE_3V:
            gi.Translate3v(node[1].i); node+=2; break;
// Texture
        case BIND_TEXTURE_2D:
            gi.BindTexture2D(node[1].i); node+=2; break;
        case BUILD_2D_MIPMAPS_BGR_ARR:
            gi.Build2DMipmapsBGR(node[1].i, node[2].i, &node[3].uc); node+=3+3*node[1].i*node[2].i/4; break;
        case BUILD_2D_MIPMAPS_BGR_PTR:
            gi.Build2DMipmapsBGR(node[1].i, node[2].i, node[3].ptr); node+=4; break;
        case GEN_TEXTURES:
            gi.GenTextures(node[1].i, node[2].i); node+=3; break;
        case TEX_COORD2:
            gi.TexCoord2(node[1].f, node[2].f); node+=3; break;
        case TEX_PAR_2D_MIN_FILTER_MIPMAP_NEARST:
            gi.TexParameter2DMinFilterLinearMipmapNearest(); ++node; break;
        case TEX_PAR_2D_WRAP_S_REPEAT:
            gi.TexParameter2DWrapSRepeat(); ++node; break;
        case TEX_PAR_2D_WRAP_T_REPEAT:
            gi.TexParameter2DWrapTRepeat(); ++node; break;
// Get
        case GET_FLOAT_VIEWPORT:
            gi.GetFloatViewport(node[1].i); node+=2; break;
        case GET_INT_VIEWPORT:
            gi.GetIntViewport(node[1].i); node+=2; break;
// Quadric objects
        case QUADRIC_TEXTURE:
            gi.QuadricTexture(node[1].b); node+=2; break;
        case SPHERE:
            gi.Sphere(node[1].f, node[2].i, node[3].i); node+=4; break;
// Int commands
        case INT_AST_DC:
            gi.IntAstDC(node[1].i, node[2].i); node+=3; break;
        case INT_AST_DD:
            gi.IntAstDD(node[1].i, node[2].i); node+=3; break;
        case INT_ADD_DC:
            gi.IntAddDC(node[1].i, node[2].i); node+=3; break;
        case INT_ADD_DD:
            gi.IntAddDD(node[1].i, node[2].i); node+=3; break;
        case INT_SUB_DC:
            gi.IntSubDC(node[1].i, node[2].i); node+=3; break;
        case INT_SUB_DD:
            gi.IntSubDD(node[1].i, node[2].i); node+=3; break;
        case INT_MUL_DC:
            gi.IntMulDC(node[1].i, node[2].i); node+=3; break;
        case INT_MUL_DD:
            gi.IntMulDD(node[1].i, node[2].i); node+=3; break;
        case INT_DIV_DC:
            gi.IntDivDC(node[1].i, node[2].i); node+=3; break;
        case INT_DIV_DD:
            gi.IntDivDD(node[1].i, node[2].i); node+=3; break;
        case INT_MOD_DC:
            gi.IntModDC(node[1].i, node[2].i); node+=3; break;
        case INT_MOD_DD:
            gi.IntModDD(node[1].i, node[2].i); node+=3; break;
// Float commands
        case FLOAT_AST_DC:
            gi.FloatAstDC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_AST_DD:
            gi.FloatAstDD(node[1].i, node[2].i); node+=3; break;
        case FLOAT_AST_DI:
            gi.FloatAstDI(node[1].i, node[2].i); node+=3; break;
        case FLOAT_AST_IC:
            gi.FloatAstIC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_AST_ID:
            gi.FloatAstID(node[1].i, node[2].i); node+=3; break;
        case FLOAT_AST_II:
            gi.FloatAstII(node[1].i, node[2].i); node+=3; break;

        case FLOAT_ADD_DC:
            gi.FloatAddDC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_ADD_DD:
            gi.FloatAddDD(node[1].i, node[2].i); node+=3; break;
        case FLOAT_ADD_DI:
            gi.FloatAddDI(node[1].i, node[2].i); node+=3; break;
        case FLOAT_ADD_IC:
            gi.FloatAddIC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_ADD_ID:
            gi.FloatAddID(node[1].i, node[2].i); node+=3; break;
        case FLOAT_ADD_II:
            gi.FloatAddII(node[1].i, node[2].i); node+=3; break;

        case FLOAT_SUB_DC:
            gi.FloatSubDC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_SUB_DD:
            gi.FloatSubDD(node[1].i, node[2].i); node+=3; break;
        case FLOAT_SUB_DI:
            gi.FloatSubDI(node[1].i, node[2].i); node+=3; break;
        case FLOAT_SUB_IC:
            gi.FloatSubIC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_SUB_ID:
            gi.FloatSubID(node[1].i, node[2].i); node+=3; break;
        case FLOAT_SUB_II:
            gi.FloatSubII(node[1].i, node[2].i); node+=3; break;

        case FLOAT_MUL_DC:
            gi.FloatMulDC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_MUL_DD:
            gi.FloatMulDD(node[1].i, node[2].i); node+=3; break;
        case FLOAT_MUL_DI:
            gi.FloatMulDI(node[1].i, node[2].i); node+=3; break;
        case FLOAT_MUL_IC:
            gi.FloatMulIC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_MUL_ID:
            gi.FloatMulID(node[1].i, node[2].i); node+=3; break;
        case FLOAT_MUL_II:
            gi.FloatMulII(node[1].i, node[2].i); node+=3; break;

        case FLOAT_DIV_DC:
            gi.FloatDivDC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_DIV_DD:
            gi.FloatDivDD(node[1].i, node[2].i); node+=3; break;
        case FLOAT_DIV_DI:
            gi.FloatDivDI(node[1].i, node[2].i); node+=3; break;
        case FLOAT_DIV_IC:
            gi.FloatDivIC(node[1].i, node[2].f); node+=3; break;
        case FLOAT_DIV_ID:
            gi.FloatDivID(node[1].i, node[2].i); node+=3; break;
        case FLOAT_DIV_II:
            gi.FloatDivII(node[1].i, node[2].i); node+=3; break;
// Mix commands
        case INT_AST_FROM_FLOAT:
            gi.IntAstFromFloat(node[1].i, node[2].i); node+=3; break;
        case FLOAT_AST_FROM_INT:
            gi.FloatAstFromInt(node[1].i, node[2].i); node+=3; break;
// Conditional commands
        case IF_INT_DC:
            {
                if ( gi.IfIntDC(node[1].i, node[2].rel, node[3].i) )
                    node += 4;
                else
                    node = pass ( node + 4 );
                break;
            }
        case IF_INT_DD:
            {
                if ( gi.IfIntDD(node[1].i, node[2].rel, node[3].i) )
                    node += 4;
                else
                    node = pass ( node + 4 );
                break;
            }
        case IF_FLOAT_DC:
            {
                if ( gi.IfFloatDC(node[1].i, node[2].rel, node[3].f) )
                     node += 4;
                else
                    node = pass ( node + 4 );
                break;
            }
        case IF_FLOAT_DD:
            {
                if ( gi.IfFloatDD(node[1].i, node[2].rel, node[3].i) )
                     node += 4;
                else
                    node = pass ( node + 4 );
                break;
            }
        case ELSE:
            {
                if ( gi.Else() )
                     ++node;
                else
                    node = pass ( node + 1 );
                break;
            }
        case END_IF:
            gi.EndIf(); ++node; break;
// Loop
        case DO:
            gi.Do(); loopPtr[++loopNum] = ++node; break;
        case WHILE_INT_DC:
            {
                if ( gi.WhileIntDC(node[1].i, node[2].rel, node[3].i) )
                    node = loopPtr[loopNum];
                else
                    node += 4, --loopNum; break;
            }
        case WHILE_INT_DD:
            {
                if ( gi.WhileIntDD(node[1].i, node[2].rel, node[3].i) )
                    node = loopPtr[loopNum];
                else
                    node += 4, --loopNum; break;
            }
// Mathematics
        case FLOAT_ABS:
            gi.FloatAbs(node[1].i); node+=2; break;
        case FLOAT_ATAN2:
            gi.FloatAtan2(node[1].i, node[2].i); node+=3; break;
        case FLOAT_COS:
            gi.FloatCos(node[1].i); node+=2; break;
        case FLOAT_SIN:
            gi.FloatSin(node[1].i); node+=2; break;
        case FLOAT_SQRT:
            gi.FloatSqrt(node[1].i); node+=2; break;
// Start position
        case NEXT_START:
            gi.NextStart ( ++node ); break;
        case NEXT_START_AND_RETURN:
            if ( gi.NextStartAndReturn ( ++node ) ) return true;
            break;
        case NEXT_START_FIRST:
            gi.NextStartFirst (); ++node; break;
// Time
        case CLOCK:
            gi.Clock(node[1].i); node+=2; break;
        case SET_TIMER:
            gi.SetTimer(node[1].i); node+=2; break;
// Next block
        case NEXT_BLOCK:
            node = node[1].next; break;
// Version 2
        case INT_AST_DA_ARR:
            gi.IntAstDA(node[1].i, node[2].i, &node[3].i); node+=3+node[2].i; break;
        case FLOAT_AST_DA_ARR:
            gi.FloatAstDA(node[1].i, node[2].i, &node[3].f); node+=3+node[2].i; break;
        case INT_AST_DA_PTR:
            gi.IntAstDA(node[1].i, node[2].i, (int*)node[3].ptr); node+=4; break;
        case FLOAT_AST_DA_PTR:
            gi.FloatAstDA(node[1].i, node[2].i, (float*)node[3].ptr); node+=4; break;
        case NORMAL_3V_I:
            gi.Normal3vI(node[1].i); node+=2; break;
        case VERTEX_2V_I:
            gi.Vertex2vI(node[1].i); node+=2; break;
        case VERTEX_3V_I:
            gi.Vertex3vI(node[1].i); node+=2; break;
        case INT_AST_DI:
            gi.IntAstDI(node[1].i, node[2].i); node+=3; break;
        case BUILD_2D_MIPMAPS_BGR_BTPC_ARR:
            gi.Build2DMipmapsBGR_BTPC(node[1].i, &node[2].uc); node+=2+(node[1].i+3)/4; break;
        case BUILD_2D_MIPMAPS_BGR_BTPC_PTR:
            gi.Build2DMipmapsBGR_BTPC(node[1].i, node[2].ptr); node+=3; break;
// Version 3
        case ENABLE_COLOR_MATERIAL: gi.EnableColorMaterial(); ++node; break;
        case DISABLE_COLOR_MATERIAL: gi.DisableColorMaterial(); ++node; break;
        case DISK: 
            gi.Disk(node[1].f, node[2].f, node[3].i, node[4].i); node+=5; break;
        case TIME:
            gi.Time(node[1].i); node+=2; break;
// Version 4
        case GET_FLOAT_MODEL_VIEW_MATRIX:
            gi.GetFloatModelViewMatrix(node[1].i); node+=2; break;
        case GET_FLOAT_PROJECTION_MATRIX:
            gi.GetFloatProjectionMatrix(node[1].i); node+=2; break;
        case LIGHT_QUADRATIC_ATTENUATION:
            gi.LightQuadraticAttenuation(node[1].light, node[2].f); node+=3; break;
        case FRUSTUM_4V:
            gi.Frustum4v ( node[1].i ); node+=2; break;
        default:
            return false;
        }
    }
    return true;
}

const GraphicInterface::Node * GraphicInterface::pass ( const Node * node ) const
{
    int count = 0;
    const Node * freeNode = getFreeNode();
    while ( node != freeNode )
    {
        switch ( node->code )
        {
// Clear
        case CLEAR_COLOR: node += 5; break;
        case CLEAR_COLOR_BUFFER: ++node; break;
        case CLEAR_DEPTH_BUFFER: ++node; break;
        case CLEAR_STENCIL_BUFFER: ++node; break;
// Enable
        case ENABLE_BLEND: ++node; break;
        case ENABLE_CULL_FACE: ++node; break;
        case ENABLE_DEPTH_TEST: ++node; break;
        case ENABLE_LIGHT: node+=2; break;
        case ENABLE_LIGHTING: ++node; break;
        case ENABLE_LINE_STIPPLE: ++node; break;
        case ENABLE_NORMALIZE: ++node; break;
        case ENABLE_POLYGON_STIPPLE: ++node; break;
        case ENABLE_STENCIL_TEST: ++node; break;
        case ENABLE_TEXTURE_2D: ++node; break;
        case ENABLE_TEXTURE_GEN_S: ++node; break;
        case ENABLE_TEXTURE_GEN_T: ++node; break;
// Disable
        case DISABLE_BLEND: ++node; break;
        case DISABLE_CULL_FACE: ++node; break;
        case DISABLE_DEPTH_TEST: ++node; break;
        case DISABLE_LIGHT: node+=2; break;
        case DISABLE_LIGHTING: ++node; break;
        case DISABLE_LINE_STIPPLE: ++node; break;
        case DISABLE_NORMALIZE: ++node; break;
        case DISABLE_POLYGON_STIPPLE: ++node; break;
        case DISABLE_STENCIL_TEST: ++node; break;
        case DISABLE_TEXTURE_GEN_S: ++node; break;
        case DISABLE_TEXTURE_GEN_T: ++node; break;
// Blend func
        case BLEND_FUNC: node+=3; break;
// Depth func
        case DEPTH_FUNC_NEVER: ++node; break;
        case DEPTH_FUNC_LESS: ++node; break;
        case DEPTH_FUNC_LEQUAL: ++node; break;
        case DEPTH_FUNC_EQUAL: ++node; break;
        case DEPTH_FUNC_GREATER: ++node; break;
        case DEPTH_FUNC_NOTEQUAL: ++node; break;
        case DEPTH_FUNC_GEQUAL: ++node; break;
        case DEPTH_FUNC_ALWAYS: ++node; break;
// Stencil func
        case STENCIL_FUNC_ALWAYS: node+=3; break;
        case STENCIL_FUNC_EQUAL: node+=3; break;
        case STENCIL_OP: node+=4; break;
// Begin, End
        case BEGIN_POINTS: ++node; break;
        case BEGIN_LINES: ++node; break;
        case BEGIN_LINE_STRIP: ++node; break;
        case BEGIN_LINE_LOOP: ++node; break;
        case BEGIN_TRIANGLES: ++node; break;
        case BEGIN_TRIANGLE_STRIP: ++node; break;
        case BEGIN_TRIANGLE_FAN: ++node; break;
        case BEGIN_QUADS: ++node; break;
        case BEGIN_QUAD_STRIP: ++node; break;
        case BEGIN_POLYGON: ++node; break;
        case END: ++node; break;
// Cull face
        case CULL_FACE_BACK: ++node; break;
        case CULL_FACE_FRONT: ++node; break;
        case CULL_FACE_FRONT_AND_BACK: ++node; break;
// Light
        case LIGHT_AMBIENT: node+=6; break;
        case LIGHT_DIFFUSE: node+=6; break;
        case LIGHT_SPECULAR: node+=6; break;
        case LIGHT_POSITION: node+=6; break;
        case LIGHT_POSITION_4V: break;
// Stipple
        case LINE_STIPPLE: break;
        case POLYGON_STIPPLE_ARR: node+=1+32; break;
        case POLYGON_STIPPLE_PTR: node+=2; break;
// Material
        case MATERIAL_BACK_AMBIENT: node+=5; break;
        case MATERIAL_BACK_DIFFUSE: node+=5; break;
        case MATERIAL_BACK_EMISSION: node+=5; break;
        case MATERIAL_BACK_SPECULAR: node+=5; break;
        case MATERIAL_BACK_SHININESS: node+=2; break;
        case MATERIAL_FRONT_AMBIENT: node+=5; break;
        case MATERIAL_FRONT_DIFFUSE: node+=5; break;
        case MATERIAL_FRONT_EMISSION: node+=5; break;
        case MATERIAL_FRONT_SPECULAR: node+=5; break;
        case MATERIAL_FRONT_SHININESS: node+=2; break;
// Polygon mode
        case POLYGON_MODE: node+=3; break;
// Color
        case COLOR3: node+=4; break;
        case COLOR4: node+=5; break;
        case COLOR_3V: node+=2; break;
        case COLOR_4V: node+=2; break;
// Normal
        case NORMAL: node+=4; break;
// Vertex
        case VERTEX2: node+=3; break;
        case VERTEX3: node+=4; break;
        case VERTEX_2V: node+=2; break;
        case VERTEX_3V: node+=2; break;
// Display list
        case CALL_LIST: node+=2; break;
        case CALL_LIST_INDEX: node+=2; break;
        case END_LIST: ++node; break;
        case NEW_LIST_COMPILE: node+=2; break;
        case NEW_LIST_COMPILE_AND_EXECUTE: node+=2; break;
// Matrix
        case LOAD_IDENTITY: ++node; break;
        case LOOK_AT: node+=2; break;
        case MATRIX_MODE_MODEL_VIEW: ++node; break;
        case MATRIX_MODE_PROJECTION: ++node; break;
        case MATRIX_MODE_TEXTURE: ++node; break;
        case MULT_MATRIX: node+=2; break;
        case PERSPECTIVE: node+=5; break;
        case PERSPECTIVE_4V: node+=2; break;
        case POP_MATRIX: ++node; break;
        case PUSH_MATRIX: ++node; break;
        case ROTATE: node+=5; break;
        case ROTATE_4V: node+=2; break;
        case SCALE: node+=4; break;
        case SCALE_3V: node+=2; break;
        case TRANSLATE: node+=4; break;
        case TRANSLATE_3V: node+=2; break;
// Texture
        case BIND_TEXTURE_2D: node+=2; break;
        case BUILD_2D_MIPMAPS_BGR_ARR: node+=3+3*node[1].i*node[2].i/4; break;
        case BUILD_2D_MIPMAPS_BGR_PTR: node+=4; break;
        case GEN_TEXTURES: node+=3; break;
        case TEX_COORD2: node+=3; break;
        case TEX_PAR_2D_MIN_FILTER_MIPMAP_NEARST: ++node; break;
        case TEX_PAR_2D_WRAP_S_REPEAT: ++node; break;
        case TEX_PAR_2D_WRAP_T_REPEAT: ++node; break;
// Get
        case GET_FLOAT_VIEWPORT: node+=2; break;
        case GET_INT_VIEWPORT: node+=2; break;
// Quadric objects
        case QUADRIC_TEXTURE: node+=2; break;
        case SPHERE: node+=4; break;
// Int commands
        case INT_AST_DC: node+=3; break;
        case INT_AST_DD: node+=3; break;
        case INT_ADD_DC: node+=3; break;
        case INT_ADD_DD: node+=3; break;
        case INT_SUB_DC: node+=3; break;
        case INT_SUB_DD: node+=3; break;
        case INT_MUL_DC: node+=3; break;
        case INT_MUL_DD: node+=3; break;
        case INT_DIV_DC: node+=3; break;
        case INT_DIV_DD: node+=3; break;
        case INT_MOD_DC: node+=3; break;
        case INT_MOD_DD: node+=3; break;
// Float commands
        case FLOAT_AST_DC: node+=3; break;
        case FLOAT_AST_DD: node+=3; break;
        case FLOAT_AST_DI: node+=3; break;
        case FLOAT_AST_IC: node+=3; break;
        case FLOAT_AST_ID: node+=3; break;
        case FLOAT_AST_II: node+=3; break;

        case FLOAT_ADD_DC: node+=3; break;
        case FLOAT_ADD_DD: node+=3; break;
        case FLOAT_ADD_DI: node+=3; break;
        case FLOAT_ADD_IC: node+=3; break;
        case FLOAT_ADD_ID: node+=3; break;
        case FLOAT_ADD_II: node+=3; break;

        case FLOAT_SUB_DC: node+=3; break;
        case FLOAT_SUB_DD: node+=3; break;
        case FLOAT_SUB_DI: node+=3; break;
        case FLOAT_SUB_IC: node+=3; break;
        case FLOAT_SUB_ID: node+=3; break;
        case FLOAT_SUB_II: node+=3; break;

        case FLOAT_MUL_DC: node+=3; break;
        case FLOAT_MUL_DD: node+=3; break;
        case FLOAT_MUL_DI: node+=3; break;
        case FLOAT_MUL_IC: node+=3; break;
        case FLOAT_MUL_ID: node+=3; break;
        case FLOAT_MUL_II: node+=3; break;

        case FLOAT_DIV_DC: node+=3; break;
        case FLOAT_DIV_DD: node+=3; break;
        case FLOAT_DIV_DI: node+=3; break;
        case FLOAT_DIV_IC: node+=3; break;
        case FLOAT_DIV_ID: node+=3; break;
        case FLOAT_DIV_II: node+=3; break;
// Mix commands
        case INT_AST_FROM_FLOAT: node+=3; break;
        case FLOAT_AST_FROM_INT: node+=3; break;
// Conditional commands
        case IF_INT_DC:   node+=4; ++count; break;
        case IF_INT_DD:   node+=4; ++count; break;
        case IF_FLOAT_DC: node+=4; ++count; break;
        case IF_FLOAT_DD: node+=4; ++count; break;
        case ELSE: ++node; if ( !count ) return node; break;
        case END_IF: ++node; if ( count ) --count; else return node; break;
// Loop
        case DO: ++node; break;
        case WHILE_INT_DC: node += 4; break;
        case WHILE_INT_DD: node += 4; break;
// Mathematics
        case FLOAT_ABS: node+=2; break;
        case FLOAT_ATAN2: node+=3; break;
        case FLOAT_COS: node+=2; break;
        case FLOAT_SIN: node+=2; break;
        case FLOAT_SQRT: node+=2; break;
// Start position
        case NEXT_START: ++node; break;
        case NEXT_START_AND_RETURN: ++node; break;
        case NEXT_START_FIRST: ++node; break;
// Time
        case CLOCK: node+=2; break;
        case SET_TIMER: node+=2; break;
// Next block
        case NEXT_BLOCK:
            node = node[1].next; break;
// Version 2
        case INT_AST_DA_ARR:   node+=3+node[2].i; break;
        case FLOAT_AST_DA_ARR: node+=3+node[2].i; break;
        case INT_AST_DA_PTR:   node+=4; break;
        case FLOAT_AST_DA_PTR: node+=4; break;
        case NORMAL_3V_I: node+=2; break;
        case VERTEX_2V_I: node+=2; break;
        case VERTEX_3V_I: node+=2; break;
        case INT_AST_DI: node+=3; break;
        case BUILD_2D_MIPMAPS_BGR_BTPC_ARR: node+=2+(node[1].i+3)/4; break;
        case BUILD_2D_MIPMAPS_BGR_BTPC_PTR: node+=3; break;
// Version 3
        case ENABLE_COLOR_MATERIAL: ++node; break;
        case DISABLE_COLOR_MATERIAL: ++node; break;
        case DISK: node+=5; break;
        case TIME: node+=2; break;
// Version 4
        case GET_FLOAT_MODEL_VIEW_MATRIX: node+=2; break;
        case LIGHT_QUADRATIC_ATTENUATION: node+=3; break;
        case FRUSTUM_4V: node+=2; break;
        }
    }
    return node;
}