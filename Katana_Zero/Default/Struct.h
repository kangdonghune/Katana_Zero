#pragma once
typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9 pTexture;// �ؽ��� ������ �����ϱ����� �İ�ü. 
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