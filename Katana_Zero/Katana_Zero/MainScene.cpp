#include "stdafx.h"
#include "MainScene.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"
#include "SceneManager.h"
CMainScene::CMainScene()
	:fScrool(0)
	,iPlantCount(-1)
	,reverse(false)
{
}


CMainScene::~CMainScene()
{
}

CScene * CMainScene::Create()
{
	CMainScene* pMain = new CMainScene();
	if (FAILED(pMain->Ready_Scene()))
	{
		Safe_Delete(pMain);
		return pMain;
	}
	return pMain;
}

HRESULT CMainScene::Ready_Scene()
{

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"TitleBG", 0);
	m_fMapWidth = float(pTexInfo->tImageInfo.Width);
	m_fMapHeight = float(pTexInfo->tImageInfo.Height);

	CSoundMgr::Get_Instance()->PlayBGM(L"song_ending_2.ogg");
	return S_OK;
}

void CMainScene::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8001)
	{
		reverse = true;
	}
	if (!reverse)
	{
		if (fScrool > 150)
			return;
		fScrool += 5.f;
		CScrollManager::Set_ScroolY(fScrool);
	}
	if (reverse)
	{
		if (CScrollManager::Get_ScroolY() < 0)
		{
			SceneManager->Change_SceneManager(SceneManager->Get_CurScene() + 1);
			CScrollManager::Set_ScroolY(0.f);
			return;
		}

		fScrool -= 5.f;
		CScrollManager::Set_ScroolY(fScrool);
	}


}

void CMainScene::Render_Scene()
{
	RECT rc = { CScrollManager::Get_ScroolX(),CScrollManager::Get_ScroolY(), WINCX + CScrollManager::Get_ScroolX(), WINCY + CScrollManager::Get_ScroolY() };
	iPlantCount += 1;
	if (iPlantCount > 95)
		iPlantCount = 0;

	Device->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"TitleBG", 0);
	if (nullptr == pTexInfo)
		return;


	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale, 2.0f, 1.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	D3DXMATRIX matScale2, matTrans2, matWorld2;

	const TEXINFO* pTexInfo2 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"TitleFence", 0);
	if (nullptr == pTexInfo2)
		return;


	float fCenterX2 = float(pTexInfo2->tImageInfo.Width >> 1);
	float fCenterY2 = float(pTexInfo2->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale2, 2.f, 1.8f, 0.f);
	D3DXMatrixTranslation(&matTrans2, WINCX / 2, WINCY / 2, 0.f);
	matWorld2 = matScale2 * matTrans2;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld2);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, &rc, &D3DXVECTOR3(fCenterX2, fCenterY2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matScale3, matTrans3, matWorld3;

	const TEXINFO* pTexInfo3 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"ZER", 0);
	if (nullptr == pTexInfo3)
		return;


	float fCenterX3 = float(pTexInfo3->tImageInfo.Width >> 1);
	float fCenterY3 = float(pTexInfo3->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale3, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans3, WINCX / 2-100, WINCY / 2 + 260 - 2 * CScrollManager::Get_ScroolY(), 0.f);
	matWorld3 = matScale3 * matTrans3;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld3);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo3->pTexture, nullptr, &D3DXVECTOR3(fCenterX3, fCenterY3, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matScale5, matTrans5, matWorld5;

	const TEXINFO* pTexInfo5 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"O", 0);
	if (nullptr == pTexInfo5)
		return;


	float fCenterX5 = float(pTexInfo5->tImageInfo.Width >> 1);
	float fCenterY5 = float(pTexInfo5->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale5, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans5, WINCX / 2 + 110, WINCY / 2 + 260 - 2 * CScrollManager::Get_ScroolY(), 0.f);
	matWorld5 = matScale5 * matTrans5;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld5);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &D3DXVECTOR3(fCenterX5, fCenterY5, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	D3DXMATRIX matScale4, matTrans4, matWorld4;

	const TEXINFO* pTexInfo4 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"Katana", 0);
	if (nullptr == pTexInfo4)
		return;


	float fCenterX4 = float(pTexInfo4->tImageInfo.Width >> 1);
	float fCenterY4 = float(pTexInfo4->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale4, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans4, WINCX / 2, WINCY / 2 +160 - 2*CScrollManager::Get_ScroolY(), 0.f);
	matWorld4 = matScale4 * matTrans4;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld4);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &D3DXVECTOR3(fCenterX4, fCenterY4, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	D3DXMATRIX matScale7, matTrans7, matWorld7;
	const TEXINFO* pTexInfo7 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"Plants", iPlantCount/8);
	if (nullptr == pTexInfo7)
		return;
	float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
	float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale7, 2.0f, 1.8f, 0.f);
	D3DXMatrixTranslation(&matTrans7, WINCX / 2, WINCY / 2 + 400 - 2 * CScrollManager::Get_ScroolY(), 0.f);
	matWorld7 = matScale7 * matTrans7;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld7);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &D3DXVECTOR3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMATRIX matScale6, matTrans6, matWorld6;

	const TEXINFO* pTexInfo6 = Texture_Maneger->Get_TexInfo_Manager(L"Main", L"Grass", 0);
	if (nullptr == pTexInfo6)
		return;
	float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
	float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
	D3DXMatrixScaling(&matScale6, 2.0f, 1.8f, 0.f);
	D3DXMatrixTranslation(&matTrans6, WINCX / 2, WINCY / 2 + 580 - 2 * CScrollManager::Get_ScroolY(), 0.f);
	matWorld6 = matScale6 * matTrans6;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld6);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &D3DXVECTOR3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	Device->Render_End();
}

void CMainScene::Release_Scene()
{
}
