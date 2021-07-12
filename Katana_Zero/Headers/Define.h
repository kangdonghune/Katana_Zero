#pragma once

//window ũ��
#define WINCX 1280
#define WINCY 720

//�̱��� �߰�

//Ŭ���̾�Ʈ
#define GameObjectManager	CGameObjectManager::Get_Instance()
#define MapObjectManager	CMapObjectManager::Get_Instance()
#define SaveLoadManager		CSaveLoadManager::Get_Instance()
#define TimeManager			CTimeManager::Get_Instance()
#define FrameManager		CFrameManager::Get_Instance()
#define ColliderManager		CColliderManager::Get_Instance()

//����
#define Device			CGraphic_Device::Get_Instance()
#define Texture_Maneger CTexture_Manager::Get_Instance()
#define RECTS			CRECT::Get_Instance()
#define UNITS			CUnit::Get_Instance()

//�̱��� �߰� ����

//�� ����
#define Tool_Idle		0b00000000000000000000000000000000
#define TOOL_Wall		0b00000000000000000000000000000001
#define	TOOL_Land		0b00000000000000000000000000000010
#define	TOOL_Celling	0b00000000000000000000000000000100
#define TOOL_Player		0b00000000000000000000000000001000
#define TOOL_Gangster	0b00000000000000000000000000010000

//Player �浹 ����
#define	C_NONE			0b00000000000000000000000000000001
#define C_LAND			0b00000000000000000000000000000010
#define C_WALL			0b00000000000000000000000000000100
#define C_WALLL			0b00000000000000000000000000001000
#define C_WALLR			0b00000000000000000000000000010000
#define C_CELLING		0b00000000000000000000000000100000


//Player Ȱ�� ���� 
#define ACTING			0b00000000000000000000000000000000
#define	NOACTING		0b00000000000000000000000000000001
//PURE


//������Ʈ ����
#define	NONE			0b00000000000000000000000000000001
#define DEAD			0b00000000000000000000000000000010
#define COLLIDE			0b00000000000000000000000000000100

#define PURE = 0