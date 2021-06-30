#pragma once

//window 크기
#define WINCX 1280
#define WINCY 760

//싱글턴 추가

//클라이언트
#define GameObjectManager	CGameObjectManager::Get_Instance()
#define MapObjectManager	CMapObjectManager::Get_Instance()

//맵툴
#define Device			CGraphic_Device::Get_Instance()
#define Texture_Maneger CTexture_Manager::Get_Instance()
#define RECTS			CRECT::Get_Instance()
#define UNITS			CUnit::Get_Instance()

//싱글턴 추가 종료

//툴 상태
#define Tool_Idle		0b00000000000000000000000000000000
#define TOOL_Wall		0b00000000000000000000000000000001
#define	TOOL_Land		0b00000000000000000000000000000010
#define	TOOL_Celling	0b00000000000000000000000000000100
#define TOOL_Player		0b00000000000000000000000000001000


//PURE

#define PURE = 0