#include "stdafx.h"
#include "C_Map.h"
#include "Texture_Manager.h"
#include "Multi_Texture.h"
#include "MapToolView.h"
#include "RECT.h"

C_Map::C_Map()
	:m_NowStage(Stage1)
	,m_pView(nullptr)
{
}


C_Map::~C_Map()
{
	Release_Map();
}

HRESULT C_Map::Ready_Map()
{
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Map/stage%d.bmp", L"Map", L"Stage", 6)))
		return E_FAIL;
	return S_OK;
}

void C_Map::Update_Map()
{
}

void C_Map::Render_Map()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 2);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX - m_pView->GetScrollPos(SB_HORZ), fCenterY - m_pView->GetScrollPos(SB_VERT),0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void C_Map::Release_Map()
{
	//Safe_Delete(m_pView);
}

void C_Map::ChangeMap(Stages StageNum)
{
}
