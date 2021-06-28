#include "stdafx.h"
#include "Main.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"


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
	}
	return pMain;
}

HRESULT CMain::Ready_Main()
{
	if (FAILED(Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return E_FAIL;
	}

	if (FAILED(Texture_Maneger->Init_Texture_Manager()))
		return E_FAIL;
	return S_OK;
}

void CMain::Update_Main()
{
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

	Device->Render_End();
}

void CMain::Release_Main()
{
}
