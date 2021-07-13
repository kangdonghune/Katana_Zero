#include "stdafx.h"
#include "Main.h"
#include "Graphic_Device.h"
#include "MapObjectManager.h"
#include "Texture_Manager.h"
#include "SaveLoadManager.h"
#include "GameObjectManager.h"
#include "FrameManager.h"
#include "TimeManager.h"
#include "ColliderManager.h"

CMain::CMain()
{
}


CMain::~CMain()
{
	Release_Main();
}

CMain * CMain::Create()
{
	CMain* pMain = new CMain();
	if (FAILED(pMain->Ready_Main()))
	{
		Safe_Delete(pMain);
		return pMain;
	}
	return pMain;
}

HRESULT CMain::Ready_Main()
{
	//타임 매니저 생성
	TimeManager->Ready_TimeManager();

	//디바이스 생성
	if (FAILED(Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return E_FAIL;
	}
	//텍스쳐 삽입
	if (FAILED(Texture_Maneger->Init_Texture_Manager()))
		return E_FAIL;
	
	//맵 오브젝트 생성
	MapObjectManager->Init_MapObjectManager();
	
	//유닛 정보 불러오기.
	if (FAILED(SaveLoadManager->LoadUnit(L"../Data/Stage1/Unit/Unit.dat")))
		return E_FAIL;

	return S_OK;
}

void CMain::Update_Main()
{
	TimeManager->Update_TimeManager();
	GameObjectManager->Update_GameObjectManager();
	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());

	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	
	ColliderManager->Collider_Obb(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
}

void CMain::LateUpdate_Main()
{
}

void CMain::Render_Main()
{
	Device->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (GetAsyncKeyState(VK_CONTROL) & 0X8001)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 150, 150, 150));
	}
	MapObjectManager->Render_MapObjectManager();
	GameObjectManager->Render_GameObjectManager();
	FrameManager->Render_Frame_Manager();
	Device->Render_End();
}

void CMain::Release_Main()
{
	MapObjectManager->Destroy_Instance();
	GameObjectManager->Destroy_Instance();
	FrameManager->Destroy_Instance();
	Device->Destroy_Instance();
}
