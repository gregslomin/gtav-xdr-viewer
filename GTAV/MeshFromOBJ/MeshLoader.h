//--------------------------------------------------------------------------------------
// File: MeshLoader.h
//
// Wrapper class for ID3DXMesh interface. Handles loading mesh data from an .obj file
// and resource management for material textures.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _MESHLOADER_H_
#define _MESHLOADER_H_
#pragma once
#include <iostream>
#include <fstream>
#include "ByteStream.h"
using namespace std;

//--------------------------------------
//--- 010 Editor v5.0 Binary Template
//
// File:
// Author:
// Revision:
// Purpose:
//--------------------------------------
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

// Vertex format
struct VERTEX
{
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
};


// Used for a hashtable vertex cache when creating the mesh from a .obj file
struct CacheEntry
{
    UINT index;
    CacheEntry* pNext;
};


// Material properties per mesh subset
struct Material
{
    WCHAR   strName[MAX_PATH];

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int nShininess;
    float fAlpha;

    bool bSpecular;

    WCHAR   strTexture[MAX_PATH];
    IDirect3DTexture9* pTexture;
    D3DXHANDLE hTechnique;
};


class CMeshLoader
{
public:
            CMeshLoader();
            ~CMeshLoader();

    HRESULT Create( IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename );
    void    Destroy();


    UINT    GetNumMaterials() const
    {
        return m_Materials.GetSize();
    }
    Material* GetMaterial( UINT iMaterial )
    {
        return m_Materials.GetAt( iMaterial );
    }

    ID3DXMesh* GetMesh()
    {
        return m_pMesh;
    }
    WCHAR* GetMediaDirectory()
    {
        return m_strMediaDir;
    }

private:

    HRESULT LoadGeometryFromOBJ( const WCHAR* strFilename );
    HRESULT LoadMaterialsFromMTL( const WCHAR* strFileName );
    void    InitMaterial( Material* pMaterial );

    DWORD   AddVertex( UINT hash, VERTEX* pVertex );
    void    DeleteCache();

    IDirect3DDevice9* m_pd3dDevice;    // Direct3D Device object associated with this mesh
    ID3DXMesh* m_pMesh;         // Encapsulated D3DX Mesh
	geometry geom;
    CGrowableArray <CacheEntry*> m_VertexCache;   // Hashtable cache for locating duplicate vertices
    CGrowableArray <VERTEX> m_Vertices;      // Filled and copied to the vertex buffer
    CGrowableArray <DWORD> m_Indices;       // Filled and copied to the index buffer
    CGrowableArray <DWORD> m_Attributes;    // Filled and copied to the attribute buffer
    CGrowableArray <Material*> m_Materials;     // Holds material properties per subset

    WCHAR   m_strMediaDir[ MAX_PATH ];               // Directory where the mesh was found
};

#endif // _MESHLOADER_H_

