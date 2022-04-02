#include "stdafx.h"
#include "Stage3.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"
#include "GameObjectManager.h"
#include "MapObjectManager.h"
#include "FrameManager.h"
#include "SaveLoadManager.h"
#include "UI.h"
CStage3::CStage3()
{
}


CStage3::~CStage3()
{

}

CScene * CStage3::Create()
{
	CStage3* pStage3 = new CStage3();
	if (FAILED(pStage3->Ready_Scene()))
	{
		Safe_Delete(pStage3);
		return pStage3;
	}
	return pStage3;
}

HRESULT CStage3::Ready_Scene()
{	
	//맵 오브젝트 생성
	MapObjectManager->Load_Terrain(L"../Data/Stage3/Terrain/Terrain.dat");
	//유닛 정보 불러오기.
	if (FAILED(SaveLoadManager->LoadUnit(L"../Data/Stage3/Unit/Unit.dat")))
		return E_FAIL;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 2);
	m_fMapWidth = float(pTexInfo->tImageInfo.Width);
	m_fMapHeight = float(pTexInfo->tImageInfo.Height);
	GameObjectManager->Insert_GameObjectManager(CUI::Create(), GAMEOBJECT::UI);
	return S_OK;

}

void CStage3::Update_Scene()
{
}

void CStage3::Render_Scene()
{
	Device->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 2);
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
	CScene::Render_Scene();
	Device->Render_End();
}

void CStage3::Release_Scene()
{
}
