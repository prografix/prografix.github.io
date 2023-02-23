
#include "ShevArray.h"

class CppFileWriter : public GraphicInterface
{
    CppFileWriter ( const CppFileWriter & );
    void operator = ( const CppFileWriter & );
// Data
    nat nextStartCount;
    GIuint intSize, floatSize, intArrCount, floatArrCount;
    struct _iobuf * file;
    Suite<bit8> buf;
    class PseudoWriteFile * tmp;
public:
    CppFileWriter ( const char * filename );
    ~CppFileWriter ();
/////////////////////////////////////////////////////////////////////
//                                                                 //
//                   Graphical commands                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// Clear
    void ClearColor ( float r, float g, float b, float a );
    void ClearColorBuffer  ();
    void ClearDepthBuffer  ();
	void ClearStencilBuffer();
// Enable
    void EnableBlend         ();
    void EnableColorMaterial ();
    void EnableCullFace      ();
    void EnableDepthTest     ();
    void EnableLight    (Light);
    void EnableLighting      ();
    void EnableLineStipple   ();
    void EnableNormalize     ();
    void EnablePolygonStipple();
	void EnableStencilTest   ();
    void EnableTexture2D     ();
    void EnableTextureGenS   ();
    void EnableTextureGenT   ();
// Disable
    void DisableBlend         ();
    void DisableColorMaterial ();
    void DisableCullFace      ();
    void DisableDepthTest     ();
    void DisableLight    (Light);
    void DisableLighting      ();
    void DisableLineStipple   ();
    void DisableNormalize     ();
    void DisablePolygonStipple();
	void DisableStencilTest   ();
    void DisableTexture2D     ();
    void DisableTextureGenS   ();
    void DisableTextureGenT   ();
// Blend func
    void BlendFunc ( BlendFuncPar src, BlendFuncPar dst );
// Depth func
    void DepthFuncNever   ();
    void DepthFuncLess    ();
    void DepthFuncLEqual  ();
    void DepthFuncEqual   ();
    void DepthFuncGreater ();
    void DepthFuncNotEqual();
    void DepthFuncGEqual  ();
    void DepthFuncAlways  ();
// Stencil func
    void StencilFuncNever   ( GIuint ref, GIuint mask );
    void StencilFuncLess    ( GIuint ref, GIuint mask );
    void StencilFuncLEqual  ( GIuint ref, GIuint mask );
    void StencilFuncEqual   ( GIuint ref, GIuint mask );
    void StencilFuncGreater ( GIuint ref, GIuint mask );
    void StencilFuncNotEqual( GIuint ref, GIuint mask );
    void StencilFuncGEqual  ( GIuint ref, GIuint mask );
    void StencilFuncAlways  ( GIuint ref, GIuint mask );
    void StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass );
// Begin, End
    void BeginPoints       ();
    void BeginLines        ();
    void BeginLineStrip    ();
    void BeginLineLoop     ();
    void BeginTriangles    ();
    void BeginTriangleStrip();
    void BeginTriangleFan  ();
    void BeginQuads        ();
    void BeginQuadStrip    ();
    void BeginPolygon      ();
    void End();
// Cull face
    void CullFaceBack ();
    void CullFaceFront ();
    void CullFaceFrontAndBack ();
// Light
    void LightAmbient   ( Light, float r, float g, float b, float a );
    void LightDiffuse   ( Light, float r, float g, float b, float a );
    void LightSpecular  ( Light, float r, float g, float b, float a );
    void LightPosition  ( Light, float x, float y, float z, float w );
    void LightPosition4v( Light, GIuint index );
    void LightQuadraticAttenuation ( Light, float );
// Stipple
    void LineStipple ( int factor, int pattern );
    void PolygonStipple ( const unsigned char mask[128] );
// Material
    void MaterialBackAmbient   ( float r, float g, float b, float a );
    void MaterialBackDiffuse   ( float r, float g, float b, float a );
    void MaterialBackEmission  ( float r, float g, float b, float a );
    void MaterialBackSpecular  ( float r, float g, float b, float a );
    void MaterialBackShininess ( float );
    void MaterialFrontAmbient  ( float r, float g, float b, float a );
    void MaterialFrontDiffuse  ( float r, float g, float b, float a );
    void MaterialFrontEmission ( float r, float g, float b, float a );
    void MaterialFrontSpecular ( float r, float g, float b, float a );
    void MaterialFrontShininess( float );
// Polygon mode
    void PolygonMode ( Face face, enum PolygonMode mode );
// Color
    void Color ( float r, float g, float b );
    void Color ( float r, float g, float b, float a );
    void Color3v ( GIuint index );
    void Color4v ( GIuint index );
// Normal
    void Normal ( float x, float y, float z );
    void Normal3vI ( GIuint index );
// Vertex
    void Vertex ( float x, float y );
    void Vertex ( float x, float y, float z );
    void Vertex2v ( GIuint index );
    void Vertex3v ( GIuint index );
    void Vertex2vI ( GIuint index );
    void Vertex3vI ( GIuint index );
// Display list
    void CallList ( int list );
    void CallListIndex ( GIuint index );
    void EndList ();
    void NewListCompile ( int list );
    void NewListCompileAndExecute ( int list );
// Matrix
    void Frustum4v ( GIuint index );
    void LoadIdentity();
    void LookAt ( GIuint index );
    void MatrixModeModelView();
    void MatrixModeProjection();
    void MatrixModeTexture();
    void MultMatrix ( GIuint index );
    void Perspective ( float fovy, float aspect, float zNear, float zFar );
    void Perspective4v ( GIuint index );
    void PopMatrix ();
    void PushMatrix();
    void Rotate ( float angle, float x, float y, float z );
    void Rotate4v ( GIuint index );
    void Scale ( float x, float y, float z );
    void Scale3v ( GIuint index );
    void Translate ( float x, float y, float z );
    void Translate3v ( GIuint index );
// Texture
    void BindTexture2D ( GIuint index );
    void Build2DMipmapsBGR ( int width, int height, const unsigned char * pData );
    void Build2DMipmapsBGR_BTPC ( int length, const unsigned char * pData );
    void GenTextures ( GIuint n, GIuint index );
    void TexCoord2 ( float s, float t );
	void TexParameter2DMinFilterLinearMipmapNearest();
	void TexParameter2DWrapSRepeat();
	void TexParameter2DWrapTRepeat();
// Get
    void GetFloatViewport ( GIuint index );
    void GetIntViewport ( GIuint index );
// Quadric objects
	void QuadricTexture ( bool );
	void Disk ( float innerRadius, float outerRadius, int slices, int loops );
	void Sphere ( float radius, int slices, int stacks );
/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 Non graphical commands                          //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// Integer values
    void IntAstDA ( GIuint index, int n, const int * );
    void IntAstDC ( GIuint index, int i );          // =
    void IntAstDD ( GIuint index1, GIuint index2 ); // =
    void IntAstDI ( GIuint index1, GIuint index2 ); // =

    void IntAddDC ( GIuint index1, int i );         // +=
    void IntAddDD ( GIuint index1, GIuint index2 ); // +=

    void IntSubDC ( GIuint index, int i );          // -=
    void IntSubDD ( GIuint index1, GIuint index2 ); // -=

    void IntMulDC ( GIuint index, int i );          // *=
    void IntMulDD ( GIuint index1, GIuint index2 ); // *=

    void IntDivDC ( GIuint index, int i );          // /=
    void IntDivDD ( GIuint index1, GIuint index2 ); // /=

    void IntModDC ( GIuint index, int i );          // %=
    void IntModDD ( GIuint index1, GIuint index2 ); // %=
// Float values
    void FloatAstDA ( GIuint index, int n, const float * );
    void FloatAstDC ( GIuint index1, float f );       // =
    void FloatAstDD ( GIuint index1, GIuint index2 ); // =
    void FloatAstDI ( GIuint index1, GIuint index2 ); // =
    void FloatAstIC ( GIuint index1, float f );       // =
    void FloatAstID ( GIuint index1, GIuint index2 ); // =
    void FloatAstII ( GIuint index1, GIuint index2 ); // =

    void FloatAddDC ( GIuint index1, float f );       // +=
    void FloatAddDD ( GIuint index1, GIuint index2 ); // +=
    void FloatAddDI ( GIuint index1, GIuint index2 ); // +=
    void FloatAddIC ( GIuint index1, float f );       // +=
    void FloatAddID ( GIuint index1, GIuint index2 ); // +=
    void FloatAddII ( GIuint index1, GIuint index2 ); // +=

    void FloatSubDC ( GIuint index1, float f );       // -=
    void FloatSubDD ( GIuint index1, GIuint index2 ); // -=
    void FloatSubDI ( GIuint index1, GIuint index2 ); // -=
    void FloatSubIC ( GIuint index1, float f );       // -=
    void FloatSubID ( GIuint index1, GIuint index2 ); // -=
    void FloatSubII ( GIuint index1, GIuint index2 ); // -=

    void FloatMulDC ( GIuint index1, float f );       // *=
    void FloatMulDD ( GIuint index1, GIuint index2 ); // *=
    void FloatMulDI ( GIuint index1, GIuint index2 ); // *=
    void FloatMulIC ( GIuint index1, float f );       // *=
    void FloatMulID ( GIuint index1, GIuint index2 ); // *=
    void FloatMulII ( GIuint index1, GIuint index2 ); // *=

    void FloatDivDC ( GIuint index1, float f );       // /=
    void FloatDivDD ( GIuint index1, GIuint index2 ); // /=
    void FloatDivDI ( GIuint index1, GIuint index2 ); // /=
    void FloatDivIC ( GIuint index1, float f );       // /=
    void FloatDivID ( GIuint index1, GIuint index2 ); // /=
    void FloatDivII ( GIuint index1, GIuint index2 ); // /=
// Mix commands
    void IntAstFromFloat ( GIuint index1, GIuint index2 ); // =
    void FloatAstFromInt ( GIuint index1, GIuint index2 ); // =
// Conditional commands
    bool IfIntDC ( GIuint index1, Relation rel, int i );
    bool IfIntDD ( GIuint index1, Relation rel, GIuint index2 );

    bool IfFloatDC ( GIuint index1, Relation rel, float f );
    bool IfFloatDD ( GIuint index1, Relation rel, GIuint index2 );

    bool Else ();
    void EndIf ();
// Loop
    void Do ();
    bool WhileIntDC ( GIuint index1, Relation rel, int i );
    bool WhileIntDD ( GIuint index1, Relation rel, GIuint index2 );
// Mathematics
    void FloatAbs   ( GIuint index );
    void FloatAtan2 ( GIuint index1, GIuint index2 );
    void FloatCos   ( GIuint index );
    void FloatSin   ( GIuint index );
    void FloatSqrt  ( GIuint index );
// Start position
    void NextStart ( const Node * node );
    bool NextStartAndReturn ( const Node * node );
    void NextStartFirst ();
// Time
    void Clock ( GIuint index );
    void Time  ( GIuint index );
    void SetTimer ( int );
};