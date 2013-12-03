/*******************************************
*                                          *
*   Paul Frazee's Vertex Array Example     *
*           nehe.gamedev.net               *
*                2003                      *
*                                          *
*******************************************/

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLaux Library
#include <stdio.h>												// Header File For Standard Input/Output
#include "NeHeGL.h"												// Header File For NeHeGL
#include <fstream>
#include <iostream>
#include "ByteStream.h"
using namespace std;
#pragma warning(default: 4995)
geometry global_geom;
model global_model;
geometry extract(char *arg)
{
    geometry ret;
    if(arg == NULL && 1 == 2)
    {
        cout << "Must supply a file to open" << endl;
        return ret;
    }
    try
    {
        ByteStream in;
        in.open("p_secret_weapon_02.xdr");
        if(!in.is_open())
        {
            cout << "Failed to open file" << arg << endl;
            return ret;
        }
        RSC7_Container con;
		in.seekg(0xA4);
		con.modelTextures= new classTextures(in);
        in.seekg(0x50);
        con.modelCollection = in.getu32();
        modelCollection collection;
        in.seekg(0x10 + (con.modelCollection&0xFFFFFFF));
        collection.modelCollectionPointer = in.getu32();
        collection.count = in.getu16();
        collection.count = in.getu16();
        in.seekg(0x10+(collection.modelCollectionPointer&0xFFFFFFF));
        uint32 *modelptr = new uint32[collection.count];
        for(int i=0; i<collection.count; i++)
        {
            modelptr[i] = in.getu32();
        }
        model *models = new model[collection.count];

        for(int i=0; i<collection.count; i++)
        {
            in.seekg(0x10+(modelptr[i]&0xFFFFFFF));
            models[i].VTable = in.getu32();
            models[i].GeometryCollectionOffset = in.getu32();
            models[i].size1 = in.getu16();
            models[i].size2 = in.getu16();
            models[i].VectorCollectionOffset = in.getu32();
            models[i].MaterialCollectionOffset = in.getu32();
            in.seekg(0x10 + (models[i].GeometryCollectionOffset&0xFFFFFFF));
            models[i].GeometryOffsetArray[0] = in.getu32();
            models[i].GeometryOffsetArray[1] = in.getu32();
            models[i].GeometryOffsetArray[2] = in.getu32();
            models[i].GeometryOffsetArray[3] = in.getu32();

            for(int j=0; j<4; j++)
            {
                in.seekg((models[i].GeometryOffsetArray[j]&0xFFFFFFF) + 0x10);
                models[i].geom[j].VTable = in.getu32();
                models[i].geom[j].null1 = in.getu32();
                models[i].geom[j].null2 = in.getu32();
                models[i].geom[j].VBOffset = in.getu32();
                models[i].geom[j].null3 = in.getu32();
                models[i].geom[j].null4 = in.getu32();
                models[i].geom[j].null5 = in.getu32();
                models[i].geom[j].IBOffset = in.getu32();
                models[i].geom[j].null6 = in.getu32();
                models[i].geom[j].null7 = in.getu32();
                models[i].geom[j].null8 = in.getu32();
                models[i].geom[j].indexCount = in.getu32();
                models[i].geom[j].faceCount = in.getu32();
                models[i].geom[j].vertCount = in.getu16();
                models[i].geom[j].unkn_primitiveType = in.getu16();






                in.seekg((models[i].geom[j].VBOffset&0xFFFFFFF) + 0x10);

                VB &vb = models[i].geom[j].vbo;
                vb.VTable = in.getu32();
                vb.stride = in.getu16();
                vb.unknown1 = in.getu16();
                vb.dataOffset = in.getu32();
                vb.vertCount = in.getu32();
                vb.dataOffset2 = in.getu32();
                vb.null1 = in.getu32();
                vb.cpuOffset1 = in.getu32();
                vb.cpuOffset2 = in.getu32();

                                unsigned int offset = (models[i].geom[j].IBOffset&0xFFFFFFF) + 0x10;
                in.seekg(offset);
                IB &ib = models[i].geom[j].ibo;
                ib.VTable = in.getu32();
                ib.stride = in.getu32();
                ib.indexOffset = in.getu32();
                ib.indicies = new unsigned short[ib.stride*2];
                offset = (ib.indexOffset & 0xFFFFFFF) + 0x2010;
                in.seekg(offset);
                try
                {
                    for(int i=0; i<ib.stride; i++)
                    {
                        ib.indicies[i] = in.getu16();
                    }
                //in.read((char*)ib.indicies, ib.stride/2);
                }
                catch(exception e)
                {
                    cout << e.what() << endl;
                }

                in.seekg((vb.dataOffset&0xFFFFFFF) + 0x2010);
                if(vb.stride == 24)
                {
                    vb.vbo = (vert28*)new vert24[vb.vertCount];
                    vert24 * v = (vert24*)vb.vbo;
                    for(int i=0; i<vb.vertCount; i++)
                    {
                        v[i].x = in.getfloat();
                        v[i].y = in.getfloat();
                        v[i].z = in.getfloat();
                        v[i].color = in.getu32();
                        v[i].tu = in.getfloat();
                        v[i].tv = in.getfloat();
                    }
                }
                else if(vb.stride == 28)
                {
                    vb.vbo = new vert28[vb.vertCount];
                    vert28 * v = (vert28*)vb.vbo;
                    vb.verts_only = new float[vb.vertCount*3];
                    int index2 = 0;
                    for(int i=0; i<vb.vertCount; i++)
                    {

                        v[i].x = in.getfloat();
                        vb.verts_only[index2++] = v[i].x;
                        v[i].y = in.getfloat();
                        vb.verts_only[index2++] = v[i].y;
                        v[i].z = in.getfloat();
                        vb.verts_only[index2++] = v[i].z;
                        v[i].w = in.getfloat();
                        v[i].color = in.getu32();
                        v[i].tu = in.getfloat();
                        v[i].tv = in.getfloat();
                    }
                }
                else
                {
                    cout << "Unknown vertex format" << endl;
                    //exit(0);
                }



                //return models[0].geom[0];
                cout << "Got Verts" << endl;




            }
            global_model = models[0];
            return models[0].geom[0];

        }



    }
    catch(exception e)
    {
        cout << e.what() << endl;
    }
    return ret;
}
#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

// TUTORIAL
// Mesh Generation Paramaters
#define MESH_RESOLUTION 4.0f									// Pixels Per Vertex
#define MESH_HEIGHTSCALE 1.0f									// Mesh Height Scale
//#define NO_VBOS												// If Defined, VBOs Will Be Forced Off

// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;					// VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;					// VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;					// VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;			// VBO Deletion Procedure

class CVert														// Vertex Class
{
public:
    float x;													// X Component
    float y;													// Y Component
    float z;													// Z Component
};
typedef CVert CVec;												// The Definitions Are Synonymous

class CTexCoord													// Texture Coordinate Class
{
public:
    float u;													// U Component
    float v;													// V Component
};

class CMesh
{
public:
    // Mesh Data
    int				m_nVertexCount;								// Vertex Count
    CVert*			m_pVertices;								// Vertex Data
    CTexCoord*		m_pTexCoords;								// Texture Coordinates
    unsigned int	m_nTextureId;								// Texture ID


    // Vertex Buffer Object Names
    unsigned int	m_nVBOVertices;								// Vertex VBO Name
    unsigned int	m_nVBOTexCoords;							// Texture Coordinate VBO Name
    unsigned int	m_nIndicies;
    // Temporary Data


public:
    CMesh();													// Mesh Constructor
    ~CMesh();													// Mesh Deconstructor

    // Heightmap Loader
    bool LoadHeightmap( char* szPath, float flHeightScale, float flResolution );
    // Single Point Height
    float PtHeight( int nX, int nY );
    // VBO Build Function
    void BuildVBOs(geometry geom);
};

bool		g_fVBOSupported = false;							// ARB_vertex_buffer_object supported?
CMesh*		g_pMesh = NULL;										// Mesh Data
float		g_flYRot = 0.0f;									// Rotation
int			g_nFPS = 0, g_nFrames = 0;							// FPS and FPS Counter
DWORD		g_dwLastFPS = 0;									// Last FPS Check Time
//~TUTORIAL

GL_Window*	g_window;
Keys*		g_keys;

// TUTORIAL
// Based Off Of Code Supplied At OpenGL.org
bool IsExtensionSupported( char* szTargetExtension )
{
    const unsigned char *pszExtensions = NULL;
    const unsigned char *pszStart;
    unsigned char *pszWhere, *pszTerminator;

    // Extension names should not have spaces
    pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
    if( pszWhere || *szTargetExtension == '\0' )
        return false;

    // Get Extensions String
    pszExtensions = glGetString( GL_EXTENSIONS );

    // Search The Extensions String For An Exact Copy
    pszStart = pszExtensions;
    for(;;)
    {
        pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
        if( !pszWhere )
            break;
        pszTerminator = pszWhere + strlen( szTargetExtension );
        if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
            if( *pszTerminator == ' ' || *pszTerminator == '\0' )
                return true;
        pszStart = pszTerminator;
    }
    return false;
}
//~TUTORIAL

BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
    g_window	= window;
    g_keys		= keys;

    // TUTORIAL
    // Load The Mesh Data
    g_pMesh = new CMesh();										// Instantiate Our Mesh
    if( !g_pMesh->LoadHeightmap( "terrain.bmp",					// Load Our Heightmap
                                MESH_HEIGHTSCALE,
                                MESH_RESOLUTION ) )
    {
        MessageBox( NULL, "Error Loading Heightmap", "Error", MB_OK );
        return false;
    }

    global_geom = extract("test.xdr");
    // Check For VBOs Supported
#ifndef NO_VBOS
    g_fVBOSupported = IsExtensionSupported( "GL_ARB_vertex_buffer_object" );
    if( g_fVBOSupported )
    {
        // Get Pointers To The GL Functions
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
        // Load Vertex Data Into The Graphics Card Memory
        g_pMesh->BuildVBOs(global_geom);									// Build The VBOs
    }
#else /* NO_VBOS */
    g_fVBOSupported = false;
#endif
    //~TUTORIAL

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 2000.0);
      //why isn't this a call a call to gluLookAt()?
      glMatrixMode(GL_MODELVIEW);
    // Setup GL States
    glClearColor (1.0f, 0.0f, 0.0f, 0.5f);						// Black Background
    glClearDepth (1.0f);										// Depth Buffer Setup
    glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
    glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
    glShadeModel (GL_SMOOTH);									// Select Smooth Shading
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
    glEnable( GL_TEXTURE_2D );									// Enable Textures
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );						// Set The Color To White
    glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);

    return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
    if( g_pMesh )												// Deallocate Our Mesh Data
        delete g_pMesh;											// And Delete VBOs
    g_pMesh = NULL;
}

void Update (DWORD milliseconds)								// Perform Motion Updates Here
{
    g_flYRot += (float) ( milliseconds ) / 1000.0f * 25.0f;		// Consistantly Rotate The Scenery

    if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
    {
        TerminateApplication (g_window);						// Terminate The Program
    }

    if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
    {
        ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
    }
}

void Draw (void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
    glLoadIdentity ();											// Reset The Modelview Matrix

    // Get FPS
    if( GetTickCount() - g_dwLastFPS >= 1000 )					// When A Second Has Passed...
    {
        g_dwLastFPS = GetTickCount();							// Update Our Time Variable
        g_nFPS = g_nFrames;										// Save The FPS
        g_nFrames = 0;											// Reset The FPS Counter

        char szTitle[256]={0};									// Build The Title String
        sprintf( szTitle, "Lesson 45: NeHe & Paul Frazee's VBO Tut - %d Triangles, %d FPS", g_pMesh->m_nVertexCount / 3, g_nFPS );
        if( g_fVBOSupported )									// Include A Notice About VBOs
            strcat( szTitle, ", Using VBOs" );
        else
            strcat( szTitle, ", Not Using VBOs" );
        SetWindowText( g_window->hWnd, szTitle );				// Set The Title
    }
    g_nFrames++;												// Increment Our FPS Counter

    // Move The Camera
    glTranslatef( 0.0f, -0.0f, -75.0f );						// Move Above The Terrain
    glRotatef( 10.0f, 1.0f, 0.0f, 0.0f );						// Look Down Slightly
    glRotatef( g_flYRot, 0.0f, 1.0f, 0.0f );					// Rotate The Camera
    glScalef(25, 25, 25);

    // Enable Pointers
    glEnableClientState( GL_VERTEX_ARRAY );
    // Enable Vertex Arrays
    //glEnableClientState( GL_INDEX_ARRAY);
    //glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays

    // Set Pointers To Our Data
    //if( g_fVBOSupported )
    //{
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_pMesh->m_nVBOVertices );
        glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );		// Set The Vertex Pointer To The Vertex Buffer
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, g_pMesh->m_nIndicies );

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

 glDrawElements(
     GL_TRIANGLE_STRIP,      // mode
     global_geom.indexCount,    // count
     GL_UNSIGNED_INT,   // type
     (void*)0           // element array buffer offset
 );
    //	//glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );		// Set The TexCoord Pointer To The TexCoord Buffer
    //} elseit
    //{
    //	glVertexPointer( 3, GL_FLOAT, 0, g_pMesh->m_pVertices ); // Set The Vertex Pointer To Our Vertex Data
    //	glTexCoordPointer( 2, GL_FLOAT, 0, g_pMesh->m_pTexCoords ); // Set The Vertex Pointer To Our TexCoord Data
    //}

    // Render
    //glDrawArrays( GL_POINTS, 0, global_geom.vertCount );	// Draw All Of The Triangles At Once


 for(int i=0; i<global_model.size1; i++)
 {


    glBegin(GL_TRIANGLES);
    if(global_model.geom[i].vbo.stride == 28)
    {
    vert28 *verts = (vert28*)global_model.geom[i].vbo.vbo;
    for(int j=0; j<global_model.geom[i].indexCount; j++ )
        {
            vert28 &vert = verts[global_model.geom[i].ibo.indicies[j]];
            const unsigned int color = vert.color;
            glColor3ub((color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));

            glVertex3f(vert.x, vert.y, vert.z);

            //glVertex3f(global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3], global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3+1], global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3+2]);
        }
    glEnd();
    }
    else
    {
            vert24 *verts = (vert24*)global_model.geom[i].vbo.vbo;
    for(int j=0; j<global_model.geom[i].indexCount; j++ )
        {
            vert24 &vert = verts[global_model.geom[i].ibo.indicies[j]];
            const unsigned int color = vert.color;
            glColor3ub((color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));

            glVertex3f(vert.x, vert.y, vert.z);

            //glVertex3f(global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3], global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3+1], global_geom.vbo.verts_only[global_geom.ibo.indicies[i]*3+2]);
        }
    glEnd();
    }
 }

    // Disable Pointers
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState(GL_INDEX_ARRAY);// Disable Vertex Arrays
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
}

CMesh :: CMesh()
{
    // Set Pointers To NULL
    //m_pTextureImage = NULL;
    m_pVertices = NULL;
    m_pTexCoords = NULL;
    m_nVertexCount = 0;
    m_nVBOVertices = m_nVBOTexCoords = m_nTextureId = 0;
}

CMesh :: ~CMesh()
{
    // Delete VBOs
    if( g_fVBOSupported )
    {
        unsigned int nBuffers[2] = { m_nVBOVertices, m_nVBOTexCoords };
        glDeleteBuffersARB( 2, nBuffers );						// Free The Memory
    }
    // Delete Data
    if( m_pVertices )											// Deallocate Vertex Data
        delete [] m_pVertices;
    m_pVertices = NULL;
    if( m_pTexCoords )											// Deallocate Texture Coord Data
        delete [] m_pTexCoords;
    m_pTexCoords = NULL;
}

bool CMesh :: LoadHeightmap( char* szPath, float flHeightScale, float flResolution )
{
    // Error-Checking
    FILE* fTest = fopen( szPath, "r" );							// Open The Image
    if( !fTest )												// Make Sure It Was Found
        return false;											// If Not, The File Is Missing
    fclose( fTest );											// Done With The Handle

    // Load Texture Data
    //m_pTextureImage = auxDIBImageLoad( szPath );				// Utilize GLaux's Bitmap Load Routine

    // Generate Vertex Field
    m_nVertexCount = (int) ( 1024 * 768 * 6 / ( flResolution * flResolution ) );
    m_pVertices = new CVec[m_nVertexCount];						// Allocate Vertex Data
    m_pTexCoords = new CTexCoord[m_nVertexCount];				// Allocate Tex Coord Data
    int nX, nZ, nTri, nIndex=0;									// Create Variables
    float flX, flZ;
    for( nZ = 0; nZ < 768; nZ += (int) flResolution )
    {
        for( nX = 0; nX < 1024; nX += (int) flResolution )
        {
            for( nTri = 0; nTri < 6; nTri++ )
            {
                // Using This Quick Hack, Figure The X,Z Position Of The Point
                flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? flResolution : 0.0f );
                flZ = (float) nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? flResolution : 0.0f );

                // Set The Data, Using PtHeight To Obtain The Y Value
                m_pVertices[nIndex].x = flX - ( 1024 / 2 );
                m_pVertices[nIndex].y = PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
                m_pVertices[nIndex].z = flZ - (  768 / 2 );

                // Stretch The Texture Across The Entire Mesh
                m_pTexCoords[nIndex].u = flX / 1024;
                m_pTexCoords[nIndex].v = flZ / 768;

                // Increment Our Index
                nIndex++;
            }
        }
    }

    // Load The Texture Into OpenGL
    glGenTextures( 1, &m_nTextureId );							// Get An Open ID
    //glBindTexture( GL_TEXTURE_2D, m_nTextureId );				// Bind The Texture
    //glTexImage2D( GL_TEXTURE_2D, 0, 3, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //// Free The Texture Data
    //if( m_pTextureImage )
    //{
    //	if( m_pTextureImage->data )
    //		free( m_pTextureImage->data );
    //	free( m_pTextureImage );
    //}
    return true;
}

float CMesh :: PtHeight( int nX, int nY )
{
    // Calculate The Position In The Texture, Careful Not To Overflow
    //int nPos = ( ( nX % m_pTextureImage->sizeX )  + ( ( nY % m_pTextureImage->sizeY ) * m_pTextureImage->sizeX ) ) * 3;
    //float flR = (float) m_pTextureImage->data[ nPos ];			// Get The Red Component
    //float flG = (float) m_pTextureImage->data[ nPos + 1 ];		// Get The Green Component
    //float flB = (float) m_pTextureImage->data[ nPos + 2 ];		// Get The Blue Component
    //return ( 0.299f * flR + 0.587f * flG + 0.114f * flB );		// Calculate The Height Using The Luminance Algorithm
    return 1;
}

void CMesh :: BuildVBOs(geometry geom)
{

    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );							// Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );			// Bind The Buffer
    // Load The Data
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, geom.vbo.vertCount*3*sizeof(float), geom.vbo.verts_only, GL_STATIC_DRAW_ARB );

    glGenBuffersARB(1, &m_nIndicies);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_nIndicies);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, geom.indexCount * 2, (void*)geom.ibo.indicies, GL_STATIC_DRAW_ARB);

    //// Generate And Bind The Texture Coordinate Buffer
    //glGenBuffersARB( 1, &m_nVBOTexCoords );							// Get A Valid Name
    //glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );		// Bind The Buffer
    //// Load The Data
    //glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*2*sizeof(float), m_pTexCoords, GL_STATIC_DRAW_ARB );

    // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
    //delete [] m_pVertices; m_pVertices = NULL;
    //delete [] m_pTexCoords; m_pTexCoords = NULL;
}
