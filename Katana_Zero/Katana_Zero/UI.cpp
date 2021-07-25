#include "stdafx.h"
#include "UI.h"
#include "GameObjectManager.h"
#include "ScrollManager.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "FrameManager.h"
#include "Player.h"

CUI::CUI()
	:m_fTimer(100)
	,m_fSlowTimer(9.f)
	,InventoryNum(0)
{
}


CUI::~CUI()
{
}

void CUI::Setting_Clear()
{
	m_fTimer = 100.f;
	m_fSlowTimer = 9.f;
	InventoryNum = 0;
}

CGameObject * CUI::Create()
{
	CUI* pUI = new CUI();
	if (FAILED(pUI->Ready_GameObject()))
	{
		Safe_Delete(pUI);
		return pUI;
	}
	return pUI;
}

HRESULT CUI::Ready_GameObject()
{
	m_fWidthRatio = 2.f;
	m_fHeightRatio = 2.3f;
	return S_OK;
}

void CUI::Update_GameObject()
{
	m_fTimer -= TimeManager->Get_DeltaTime() *(FrameManager->Get_FPS() / 60);
	if (GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).empty())
		InventoryNum = 0;
	if (!GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).empty())
	{
		switch (dynamic_cast<CPlayer*>(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front())->Get_ItemType())
		{
		case ITEMTYPE::END:
			InventoryNum = 0;
			break;
		case ITEMTYPE::BUTCHERKNIFE:
			InventoryNum = 1;
			break;
		case ITEMTYPE::SMOKE:
			InventoryNum = 2;
			break;
		case ITEMTYPE::EXPLOSIVEVIAL:
			InventoryNum = 3;
			break;
		default:
			break;
		}
	}
	
}

void CUI::LateUpdate_GameObject()
{
}

void CUI::Render_GameObject()
{
	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"UI", L"Hud", 0);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"UI에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_fWidthRatio, m_fHeightRatio, 0.f);
	D3DXMatrixTranslation(&matTrans, 640,0 + m_fHeightRatio*fCenterY , 0.f);
	matWorld = matScale *matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matScale1, matTrans1, matWorld1;

	const TEXINFO* pTexInfo1 = Texture_Maneger->Get_TexInfo_Manager(L"UI", L"Inventory", InventoryNum);
	if (nullptr == pTexInfo1)
	{
		ERR_MSG(L"UI에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX1 = pTexInfo1->tImageInfo.Width >> 1;
	float fCenterY1 = pTexInfo1->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale1, m_fWidthRatio, m_fHeightRatio, 0.f);
	D3DXMatrixTranslation(&matTrans1, 1200, 0 + m_fHeightRatio*fCenterY1, 0.f);
	matWorld1 = matScale1 *matTrans1;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(fCenterX1, fCenterY1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matScale2, matTrans2, matWorld2;
	const TEXINFO* pTexInfo2 = Texture_Maneger->Get_TexInfo_Manager(L"UI", L"Battery", 9);
	if (nullptr == pTexInfo2)
	{
		ERR_MSG(L"UI에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX2 = pTexInfo2->tImageInfo.Width >> 1;
	float fCenterY2 = pTexInfo2->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale2, m_fWidthRatio, m_fHeightRatio, 0.f);
	D3DXMatrixTranslation(&matTrans2, 100, 10 + m_fHeightRatio*fCenterY2, 0.f);
	matWorld2 = matScale2 *matTrans2;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld2);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(fCenterX1, fCenterY1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matScale4, matTrans4, matWorld4;
	const TEXINFO* pTexInfo4 = Texture_Maneger->Get_TexInfo_Manager(L"UI", L"TimerFrame", 0);
	if (nullptr == pTexInfo4)
	{
		ERR_MSG(L"UI에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX4 = pTexInfo4->tImageInfo.Width >> 1;
	float fCenterY4 = pTexInfo4->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale4, m_fWidthRatio - 0.4, m_fHeightRatio - 0.4, 0.f);
	D3DXMatrixTranslation(&matTrans4, 580, 5 + m_fHeightRatio*fCenterY4, 0.f);
	matWorld4 = matScale4 *matTrans4;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld4);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &D3DXVECTOR3(fCenterX1, fCenterY1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMATRIX matScale3, matTrans3, matWorld3;
	const TEXINFO* pTexInfo3 = Texture_Maneger->Get_TexInfo_Manager(L"UI", L"Timer", 0);
	if (nullptr == pTexInfo3)
	{
		ERR_MSG(L"UI에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX3 = pTexInfo3->tImageInfo.Width >> 1;
	float fCenterY3 = pTexInfo3->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale3, (m_fWidthRatio - 0.42)*(m_fTimer/100.f), m_fHeightRatio - 0.42, 0.f);
	D3DXMatrixTranslation(&matTrans3, 563, 4 + m_fHeightRatio*fCenterY3, 0.f);
	matWorld3 = matScale3 *matTrans3;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld3);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo3->pTexture, nullptr, &D3DXVECTOR3(0.f, fCenterY3, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CUI::Release_GameObject()
{
}
