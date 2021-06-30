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

typedef struct tagUnitInfo
{
	D3DXVECTOR3	D3VecPos;
	wstring	wstrState = L"Idle";
	wstring wstrKey = L"";
	UNITTYPE::TYPE type;
	
}UNITINFO;