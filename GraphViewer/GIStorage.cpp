
#include "GraphicInterface.h"
#include "GIStorage.h"

const int blockSize = 500;

GIStorage::GIStorage () : nPtr(0), maxPtr(0), ptrArr(0)
{
    curBlock = freeNode = data = new Node[blockSize];
    curBlock[blockSize-1].next = 0;
}

const GraphicInterface::Node * GIStorage::getFirstNode() const { return data; }
const GraphicInterface::Node * GIStorage::getFreeNode () const { return freeNode; }

GIStorage::~GIStorage ()
{
    while ( data )
    {
        Node * node = data[blockSize-1].next;
        delete[] data;
        data = node;
    }
    for ( int i = nPtr; --i >= 0; ) delete[] ptrArr[i];
    delete[] ptrArr;
}

void GIStorage::clear ()
{
    Node * node = data[blockSize-1].next;
    while ( node )
    {
        Node * ptr = node[blockSize-1].next;
        delete[] node;
        node = ptr;
    }
    curBlock = freeNode = data;
    curBlock[blockSize-1].next = 0;
}

int GIStorage::countNodes () const
{
    int n = freeNode - curBlock;
    const Node * node = data[blockSize-1].next;
    while ( node )
    {
        node = node[blockSize-1].next;
        n += blockSize;
    }
    return n;
}

void GIStorage::put ( OpCode code )
{
    if ( freeNode - curBlock > blockSize - 3 )
    {
        (freeNode++)->code = NEXT_BLOCK;
        curBlock = freeNode = curBlock[blockSize-1].next = freeNode->next = new Node[blockSize];
        curBlock[blockSize-1].next = 0;
    }
    (freeNode++)->code = code;
}

GIStorage::Node * GIStorage::getNodes ( int n )
{
    if ( freeNode - curBlock > blockSize - ( n + 2 ) )
    {
        (freeNode++)->code = NEXT_BLOCK;
        curBlock = freeNode = curBlock[blockSize-1].next = freeNode->next = new Node[blockSize];
        curBlock[blockSize-1].next = 0;
    }
    Node * ret = freeNode;
    freeNode += n;
    return ret;
}

unsigned char * GIStorage::putArray ( int n, const unsigned char * arr )
{
    int i;
    unsigned char * p = new unsigned char[n];
    for ( i = n; --i >= 0; ) p[i] = arr[i];
    if ( !ptrArr )
    {
        ptrArr = new void *[maxPtr=16];
    }
    if ( nPtr >= maxPtr )
    {
        maxPtr <<= 1;
        void ** pp = new void *[maxPtr];
        for ( int i = maxPtr; --i >= nPtr; ) pp[i] = 0;
        for ( i = nPtr; --i >= 0; ) pp[i] = ptrArr[i];
        delete[] ptrArr;
        ptrArr = pp;
    }
    ptrArr[nPtr++] = p;
    return p;
}

// Clear

void GIStorage::ClearColor ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = CLEAR_COLOR;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::ClearColorBuffer  () { put ( CLEAR_COLOR_BUFFER   ); }
void GIStorage::ClearDepthBuffer  () { put ( CLEAR_DEPTH_BUFFER   ); }
void GIStorage::ClearStencilBuffer() { put ( CLEAR_STENCIL_BUFFER ); }

// Enable

void GIStorage::EnableBlend         () { put ( ENABLE_BLEND          ); }
void GIStorage::EnableColorMaterial () { put ( ENABLE_COLOR_MATERIAL ); }
void GIStorage::EnableCullFace      () { put ( ENABLE_CULL_FACE      ); }
void GIStorage::EnableDepthTest     () { put ( ENABLE_DEPTH_TEST     ); }

void GIStorage::EnableLight ( Light light )
{
    Node * node = getNodes(2);
    node[0].i = ENABLE_LIGHT;
    node[1].i = light;
}

void GIStorage::EnableLighting      () { put ( ENABLE_LIGHTING        ); }
void GIStorage::EnableLineStipple   () { put ( ENABLE_LINE_STIPPLE    ); }
void GIStorage::EnableNormalize     () { put ( ENABLE_NORMALIZE       ); }
void GIStorage::EnablePolygonStipple() { put ( ENABLE_POLYGON_STIPPLE ); }
void GIStorage::EnableStencilTest   () { put ( ENABLE_STENCIL_TEST    ); }
void GIStorage::EnableTexture2D     () { put ( ENABLE_TEXTURE_2D      ); }
void GIStorage::EnableTextureGenS   () { put ( ENABLE_TEXTURE_GEN_S   ); }
void GIStorage::EnableTextureGenT   () { put ( ENABLE_TEXTURE_GEN_T   ); }

// Disable

void GIStorage::DisableBlend        () { put ( DISABLE_BLEND          ); }
void GIStorage::DisableColorMaterial() { put ( DISABLE_COLOR_MATERIAL ); }
void GIStorage::DisableCullFace     () { put ( DISABLE_CULL_FACE      ); }
void GIStorage::DisableDepthTest    () { put ( DISABLE_DEPTH_TEST     ); }

void GIStorage::DisableLight ( Light light )
{
    Node * node = getNodes(2);
    node[0].i = DISABLE_LIGHT;
    node[1].i = light;
}

void GIStorage::DisableLighting      () { put ( DISABLE_LIGHTING        ); }
void GIStorage::DisableLineStipple   () { put ( DISABLE_LINE_STIPPLE    ); }
void GIStorage::DisableNormalize     () { put ( DISABLE_NORMALIZE       ); }
void GIStorage::DisablePolygonStipple() { put ( DISABLE_POLYGON_STIPPLE ); }
void GIStorage::DisableStencilTest   () { put ( DISABLE_STENCIL_TEST    ); }
void GIStorage::DisableTexture2D     () { put ( DISABLE_TEXTURE_2D      ); }
void GIStorage::DisableTextureGenS   () { put ( DISABLE_TEXTURE_GEN_S   ); }
void GIStorage::DisableTextureGenT   () { put ( DISABLE_TEXTURE_GEN_T   ); }

// Blend func

void GIStorage::BlendFunc ( BlendFuncPar src, BlendFuncPar dst )
{
    Node * node = getNodes(3);
    node[0].i = BLEND_FUNC;
    node[1].i = src;
    node[2].i = dst;
}

// Depth func

void GIStorage::DepthFuncNever   () { put ( DEPTH_FUNC_NEVER    ); }
void GIStorage::DepthFuncLess    () { put ( DEPTH_FUNC_LESS     ); }
void GIStorage::DepthFuncLEqual  () { put ( DEPTH_FUNC_LEQUAL   ); }
void GIStorage::DepthFuncEqual   () { put ( DEPTH_FUNC_EQUAL    ); }
void GIStorage::DepthFuncGreater () { put ( DEPTH_FUNC_GREATER  ); }
void GIStorage::DepthFuncNotEqual() { put ( DEPTH_FUNC_NOTEQUAL ); }
void GIStorage::DepthFuncGEqual  () { put ( DEPTH_FUNC_GEQUAL   ); }
void GIStorage::DepthFuncAlways  () { put ( DEPTH_FUNC_ALWAYS   ); }

// Stencil func

void GIStorage::StencilFuncNever ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_NEVER;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncLess ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_LESS;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncLEqual ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_LEQUAL;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncEqual ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_EQUAL;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncGreater ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_GREATER;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncNotEqual ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_NOTEQUAL;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncGEqual ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_GEQUAL;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilFuncAlways ( GIuint ref, GIuint mask )
{
    Node * node = getNodes(3);
    node[0].i = STENCIL_FUNC_ALWAYS;
    node[1].i = ref;
    node[2].i = mask;
}

void GIStorage::StencilOp ( StencilOpPar fail, StencilOpPar zfail, StencilOpPar zpass )
{
    Node * node = getNodes(4);
    node[0].i = STENCIL_OP;
    node[1].i = fail;
    node[2].i = zfail;
    node[3].i = zpass;
}

// Begin, End

void GIStorage::BeginPoints       () { put ( BEGIN_POINTS         ); }
void GIStorage::BeginLines        () { put ( BEGIN_LINES          ); }
void GIStorage::BeginLineStrip    () { put ( BEGIN_LINE_STRIP     ); }
void GIStorage::BeginLineLoop     () { put ( BEGIN_LINE_LOOP      ); }
void GIStorage::BeginTriangles    () { put ( BEGIN_TRIANGLES      ); }
void GIStorage::BeginTriangleStrip() { put ( BEGIN_TRIANGLE_STRIP ); }
void GIStorage::BeginTriangleFan  () { put ( BEGIN_TRIANGLE_FAN   ); }
void GIStorage::BeginQuads        () { put ( BEGIN_QUADS          ); }
void GIStorage::BeginQuadStrip    () { put ( BEGIN_QUAD_STRIP     ); }
void GIStorage::BeginPolygon      () { put ( BEGIN_POLYGON        ); }
void GIStorage::End               () { put ( END                  ); }

// Cull face

void GIStorage::CullFaceBack () { put ( CULL_FACE_BACK ); }
void GIStorage::CullFaceFront () { put ( CULL_FACE_FRONT ); }
void GIStorage::CullFaceFrontAndBack () { put ( CULL_FACE_FRONT_AND_BACK ); }

// Light

void GIStorage::LightAmbient ( Light i, float r, float g, float b, float a )
{
    Node * node = getNodes(6);
    node[0].i = LIGHT_AMBIENT;
    node[1].i = i;
    node[2].f = r;
    node[3].f = g;
    node[4].f = b;
    node[5].f = a;
}

void GIStorage::LightDiffuse ( Light i, float r, float g, float b, float a )
{
    Node * node = getNodes(6);
    node[0].i = LIGHT_DIFFUSE;
    node[1].i = i;
    node[2].f = r;
    node[3].f = g;
    node[4].f = b;
    node[5].f = a;
}

void GIStorage::LightSpecular ( Light i, float r, float g, float b, float a )
{
    Node * node = getNodes(6);
    node[0].i = LIGHT_SPECULAR;
    node[1].i = i;
    node[2].f = r;
    node[3].f = g;
    node[4].f = b;
    node[5].f = a;
}

void GIStorage::LightPosition ( Light i, float r, float g, float b, float a )
{
    Node * node = getNodes(6);
    node[0].i = LIGHT_POSITION;
    node[1].i = i;
    node[2].f = r;
    node[3].f = g;
    node[4].f = b;
    node[5].f = a;
}

void GIStorage::LightPosition4v ( Light i, GIuint index )
{
    Node * node = getNodes(3);
    node[0].i = LIGHT_POSITION_4V;
    node[1].i = i;
    node[2].i = index;
}

void GIStorage::LightQuadraticAttenuation ( Light i, float f )
{
    Node * node = getNodes(3);
    node[0].i = LIGHT_QUADRATIC_ATTENUATION;
    node[1].i = i;
    node[2].f = f;
}

// Stipple

void GIStorage::LineStipple ( int factor, int pattern )
{
    Node * node = getNodes(3);
    node[0].i = LINE_STIPPLE;
    node[1].i = factor;
    node[2].i = pattern;
}

void GIStorage::PolygonStipple ( const unsigned char mask[128] )
{
    Node * node = getNodes(2);
    node[0].i = POLYGON_STIPPLE_PTR;
    node[1].ptr = putArray ( 128, mask );
}

// Material

void GIStorage::MaterialBackAmbient ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_BACK_AMBIENT;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialBackDiffuse ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_BACK_DIFFUSE;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialBackEmission ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_BACK_EMISSION;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialBackSpecular ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_BACK_SPECULAR;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialBackShininess ( float s )
{
    Node * node = getNodes(2);
    node[0].i = MATERIAL_BACK_SHININESS;
    node[1].f = s;
}

void GIStorage::MaterialFrontAmbient ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_FRONT_AMBIENT;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialFrontDiffuse ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_FRONT_DIFFUSE;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialFrontEmission ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_FRONT_EMISSION;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialFrontSpecular ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = MATERIAL_FRONT_SPECULAR;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::MaterialFrontShininess ( float s )
{
    Node * node = getNodes(2);
    node[0].i = MATERIAL_FRONT_SHININESS;
    node[1].f = s;
}

// Polygon mode

void GIStorage::PolygonMode ( Face face, enum PolygonMode mode )
{
    Node * node = getNodes(3);
    node[0].i = POLYGON_MODE;
    node[1].i = face;
    node[2].i = mode;
}

// Color

void GIStorage::Color ( float r, float g, float b )
{
    Node * node = getNodes(4);
    node[0].i = COLOR3;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
}

void GIStorage::Color ( float r, float g, float b, float a )
{
    Node * node = getNodes(5);
    node[0].i = COLOR4;
    node[1].f = r;
    node[2].f = g;
    node[3].f = b;
    node[4].f = a;
}

void GIStorage::Color3v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = COLOR_3V;
    node[1].i = index;
}

void GIStorage::Color4v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = COLOR_4V;
    node[1].i = index;
}

// Normal

void GIStorage::Normal ( float x, float y, float z )
{
    Node * node = getNodes(4);
    node[0].i = NORMAL;
    node[1].f = x;
    node[2].f = y;
    node[3].f = z;
}

void GIStorage::Normal3vI ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = NORMAL_3V_I;
    node[1].i = index;
}

// Vertex

void GIStorage::Vertex ( float x, float y )
{
    Node * node = getNodes(3);
    node[0].i = VERTEX2;
    node[1].f = x;
    node[2].f = y;
}

void GIStorage::Vertex ( float x, float y, float z )
{
    Node * node = getNodes(4);
    node[0].i = VERTEX3;
    node[1].f = x;
    node[2].f = y;
    node[3].f = z;
}

void GIStorage::Vertex2v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = VERTEX_2V;
    node[1].i = index;
}

void GIStorage::Vertex3v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = VERTEX_3V;
    node[1].i = index;
}

void GIStorage::Vertex2vI ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = VERTEX_2V_I;
    node[1].i = index;
}

void GIStorage::Vertex3vI ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = VERTEX_3V_I;
    node[1].i = index;
}

// Display list

void GIStorage::CallList ( int list )
{
    Node * node = getNodes(2);
    node[0].i = CALL_LIST;
    node[1].i = list;
}

void GIStorage::CallListIndex ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = CALL_LIST_INDEX;
    node[1].i = index;
}

void GIStorage::EndList () { put ( END_LIST  ); }

void GIStorage::NewListCompile ( int list )
{
    Node * node = getNodes(2);
    node[0].i = NEW_LIST_COMPILE;
    node[1].i = list;
}

void GIStorage::NewListCompileAndExecute ( int list )
{
    Node * node = getNodes(2);
    node[0].i = NEW_LIST_COMPILE_AND_EXECUTE;
    node[1].i = list;
}

// Matrix

void GIStorage::Frustum4v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = FRUSTUM_4V;
    node[1].i = index;
}

void GIStorage::LoadIdentity() { put ( LOAD_IDENTITY ); }

void GIStorage::LookAt ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = LOOK_AT;
    node[1].i = index;
}

void GIStorage::MatrixModeModelView () { put ( MATRIX_MODE_MODEL_VIEW ); }
void GIStorage::MatrixModeProjection() { put ( MATRIX_MODE_PROJECTION ); }
void GIStorage::MatrixModeTexture   () { put ( MATRIX_MODE_TEXTURE    ); }

void GIStorage::MultMatrix ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = MULT_MATRIX;
    node[1].i = index;
}

void GIStorage::Perspective ( float fovy, float aspect, float zNear, float zFar )
{
    Node * node = getNodes(5);
    node[0].i = PERSPECTIVE;
    node[1].f = fovy;
    node[2].f = aspect;
    node[3].f = zNear;
    node[4].f = zFar;
}

void GIStorage::Perspective4v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = PERSPECTIVE_4V;
    node[1].i = index;
}

void GIStorage::PopMatrix () { put ( POP_MATRIX  ); }
void GIStorage::PushMatrix() { put ( PUSH_MATRIX ); }

void GIStorage::Rotate ( float angle, float x, float y, float z )
{
    Node * node = getNodes(5);
    node[0].i = ROTATE;
    node[1].f = angle;
    node[2].f = x;
    node[3].f = y;
    node[4].f = z;
}

void GIStorage::Rotate4v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = ROTATE_4V;
    node[1].i = index;
}

void GIStorage::Scale ( float x, float y, float z )
{
    Node * node = getNodes(4);
    node[0].i = SCALE;
    node[1].f = x;
    node[2].f = y;
    node[3].f = z;
}

void GIStorage::Scale3v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = SCALE_3V;
    node[1].i = index;
}

void GIStorage::Translate ( float x, float y, float z )
{
    Node * node = getNodes(4);
    node[0].i = TRANSLATE;
    node[1].f = x;
    node[2].f = y;
    node[3].f = z;
}

void GIStorage::Translate3v ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = TRANSLATE_3V;
    node[1].i = index;
}

// Texture

void GIStorage::BindTexture2D ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = BIND_TEXTURE_2D;
    node[1].i = index;
}

void GIStorage::Build2DMipmapsBGR ( int width, int height, const unsigned char * pData )
{
    Node * node = getNodes(4);
    node[0].i = BUILD_2D_MIPMAPS_BGR_PTR;
    node[1].i = width;
    node[2].i = height;
    node[3].ptr = putArray ( 3 * width * height, pData );
}

void GIStorage::Build2DMipmapsBGR_BTPC ( int lenght, const unsigned char * pData )
{
    Node * node = getNodes(3);
    node[0].i = BUILD_2D_MIPMAPS_BGR_BTPC_PTR;
    node[1].i = lenght;
    node[2].ptr = putArray ( lenght, pData );
}

void GIStorage::GenTextures ( GIuint n, GIuint index )
{
    Node * node = getNodes(3);
    node[0].i = GEN_TEXTURES;
    node[1].i = n;
    node[2].i = index;
}

void GIStorage::TexCoord2 ( float s, float t )
{
    Node * node = getNodes(3);
    node[0].i = TEX_COORD2;
    node[1].f = s;
    node[2].f = t;
}

void GIStorage::TexParameter2DMinFilterLinearMipmapNearest () { put ( TEX_PAR_2D_MIN_FILTER_MIPMAP_NEARST ); }
void GIStorage::TexParameter2DWrapSRepeat() { put ( TEX_PAR_2D_WRAP_S_REPEAT ); }
void GIStorage::TexParameter2DWrapTRepeat() { put ( TEX_PAR_2D_WRAP_T_REPEAT ); }

// Get

void GIStorage::GetFloatModelViewMatrix ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = GET_FLOAT_MODEL_VIEW_MATRIX;
    node[1].i = index;
}

void GIStorage::GetFloatProjectionMatrix ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = GET_FLOAT_PROJECTION_MATRIX;
    node[1].i = index;
}

void GIStorage::GetFloatViewport ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = GET_FLOAT_VIEWPORT;
    node[1].i = index;
}

void GIStorage::GetIntViewport ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = GET_INT_VIEWPORT;
    node[1].i = index;
}

// Quadric objects

void GIStorage::QuadricTexture ( bool b )
{
    Node * node = getNodes(2);
    node[0].i = QUADRIC_TEXTURE;
    node[1].b = b;
}

void GIStorage::Disk ( float innerRadius, float outerRadius, int slices, int loops )
{
    Node * node = getNodes(5);
    node[0].i = DISK;
    node[1].f = innerRadius;
    node[2].f = outerRadius;
    node[3].i = slices;
    node[4].i = loops;
}

void GIStorage::Sphere ( float radius, int slices, int stakes )
{
    Node * node = getNodes(4);
    node[0].i = SPHERE;
    node[1].f = radius;
    node[2].i = slices;
    node[3].i = stakes;
}

// Int values

void GIStorage::IntAstDA ( GIuint u, int n, const int * p )
{
    Node * node = getNodes(4);
    node[0].i = INT_AST_DA_PTR;
    node[1].i = u;
    node[2].i = n;
    node[3].ptr = putArray ( 4*n, (unsigned char*)p );
}

void GIStorage::IntAstDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_AST_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntAstDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_AST_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntAstDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_AST_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntAstFromFloat ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_AST_FROM_FLOAT;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntAddDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_ADD_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntAddDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_ADD_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntSubDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_SUB_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntSubDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_SUB_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntMulDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_MUL_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntMulDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_MUL_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntDivDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_DIV_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntDivDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_DIV_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::IntModDC ( GIuint index, int i )
{
    Node * node = getNodes(3);
    node[0].i = INT_MOD_DC;
    node[1].i = index;
    node[2].i = i;
}

void GIStorage::IntModDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = INT_MOD_DD;
    node[1].i = index1;
    node[2].i = index2;
}

// Float values

void GIStorage::FloatAstDA ( GIuint u, int n, const float * p )
{
    Node * node = getNodes(4);
    node[0].i = FLOAT_AST_DA_PTR;
    node[1].i = u;
    node[2].i = n;
    node[3].ptr = putArray ( 4*n, (unsigned char*)p );
}

void GIStorage::FloatAstDC ( GIuint u, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_DC;
    node[1].i = u;
    node[2].f = f;
}

void GIStorage::FloatAstDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAstDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAstIC ( GIuint u, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_IC;
    node[1].i = u;
    node[2].f = f;
}

void GIStorage::FloatAstID ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_ID;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAstII ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_II;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAstFromInt ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_AST_FROM_INT;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAddDC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_DC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatAddDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAddDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAddIC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_ID;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatAddID ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_ID;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatAddII ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ADD_II;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatSubDC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_DC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatSubDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatSubDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatSubIC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_IC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatSubID ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_ID;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatSubII ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_SUB_II;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatMulDC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_DC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatMulDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatMulDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatMulIC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_IC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatMulID ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_ID;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatMulII ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_MUL_II;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatDivDC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_DC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatDivDD ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_DD;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatDivDI ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_DI;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatDivIC ( GIuint index, float f )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_IC;
    node[1].i = index;
    node[2].f = f;
}

void GIStorage::FloatDivID ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_ID;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatDivII ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_DIV_II;
    node[1].i = index1;
    node[2].i = index2;
}

// Conditional commands

bool GIStorage::IfIntDC ( GIuint index1, Relation rel, int i )
{
    Node * node = getNodes(4);
    node[0].i = IF_INT_DC;
    node[1].i = index1;
    node[2].i = rel;
    node[3].i = i;
    return 0;
}

bool GIStorage::IfIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    Node * node = getNodes(4);
    node[0].i = IF_INT_DD;
    node[1].i = index1;
    node[2].i = rel;
    node[3].i = index2;
    return 0;
}

bool GIStorage::IfFloatDC ( GIuint index1, Relation rel, float f )
{
    Node * node = getNodes(4);
    node[0].i = IF_FLOAT_DC;
    node[1].i = index1;
    node[2].i = rel;
    node[3].f = f;
    return 1;
}

bool GIStorage::IfFloatDD ( GIuint index1, Relation rel, GIuint index2 )
{
    Node * node = getNodes(4);
    node[0].i = IF_FLOAT_DD;
    node[1].i = index1;
    node[2].i = rel;
    node[3].i = index2;
    return 1;
}

bool GIStorage::Else () { put ( ELSE ); return 1; }
void GIStorage::EndIf() { put ( END_IF ); }

// Loop

void GIStorage::Do () { put ( DO ); }

bool GIStorage::WhileIntDC ( GIuint index1, Relation rel, int i )
{
    Node * node = getNodes(4);
    node[0].i = WHILE_INT_DC;
    node[1].i = index1;
    node[2].rel = rel;
    node[3].i = i;
    return false;
}

bool GIStorage::WhileIntDD ( GIuint index1, Relation rel, GIuint index2 )
{
    Node * node = getNodes(4);
    node[0].i = WHILE_INT_DD;
    node[1].i = index1;
    node[2].rel = rel;
    node[3].i = index2;
    return false;
}

// Mathematics

void GIStorage::FloatAbs ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = FLOAT_ABS;
    node[1].i = index;
}

void GIStorage::FloatAtan2 ( GIuint index1, GIuint index2 )
{
    Node * node = getNodes(3);
    node[0].i = FLOAT_ATAN2;
    node[1].i = index1;
    node[2].i = index2;
}

void GIStorage::FloatCos ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = FLOAT_COS;
    node[1].i = index;
}

void GIStorage::FloatSin ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = FLOAT_SIN;
    node[1].i = index;
}

void GIStorage::FloatSqrt ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = FLOAT_SQRT;
    node[1].i = index;
}

// Start position

void GIStorage::NextStart ( const Node * ) { put ( NEXT_START ); }
bool GIStorage::NextStartAndReturn ( const Node * ) { put ( NEXT_START_AND_RETURN ); return 0; }
void GIStorage::NextStartFirst () { put ( NEXT_START_FIRST ); }

// Time

void GIStorage::Clock ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = CLOCK;
    node[1].i = index;
}

void GIStorage::Time ( GIuint index )
{
    Node * node = getNodes(2);
    node[0].i = TIME;
    node[1].i = index;
}

void GIStorage::SetTimer ( int elapse )
{
    Node * node = getNodes(2);
    node[0].i = SET_TIMER;
    node[1].i = elapse;
}
