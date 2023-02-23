
typedef unsigned int GIuint;

enum BlendFuncPar
{
    BFP_ZERO = 0,
    BFP_ONE = 1,
    BFP_DST_COLOR = 2,
    BFP_ONE_MINUS_DST_COLOR = 3,
    BFP_SRC_ALPHA = 4,
    BFP_ONE_MINUS_SRC_ALPHA = 5,
    BFP_DST_ALPHA = 6,
    BFP_ONE_MINUS_DST_ALPHA = 7,
    BFP_SRC_ALPHA_SATURATE = 8
};

enum Face
{
    FACE_FRONT = 1,
    FACE_BACK = 2,
    FACE_FRONT_AND_BACK = 3
};

enum Light
{
    LIGHT_0 = 0,
    LIGHT_1 = 1,
    LIGHT_2 = 2,
    LIGHT_3 = 3,
    LIGHT_4 = 4,
    LIGHT_5 = 5,
    LIGHT_6 = 6,
    LIGHT_7 = 7,
};

enum PolygonMode
{
    PM_POINT = 0,
    PM_LINE = 1,
    PM_FILL = 2
};

enum StencilOpPar
{
    SOP_KEEP = 0,
    SOP_ZERO = 1,
    SOP_REPLACE = 2,
    SOP_INCR = 3,
    SOP_DECR = 4,
    SOP_INVERT = 5
};

enum Relation
{
    EQ = 0, // ==
    NE = 1, // !=
    LT = 2, // <
    LE = 3, // <=
    GT = 4, // >
    GE = 5  // >=
};

class GraphicInterface
{
protected:
// Enum
    enum OpCode
    {
// Clear
        CLEAR_COLOR,
        CLEAR_COLOR_BUFFER,
        CLEAR_DEPTH_BUFFER,
        CLEAR_STENCIL_BUFFER,
// Enable
        ENABLE_BLEND, 
        ENABLE_CULL_FACE,
        ENABLE_DEPTH_TEST,
        ENABLE_LIGHT,
        ENABLE_LIGHTING, 
        ENABLE_LINE_STIPPLE,
        ENABLE_NORMALIZE,
        ENABLE_POLYGON_STIPPLE,
        ENABLE_STENCIL_TEST,
        ENABLE_TEXTURE_2D,
        ENABLE_TEXTURE_GEN_S,
        ENABLE_TEXTURE_GEN_T,
// Disable
        DISABLE_BLEND,
        DISABLE_CULL_FACE,
        DISABLE_DEPTH_TEST,
        DISABLE_LIGHT,
        DISABLE_LIGHTING,
        DISABLE_LINE_STIPPLE,
        DISABLE_NORMALIZE,
        DISABLE_POLYGON_STIPPLE,
        DISABLE_STENCIL_TEST,
        DISABLE_TEXTURE_2D,
        DISABLE_TEXTURE_GEN_S,
        DISABLE_TEXTURE_GEN_T,
// Blend func
        BLEND_FUNC,
// Depth func
        DEPTH_FUNC_NEVER,   DEPTH_FUNC_LESS,     DEPTH_FUNC_LEQUAL, DEPTH_FUNC_EQUAL,
        DEPTH_FUNC_GREATER, DEPTH_FUNC_NOTEQUAL, DEPTH_FUNC_GEQUAL, DEPTH_FUNC_ALWAYS,
// Stencil func
        STENCIL_FUNC_NEVER,   STENCIL_FUNC_LESS,     STENCIL_FUNC_LEQUAL, STENCIL_FUNC_EQUAL,
        STENCIL_FUNC_GREATER, STENCIL_FUNC_NOTEQUAL, STENCIL_FUNC_GEQUAL, STENCIL_FUNC_ALWAYS,
        STENCIL_OP,
// Begin, End
        BEGIN_POINTS,
        BEGIN_LINES, BEGIN_LINE_STRIP, BEGIN_LINE_LOOP,
        BEGIN_TRIANGLES, BEGIN_TRIANGLE_STRIP, BEGIN_TRIANGLE_FAN,
        BEGIN_QUADS, BEGIN_QUAD_STRIP,
        BEGIN_POLYGON, END,
// Cull face
        CULL_FACE_BACK,
        CULL_FACE_FRONT,
        CULL_FACE_FRONT_AND_BACK,
// Light
        LIGHT_AMBIENT,
        LIGHT_AMBIENT_4V,
        LIGHT_DIFFUSE,
        LIGHT_DIFFUSE_4V,
        LIGHT_SPECULAR,
        LIGHT_SPECULAR_4V,
        LIGHT_POSITION,
        LIGHT_POSITION_4V,
// Stipple
        LINE_STIPPLE,
        POLYGON_STIPPLE_ARR,
        POLYGON_STIPPLE_PTR,
// Material
        MATERIAL_BACK_AMBIENT,
        MATERIAL_BACK_DIFFUSE,
        MATERIAL_BACK_EMISSION,
        MATERIAL_BACK_SPECULAR,
        MATERIAL_BACK_SHININESS,
        MATERIAL_FRONT_AMBIENT,
        MATERIAL_FRONT_DIFFUSE,
        MATERIAL_FRONT_EMISSION,
        MATERIAL_FRONT_SPECULAR,
        MATERIAL_FRONT_SHININESS,
// Polygon mode
        POLYGON_MODE,
// Color
        COLOR3, COLOR4, COLOR_3V, COLOR_4V,
// Normal,
        NORMAL, NORMAL_3V,
// Vertex
        VERTEX2, VERTEX3, VERTEX_2V, VERTEX_3V,
// Display list
        CALL_LIST,
        CALL_LIST_INDEX,
        END_LIST,
        NEW_LIST_COMPILE,
        NEW_LIST_COMPILE_AND_EXECUTE,
// Matrix
        LOAD_IDENTITY,
        LOOK_AT,
        MATRIX_MODE_MODEL_VIEW,
        MATRIX_MODE_PROJECTION,
        MATRIX_MODE_TEXTURE,
        MULT_MATRIX,
        PERSPECTIVE, PERSPECTIVE_4V,
        POP_MATRIX,
        PUSH_MATRIX,
        ROTATE, ROTATE_4V,
        SCALE, SCALE_3V,
        TRANSLATE, TRANSLATE_3V,
// Texture
        BIND_TEXTURE_2D,
        BUILD_2D_MIPMAPS_BGR_ARR,
        BUILD_2D_MIPMAPS_BGR_PTR,
        GEN_TEXTURES,
        TEX_COORD2,
        TEX_PAR_2D_MIN_FILTER_MIPMAP_NEARST,
        TEX_PAR_2D_WRAP_S_REPEAT,
        TEX_PAR_2D_WRAP_T_REPEAT,
// Get
        GET_FLOAT_VIEWPORT,
        GET_INT_VIEWPORT,
// Quadric objects
        QUADRIC_TEXTURE, SPHERE,
/////////////////////////////////////////////////////////////////////////////////////////
// Int commands
        INT_AST_DC, INT_AST_DD,
        INT_ADD_DC, INT_ADD_DD,
        INT_SUB_DC, INT_SUB_DD,
        INT_MUL_DC, INT_MUL_DD,
        INT_DIV_DC, INT_DIV_DD,
        INT_MOD_DC, INT_MOD_DD,
// Float commands
        FLOAT_AST_DC, FLOAT_AST_DD, FLOAT_AST_DI, FLOAT_AST_IC, FLOAT_AST_ID, FLOAT_AST_II,
        FLOAT_ADD_DC, FLOAT_ADD_DD, FLOAT_ADD_DI, FLOAT_ADD_IC, FLOAT_ADD_ID, FLOAT_ADD_II,
        FLOAT_SUB_DC, FLOAT_SUB_DD, FLOAT_SUB_DI, FLOAT_SUB_IC, FLOAT_SUB_ID, FLOAT_SUB_II,
        FLOAT_MUL_DC, FLOAT_MUL_DD, FLOAT_MUL_DI, FLOAT_MUL_IC, FLOAT_MUL_ID, FLOAT_MUL_II,
        FLOAT_DIV_DC, FLOAT_DIV_DD, FLOAT_DIV_DI, FLOAT_DIV_IC, FLOAT_DIV_ID, FLOAT_DIV_II,
// Mix commands
        INT_AST_FROM_FLOAT,
        FLOAT_AST_FROM_INT,
// Conditional commands
        IF_INT_DC, IF_INT_DD, 
        IF_FLOAT_DC, IF_FLOAT_DD, 
        ELSE, END_IF,
// Loop
        DO,
        WHILE_INT_DC, WHILE_INT_DD,
// Mathematics
        FLOAT_ABS,
        FLOAT_ATAN2,
        FLOAT_COS,
        FLOAT_SIN,
        FLOAT_SQRT,
// Start position
        NEXT_START,
        NEXT_START_AND_RETURN,
        NEXT_START_FIRST,
// Time
        CLOCK,
        SET_TIMER,
// Next block
        NEXT_BLOCK,
// Version 2
        INT_AST_DA_ARR, FLOAT_AST_DA_ARR,
        INT_AST_DA_PTR, FLOAT_AST_DA_PTR,
        NORMAL_3V_I,
        VERTEX_2V_I, VERTEX_3V_I,
        INT_AST_DI,
        BUILD_2D_MIPMAPS_BGR_BTPC_ARR,
        BUILD_2D_MIPMAPS_BGR_BTPC_PTR,
// Version 3
        ENABLE_COLOR_MATERIAL,
        DISABLE_COLOR_MATERIAL,
        DISK,
        TIME,
// Version 4
        GET_FLOAT_MODEL_VIEW_MATRIX,
        GET_FLOAT_PROJECTION_MATRIX,
        LIGHT_QUADRATIC_ATTENUATION,
        FRUSTUM_4V,
    };
public:
    union Node
    {
        BlendFuncPar    bfp;
	    OpCode          code;
        Face            face;
        Light           light;
        PolygonMode     mode;
        StencilOpPar    sop;
        Relation        rel;
        Node *          next;
        float           f;
        int             i;
        bool            b;
        unsigned char   uc;
        const unsigned char * ptr;
    };
    bool operator >> ( GraphicInterface & gi ) const;
protected:
    const Node * pass ( const Node * node ) const;
    virtual const Node * getFirstNode() const;
    virtual const Node * getFreeNode () const;
    virtual const Node * getNextStart() const;
public:
/////////////////////////////////////////////////////////////////////
//                                                                 //
//                   Graphical commands                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// Clear
    virtual void ClearColor ( float r, float g, float b, float a );
    virtual void ClearColorBuffer  ();
    virtual void ClearDepthBuffer  ();
	virtual void ClearStencilBuffer();
// Enable
    virtual void EnableBlend         ();
    virtual void EnableColorMaterial ();
    virtual void EnableCullFace      ();
    virtual void EnableDepthTest     ();
    virtual void EnableLight    (Light);
    virtual void EnableLighting      ();
    virtual void EnableLineStipple   ();
    virtual void EnableNormalize     ();
    virtual void EnablePolygonStipple();
	virtual void EnableStencilTest   ();
    virtual void EnableTexture2D     ();
    virtual void EnableTextureGenS   ();
    virtual void EnableTextureGenT   ();
// Disable
    virtual void DisableBlend         ();
    virtual void DisableColorMaterial ();
    virtual void DisableCullFace      ();
    virtual void DisableDepthTest     ();
    virtual void DisableLight    (Light);
    virtual void DisableLighting      ();
    virtual void DisableLineStipple   ();
    virtual void DisableNormalize     ();
    virtual void DisablePolygonStipple();
	virtual void DisableStencilTest   ();
    virtual void DisableTexture2D     ();
    virtual void DisableTextureGenS   ();
    virtual void DisableTextureGenT   ();
// Blend func
    virtual void BlendFunc ( BlendFuncPar src, BlendFuncPar dst );
// Depth func
    virtual void DepthFuncNever   ();
    virtual void DepthFuncLess    ();
    virtual void DepthFuncLEqual  ();
    virtual void DepthFuncEqual   ();
    virtual void DepthFuncGreater ();
    virtual void DepthFuncNotEqual();
    virtual void DepthFuncGEqual  ();
    virtual void DepthFuncAlways  ();
// Stencil func
    virtual void StencilFuncNever   ( GIuint ref, GIuint mask );
    virtual void StencilFuncLess    ( GIuint ref, GIuint mask );
    virtual void StencilFuncLEqual  ( GIuint ref, GIuint mask );
    virtual void StencilFuncEqual   ( GIuint ref, GIuint mask );
    virtual void StencilFuncGreater ( GIuint ref, GIuint mask );
    virtual void StencilFuncNotEqual( GIuint ref, GIuint mask );
    virtual void StencilFuncGEqual  ( GIuint ref, GIuint mask );
    virtual void StencilFuncAlways  ( GIuint ref, GIuint mask );
    virtual void StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass );
// Begin, End
    virtual void BeginPoints       ();
    virtual void BeginLines        ();
    virtual void BeginLineStrip    ();
    virtual void BeginLineLoop     ();
    virtual void BeginTriangles    ();
    virtual void BeginTriangleStrip();
    virtual void BeginTriangleFan  ();
    virtual void BeginQuads        ();
    virtual void BeginQuadStrip    ();
    virtual void BeginPolygon      ();
    virtual void End               ();
// Cull face
    virtual void CullFaceBack ();
    virtual void CullFaceFront ();
    virtual void CullFaceFrontAndBack ();
// Light
    virtual void LightAmbient   ( Light, float r, float g, float b, float a );
    virtual void LightDiffuse   ( Light, float r, float g, float b, float a );
    virtual void LightSpecular  ( Light, float r, float g, float b, float a );
    virtual void LightPosition  ( Light, float x, float y, float z, float w );
    virtual void LightPosition4v( Light, GIuint index );
    virtual void LightQuadraticAttenuation( Light, float );
// Stipple
    virtual void LineStipple ( int factor, int pattern );
    virtual void PolygonStipple ( const unsigned char mask[128] );
// Material
    virtual void MaterialBackAmbient  ( float r, float g, float b, float a );
    virtual void MaterialBackDiffuse  ( float r, float g, float b, float a );
    virtual void MaterialBackEmission ( float r, float g, float b, float a );
    virtual void MaterialBackSpecular ( float r, float g, float b, float a );
    virtual void MaterialBackShininess( float );
    virtual void MaterialFrontAmbient  ( float r, float g, float b, float a );
    virtual void MaterialFrontDiffuse  ( float r, float g, float b, float a );
    virtual void MaterialFrontEmission ( float r, float g, float b, float a );
    virtual void MaterialFrontSpecular ( float r, float g, float b, float a );
    virtual void MaterialFrontShininess( float );
// Polygon mode
    virtual void PolygonMode ( Face face, enum PolygonMode mode );
// Color
    virtual void Color ( float r, float g, float b );
    virtual void Color ( float r, float g, float b, float a );
    virtual void Color3v ( GIuint index );
    virtual void Color4v ( GIuint index );
// Normal
    virtual void Normal ( float x, float y, float z );
    virtual void Normal3vI ( GIuint index );
// Vertex
    virtual void Vertex ( float x, float y );
    virtual void Vertex ( float x, float y, float z );
    virtual void Vertex2v ( GIuint index );
    virtual void Vertex3v ( GIuint index );
    virtual void Vertex2vI ( GIuint index );
    virtual void Vertex3vI ( GIuint index );
// Display list
    virtual void CallList ( int list );
    virtual void CallListIndex ( GIuint index );
    virtual void EndList ();
    virtual void NewListCompile ( int list );
    virtual void NewListCompileAndExecute ( int list );
// Matrix
    virtual void Frustum4v ( GIuint index );
    virtual void LoadIdentity();
    virtual void LookAt ( GIuint index );
    virtual void MatrixModeModelView();
    virtual void MatrixModeProjection();
    virtual void MatrixModeTexture();
    virtual void MultMatrix ( GIuint index );
    virtual void Perspective ( float fovy, float aspect, float zNear, float zFar );
    virtual void Perspective4v ( GIuint index );
    virtual void PopMatrix ();
    virtual void PushMatrix();
    virtual void Rotate ( float angle, float x, float y, float z );
    virtual void Rotate4v ( GIuint index );
    virtual void Scale ( float x, float y, float z );
    virtual void Scale3v ( GIuint index );
    virtual void Translate ( float x, float y, float z );
    virtual void Translate3v ( GIuint index );
// Texture
    virtual void BindTexture2D ( GIuint index );
    virtual void Build2DMipmapsBGR ( int width, int height, const unsigned char * pData );
    virtual void Build2DMipmapsBGR_BTPC ( int length, const unsigned char * pData );
    virtual void GenTextures ( GIuint n, GIuint index );
    virtual void TexCoord2 ( float s, float t );
	virtual void TexParameter2DMinFilterLinearMipmapNearest();
	virtual void TexParameter2DWrapSRepeat();
	virtual void TexParameter2DWrapTRepeat();
// Get
    virtual void GetFloatModelViewMatrix ( GIuint index );
    virtual void GetFloatProjectionMatrix ( GIuint index );
    virtual void GetFloatViewport ( GIuint index );
    virtual void GetIntViewport ( GIuint index );
// Quadric objects
	virtual void QuadricTexture ( bool );
	virtual void Disk ( float innerRadius, float outerRadius, int slices, int loops );
	virtual void Sphere ( float radius, int slices, int stakes );
/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 Non graphical commands                          //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// Integer commands
    virtual void IntAstDA ( GIuint index, int n, const int * );
    virtual void IntAstDC ( GIuint index, int i );          // =
    virtual void IntAstDD ( GIuint index1, GIuint index2 ); // =
    virtual void IntAstDI ( GIuint index1, GIuint index2 ); // =

    virtual void IntAddDC ( GIuint index, int i );          // +=
    virtual void IntAddDD ( GIuint index1, GIuint index2 ); // +=

    virtual void IntSubDC ( GIuint index, int i );          // -=
    virtual void IntSubDD ( GIuint index1, GIuint index2 ); // -=

    virtual void IntMulDC ( GIuint index, int i );          // *=
    virtual void IntMulDD ( GIuint index1, GIuint index2 ); // *=

    virtual void IntDivDC ( GIuint index, int i );          // /=
    virtual void IntDivDD ( GIuint index1, GIuint index2 ); // /=

    virtual void IntModDC ( GIuint index, int i );          // %=
    virtual void IntModDD ( GIuint index1, GIuint index2 ); // %=
// Float commands
    virtual void FloatAstDA ( GIuint index, int n, const float * );
    virtual void FloatAstDC ( GIuint index1, float f );       // =
    virtual void FloatAstDD ( GIuint index1, GIuint index2 ); // =
    virtual void FloatAstDI ( GIuint index1, GIuint index2 ); // =
    virtual void FloatAstIC ( GIuint index1, float f );       // =
    virtual void FloatAstID ( GIuint index1, GIuint index2 ); // =
    virtual void FloatAstII ( GIuint index1, GIuint index2 ); // =

    virtual void FloatAddDC ( GIuint index1, float f );       // +=
    virtual void FloatAddDD ( GIuint index1, GIuint index2 ); // +=
    virtual void FloatAddDI ( GIuint index1, GIuint index2 ); // +=
    virtual void FloatAddIC ( GIuint index1, float f );       // +=
    virtual void FloatAddID ( GIuint index1, GIuint index2 ); // +=
    virtual void FloatAddII ( GIuint index1, GIuint index2 ); // +=

    virtual void FloatSubDC ( GIuint index1, float f );       // -=
    virtual void FloatSubDD ( GIuint index1, GIuint index2 ); // -=
    virtual void FloatSubDI ( GIuint index1, GIuint index2 ); // -=
    virtual void FloatSubIC ( GIuint index1, float f );       // -=
    virtual void FloatSubID ( GIuint index1, GIuint index2 ); // -=
    virtual void FloatSubII ( GIuint index1, GIuint index2 ); // -=

    virtual void FloatMulDC ( GIuint index1, float f );       // *=
    virtual void FloatMulDD ( GIuint index1, GIuint index2 ); // *=
    virtual void FloatMulDI ( GIuint index1, GIuint index2 ); // *=
    virtual void FloatMulIC ( GIuint index1, float f );       // *=
    virtual void FloatMulID ( GIuint index1, GIuint index2 ); // *=
    virtual void FloatMulII ( GIuint index1, GIuint index2 ); // *=

    virtual void FloatDivDC ( GIuint index1, float f );       // /=
    virtual void FloatDivDD ( GIuint index1, GIuint index2 ); // /=
    virtual void FloatDivDI ( GIuint index1, GIuint index2 ); // /=
    virtual void FloatDivIC ( GIuint index1, float f );       // /=
    virtual void FloatDivID ( GIuint index1, GIuint index2 ); // /=
    virtual void FloatDivII ( GIuint index1, GIuint index2 ); // /=
// Mix commands
    virtual void IntAstFromFloat ( GIuint index1, GIuint index2 ); // =
    virtual void FloatAstFromInt ( GIuint index1, GIuint index2 ); // =
// Conditional commands
    virtual bool IfIntDC ( GIuint index1, Relation rel, int i );
    virtual bool IfIntDD ( GIuint index1, Relation rel, GIuint index2 );

    virtual bool IfFloatDC ( GIuint index1, Relation rel, float f );
    virtual bool IfFloatDD ( GIuint index1, Relation rel, GIuint index2 );

    virtual bool Else ();
    virtual void EndIf ();
// Loop
    virtual void Do ();
    virtual bool WhileIntDC ( GIuint index1, Relation rel, int i );
    virtual bool WhileIntDD ( GIuint index1, Relation rel, GIuint index2 );
// Mathematics
    virtual void FloatAbs   ( GIuint index );
    virtual void FloatAtan2 ( GIuint index1, GIuint index2 );
    virtual void FloatCos   ( GIuint index );
    virtual void FloatSin   ( GIuint index );
    virtual void FloatSqrt  ( GIuint index );
// Start position
    virtual void NextStart ( const Node * node = 0 );
    virtual bool NextStartAndReturn ( const Node * node = 0 );
    virtual void NextStartFirst ();
// Time
    virtual void Clock ( GIuint index );
    virtual void Time  ( GIuint index );
    virtual void SetTimer ( int );
// Destructor
    virtual ~GraphicInterface ();
};
