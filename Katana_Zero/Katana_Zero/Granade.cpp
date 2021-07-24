#include "stdafx.h"
#include "Granade.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "Gangster.h"
#include "ScrollManager.h"

CGranade::CGranade()
{
}


CGranade::~CGranade()
{
}

CGameObject * CGranade::Create(CGameObject * pShooter)
{
	CGranade* pGranade = new CGranade();
	pGranade->Set_Shooter(pShooter);
	if (FAILED(pGranade->Ready_GameObject()))
	{
		Safe_Delete(pGranade);
		return pGranade;
	}
	return pGranade;
}

void CGranade::DrawRange()
{
	D3DXVECTOR2 Line[128];
	float Step = g_PI * 2.0 / 40;
	int Count = 0;
	for (float a = 0; a < g_PI*2.0; a += Step)
	{
		float X1 = m_fRadius*cos(a) + m_pUnitInfo->D3VecPos.x  - CScrollManager::Get_ScroolX();
		float Y1 = m_fRadius * sin(a) + m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY();
		float X2 = m_fRadius * cos(a + Step) + m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX();
		float Y2 = m_fRadius * sin(a + Step) + m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY();
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	Device->m_pLine->Draw(Line, Count, D3DCOLOR_ARGB(255, 255, 0, 0));
}

HRESULT CGranade::Ready_GameObject()
{
	m_pUnitInfo = new UNITINFO{};
	m_pUnitInfo->wstrKey = L"Projectile";
	m_pUnitInfo->wstrState = L"Granade";
	m_fRatio = 1.f;
	m_fWidthRatio = m_fRatio;
	m_fHeightRatio = 1.f;
	m_fRotateAngle = m_pShooter->Get_RotateAngle();
	m_fTargetAngle = m_pShooter->Get_TargetAngle();
	m_pUnitInfo->D3VecPos.x = m_pShooter->Get_UnitInfo()->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle))*(m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Width >> 1)) + cosf(D3DXToRadian(m_fTargetAngle))*(m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Width >> 1));
	m_pUnitInfo->D3VecPos.y = m_pShooter->Get_UnitInfo()->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle))* (m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Height >> 1));
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 20.f;
	m_fUnitSpeed = 25.f;
	m_fRadius = 5*Texture_Maneger->Get_TexInfo_Manager(L"Effect", L"Explosion",0)->tImageInfo.Height>>1;
	m_fHeight = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Height;
	m_iUnitDir = m_pShooter->Get_UnitDir();

	return S_OK;
}

void CGranade::Update_GameObject()
{
	m_fRatio = m_fWidthRatio;
	m_pUnitInfo->D3VecPos.x += cosf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
	m_pUnitInfo->D3VecPos.y -= sinf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
	Update_HitBoxOBB();
}

void CGranade::LateUpdate_GameObject()
{
}

void CGranade::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"Bullet 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matRevolve, matParent, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Effect에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_fWidthRatio, m_fHeightRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY(), 0.f);
	matWorld = matScale * matRolateZ *matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	Render_HitBoxObb();
	Render_ObbLineD3D();
	DrawRange();
}

void CGranade::Release_GameObject()
{
}
