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
	int ID = 0;
}MYLINE;

typedef struct tagUnitInfo
{
	D3DXVECTOR3	D3VecPos{};
	wstring	wstrState = L"Idle";
	wstring wstrKey = L"";
	UNITTYPE::TYPE type;
	int	iCollide = C_NONE;
}UNITINFO;

typedef struct tagItemInfo
{
	D3DXVECTOR3	D3VecPos{};
	wstring	wstrState = L"";
	wstring wstrKey = L"Projectile";
	ITEMTYPE::TYPE type;
}ITEMINFO;

typedef	struct tagFrame
{
	float fFrameStart;
	float fFrameEnd;
}FRAME;

typedef struct tagHitBox
{
	D3DXVECTOR3 D3Dot1;
	D3DXVECTOR3 D3Dot2;
	D3DXVECTOR3 D3Dot3;
	D3DXVECTOR3 D3Dot4;

}HITBOX;