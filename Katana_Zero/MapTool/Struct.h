#pragma once
typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9 pTexture;// 텍스쳐 한장을 보관하기위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo;

}TEXINFO;

typedef struct tagRedefineRect
{
	RECT rc;
	TERRAINTYPE::TYPE type;
}MYRECT;


typedef struct tagRedefineLine
{
	POINT Start;
	POINT End;
	TERRAINTYPE::TYPE type;
}MYLINE;