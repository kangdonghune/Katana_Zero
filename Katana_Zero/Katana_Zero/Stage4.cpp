#include "stdafx.h"
#include "Stage4.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"
#include "GameObjectManager.h"
#include "MapObjectManager.h"
#include "FrameManager.h"
#include "SaveLoadManager.h"


CStage4::CStage4()
{
}


CStage4::~CStage4()
{
}

CScene * CStage4::Create()
{
	CStage4* pStage4 = new CStage4();
	if (FAILED(pStage4->Ready_Scene()))
	{
		Safe_Delete(pStage4);
		return pStage4;
	}
	return pStage4;
}

HRESULT CStage4::Ready_Scene()
{
	//맵 오브젝트 생성
	MapObjectManager->Load_Terrain(L"../Data/Stage4/Terrain/Terrain.dat");
	//유닛 정보 불러오기.
	if (FAILED(SaveLoadManager->LoadUnit(L"../Data/Stage4/Unit/Unit.dat")))
		return E_FAIL;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 3);
	m_fMapWidth = float(pTexInfo->tImageInfo.Width);
	m_fMapHeight = float(pTexInfo->tImageInfo.Height);
	return S_OK;
}

void CStage4::Update_Scene()
{
}

void CStage4::Render_Scene()
{
	Device->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 3);
	if (nullptr == pTexInfo)
		return;


	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	RECT rc = { CScrollManager::Get_ScroolX(),CScrollManager::Get_ScroolY(), WINCX + CScrollManager::Get_ScroolX(), WINCY + CScrollManager::Get_ScroolY() };
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (GetAsyncKeyState(VK_CONTROL) & 0X8001)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 150, 150, 150));
	}
	MapObjectManager->Render_MapObjectManager();
	GameObjectManager->Render_GameObjectManager();
	FrameManager->Render_Frame_Manager();
	Device->Render_End();
}

void CStage4::Release_Scene()
{
}
