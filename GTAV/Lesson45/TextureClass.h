#include <stdio.h>
#include <vector>
#include <string>
#include "ByteStream.h"
using namespace std;
struct texture_header{
int VTable;
int BlockMapOffset; // Offset to the block map
int ParentDictionary;
int UsageCount;
int HashTableOffset; // Offset to the hash table
short TextureCount; // Number of textures in this dictionary
short TextureCount2; // Repeated texture count
int TextureListOffset; // Offset to the texture list
short TextureCount3; // Repeated texture count
short TextureCount4; // Repeated texture count
};

 


             struct TextureInfoEx{
             short Unknown1; // 0x0020
             short Unknown2; // 0x0003
             int Unknown3; // 0x00000001
             int Unknown4; // 0x00000000
             int Unknown5; // 0x00000000
             int Unknown6; // 0x00000000
             int Unknown7; // 0xFFFF0000
             int Unknown8; // 0xFFFF0000
             int Unknown9; // 0x84000002
             int TextureType;//.DXT1
             int GpuTextureDataOffset; // 0x60000054, need to remove the upper and lower 8 bits
           
             int Unknown10; // 0x003FE1FF
             int Unknown11; // 0x00000D10
             int Unknown12; // 0x00000000
             int Unknown13; // 0x00000200
             unsigned char Padding[0xC]; // 12 bytes of 0xCD padding
             };

 struct TextureInfo{
 
             int VTable;
             int BlockMapOffset; // 0x00000000
             int Unknown1; // 0x00000001
             int Unknown2; // 0x00000000
             int Unknown3; // 0x00000000
             int Unknown44; // 0x00000001
             int Unknown5; // 0x00000000
             int Unknown6; // 0x00000000
        
             int NameOffset; // Null terminated texture name
             int Unknown7; // 0x00000001 Maybe number of levels??
             int Unknown8; // 0x00000001 Maybe number of levels??
             int Unknown9; // 0x00000001 Maybe number of levels??
             int Unknown10; // 0x00000001 Maybe number of levels??
             int Unknown11; // 0x00000001 Maybe number of levels??
             short Width; // Width of texture
             short Height; // Height of texture

             int Unknown12;
            
             string name;
             TextureInfoEx xInfo;

  } ; 


class classTextures{
public:
	texture_header header;
	vector<unsigned long> hashes;
	vector<unsigned long> infoOffsets;
	vector<TextureInfo> TextureInfos;
	vector<classTextures> ExternalTextures;
	void ReadExtraTextureData(ByteStream bs, TextureInfoEx* dst){
		        dst->Unknown1 = bs.getu16();
                dst->Unknown2 = bs.getu16();
                dst->Unknown3 = bs.getu32();
                dst->Unknown4 = bs.getu32();
                dst->Unknown5 = bs.getu32();
                dst->Unknown6 = bs.getu32();
                dst->Unknown7 = bs.getu32();
                dst->Unknown8 = bs.getu32();
                dst->Unknown9 = bs.getu32();
                dst->GpuTextureDataOffset = bs.getu32()&0xFFFFFFF + 0x2010;
                dst->TextureType = dst->GpuTextureDataOffset & 0xFF;//Don't ask, it's weird.
                
                dst->Unknown10 = bs.getu32();
                dst->Unknown11 = bs.getu32();
                dst->Unknown12 = bs.getu32();
                dst->Unknown13 = bs.getu32();
                bs.read(&dst->Padding,12);


	}
	void ReadTextureData(ByteStream bs, TextureInfo* dst){
		unsigned long cur=ftell(bs.getfp());
			    dst->VTable = bs.getu32();
                dst->BlockMapOffset = bs.getu32();
                dst->Unknown1 = bs.getu32();
                dst->Unknown2 = bs.getu32();
                dst->Unknown3 = bs.getu32();
                dst->Unknown44 = bs.getu32();
                dst->Unknown5 = bs.getu32();
                dst->Unknown6 = bs.getu32();
              
                dst->NameOffset = (bs.getu32()&0xFFFFFF) + 0x10;
             
                dst->Unknown7= bs.getu32();
                dst->Unknown8= bs.getu32();
                dst->Unknown9= bs.getu32();
                dst->Unknown10= bs.getu32();
                dst->Unknown11= bs.getu32();
                dst->Width = bs.getu16();
                dst->Height = bs.getu16();
                dst->Unknown12= bs.getu32();
				ReadExtraTextureData(bs, &dst->xInfo);
				fseek(bs.getfp(), dst->NameOffset , SEEK_SET);
				dst->name="";
				while(1){
					char k=fgetc(bs.getfp());
					if(k==0){
						break;
					}
				dst->name+=k;
				}

	}
	void ReadHeader(ByteStream bs, texture_header* hdr){
		hdr->VTable=bs.getu32();;
		hdr->BlockMapOffset=bs.getu32();;// Offset to the block map
		hdr->ParentDictionary=bs.getu32();;
		hdr-> UsageCount=bs.getu32();;
		hdr-> HashTableOffset=bs.getu32();; // Offset to the hash table
		hdr->TextureCount=bs.getu16();;; // Number of textures in this dictionary
		hdr->TextureCount2=bs.getu16();;; // Repeated texture count
		hdr->TextureListOffset=bs.getu32();;; // Offset to the texture list
		hdr->TextureCount3=bs.getu16();;; // Repeated texture count
		hdr->TextureCount4=bs.getu16();;; // Repeated texture count

	}
	classTextures(ByteStream bs){
	   int address=bs.getu32();
		bs.seekg((address&0xFFFFFF) + 0x10);
		ReadHeader(bs, &header);
		bs.seekg((header.HashTableOffset&0xFFFFFF) + 0x10);
        for (int i = 0; i < header.TextureCount; i++)
			hashes.push_back(bs.getu32());

            // Read our info offsets

		bs.seekg((header.TextureListOffset&0xFFFFFF) + 0x10);
        for (int i = 0; i < header.TextureCount; i++){
			infoOffsets.push_back(bs.getu32());
		}
            // Read our texture info
            
            for (int i = 0; i < header.TextureCount; i++)
            {
               bs.seekg((infoOffsets[i]&0xFFFFFFF) + 0x10);
			   TextureInfo ti;
			   ReadTextureData(bs, &ti);
               TextureInfos.push_back(ti);
            }

	}


};