/********************
*                   *
*   NeHeGL Header   *
*                   *
**********************************************************************************
*                                                                                *
*	You Need To Provide The Following Functions:                                 *
*                                                                                *
*	BOOL Initialize (GL_Window* window, Keys* keys);                             *
*		Performs All Your Initialization                                         *
*		Returns TRUE If Initialization Was Successful, FALSE If Not              *
*		'window' Is A Parameter Used In Calls To NeHeGL                          *
*		'keys' Is A Structure Containing The Up/Down Status Of keys              *
*                                                                                *
*	void Deinitialize (void);                                                    *
*		Performs All Your DeInitialization                                       *
*                                                                                *
*	void Update (DWORD milliseconds);                                            *
*		Perform Motion Updates                                                   *
*		'milliseconds' Is The Number Of Milliseconds Passed Since The Last Call  *
*		With Whatever Accuracy GetTickCount() Provides                           *
*                                                                                *
*	void Draw (void);                                                            *
*		Perform All Your Scene Drawing                                           *
*                                                                                *
*********************************************************************************/

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED

#include <windows.h>								// Header File For Windows

typedef struct {									// Structure For Keyboard Stuff
	BOOL keyDown [256];								// Holds TRUE / FALSE For Each Key
} Keys;												// Keys

typedef struct {									// Contains Information Vital To Applications
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// Window Creation Info
	Application*		application;				// Application Structure
	char*				title;						// Window Title
	int					width;						// Width
	int					height;						// Height
	int					bitsPerPixel;				// Bits Per Pixel
	BOOL				isFullScreen;				// FullScreen?
} GL_WindowInit;									// GL_WindowInit
typedef unsigned int uint32;
typedef unsigned short uint16;
struct vert24
{
	float x, y, z;
	//        if(stride == 28)
	//          float w;
	uint32 color;
	float tu, tv;                     
};
struct vert28
{
	float x, y, z, w;
	//        if(stride == 28)
	//          float w;
	uint32 color;
	float tu, tv;                     
};
struct RSC7_Container{
	uint32  RSC7;
	uint32  RSC7version;
	uint32  RSC7system_flag;
	uint32  RSC7graphics_flag;
	uint32 VTable;

	uint32 BlockMapAddress;
	uint32 ShaderGroup;
	uint32 SkeletonData;
	struct RCS7{
		float x;
		float y;
		float z;
		float w;
	}Vectors[3];
	long modelCollection;
	long lodCollections[3];
	struct{
		float x;
		float y;
		float z;
		float w;
	}maxVector;   
	uint32 unk1;
	uint32 neg[3];
	float  unk2;
	uint32 unks[5];
};


struct IB
{
	uint32 VTable;
	uint32 stride;
	uint32 indexOffset;
	unsigned short *indicies;
};

struct VB
{
	//        FSeek((VBOffset&0xFFFFFFF) + 0x10);
	uint32 VTable;
	uint16 stride;
	uint16 unknown1;
	uint32 dataOffset;
	uint32 vertCount;
	uint32 dataOffset2;
	uint32 null1;
	uint32 cpuOffset1;
	uint32 cpuOffset2;
	void *vbo;
	float *verts_only;

	//      FSeek((dataOffset&0xFFFFFFF) + 0x2010);

};
struct modelCollection
{
	uint32 modelCollectionPointer;
	uint16 count;
	uint16 count2;
} ;

struct geometry
{
	uint32 VTable;
	uint32 null1;
	uint32 null2;
	uint32 VBOffset;
	uint32 null3;
	uint32 null4;
	uint32 null5;
	uint32 IBOffset;
	uint32 null6;
	uint32 null7;
	uint32 null8;
	uint32 indexCount;
	uint32 faceCount;
	uint16 vertCount;
	uint16 unkn_primitiveType;
	VB vbo;
	IB ibo;

};

struct model
{
	uint32 VTable;
	uint32 GeometryCollectionOffset;
	uint16 size1;
	uint16 size2;
	uint32 VectorCollectionOffset;
	uint32 MaterialCollectionOffset;
	uint32 GeometryOffsetArray[4];
	geometry geom[4];
} ;
typedef struct {									// Contains Information Vital To A Window
	Keys*				keys;						// Key Structure
	HWND				hWnd;						// Window Handle
	HDC					hDC;						// Device Context
	HGLRC				hRC;						// Rendering Context
	GL_WindowInit		init;						// Window Init
	BOOL				isVisible;					// Window Visible?
	DWORD				lastTickCount;				// Tick Counter
} GL_Window;										// GL_Window

void TerminateApplication (GL_Window* window);		// Terminate The Application

void ToggleFullscreen (GL_Window* window);			// Toggle Fullscreen / Windowed Mode

// These Are The Function You Must Provide
BOOL Initialize (GL_Window* window, Keys* keys);	// Performs All Your Initialization

void Deinitialize (void);							// Performs All Your DeInitialization

void Update (DWORD milliseconds);					// Perform Motion Updates

void Draw (void);									// Perform All Your Scene Drawing

#endif												// GL_FRAMEWORK__INCLUDED
