#include "stdafx.h"
#include "Laser.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "Gangster.h"
#include "ScrollManager.h"
#include "Boss.h"

CLaser::CLaser()
{
}


CLaser::~CLaser()
{
}

CGameObject * CLaser::Create(CGameObject * pShooter)
{
	CLaser* pLaser = new CLaser();
	pLaser->Set_Shooter(pShooter);
	if (FAILED(pLaser->Ready_GameObject()))
	{
		Safe_Delete(pLaser);
		return pLaser;
	}
	return pLaser;
}

HRESULT CLaser::Ready_GameObject()
{
	m_pUnitInfo = new UNITINFO{};
	m_pUnitInfo->wstrKey = L"Projectile";
	m_pUnitInfo->wstrState = L"Bullet";
	m_fRatio = 0.8f;
	m_fWidthRatio = m_fRatio;
	m_fHeightRatio = 1.f;
	m_fRotateAngle = 0.f;
	m_fTargetAngle = m_pShooter->Get_TargetAngle();
	m_pUnitInfo->D3VecPos.x = m_pShooter->Get_UnitInfo()->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle))*(m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Width >> 1)) + cosf(D3DXToRadian(m_fTargetAngle))*(m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Width >> 1));
	m_pUnitInfo->D3VecPos.y = m_pShooter->Get_UnitInfo()->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle))* (m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Height >> 1));
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 0.f;
	m_fUnitSpeed = 0.f;
	m_iUnitDir = m_pShooter->Get_UnitDir();

	return S_OK;
}

void CLaser::Update_GameObject()
{
	m_fRatio = m_fWidthRatio;
	m_pUnitInfo->D3VecPos.x += cosf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
	m_pUnitInfo->D3VecPos.y -= sinf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
	Update_HitBoxLaser();
	//방향만 이동.
}

void CLaser::LateUpdate_GameObject()
{
}

void CLaser::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"Laser 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matRevolve, matParent, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Laser에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir*m_fWidthRatio, m_fHeightRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_iUnitDir * 30, 0, 0.f);
	D3DXMatrixTranslation(&matParent, m_pShooter->Get_UnitInfo()->D3VecPos.x, m_pShooter->Get_UnitInfo()->D3VecPos.y, 0.f);
	matWorld = matScale*matTrans*matRolateZ*matParent;
	if (dynamic_cast<CBoss*>(m_pShooter)->Get_BossState() == BOSSSTATE::TAKEOUT)
	{
		D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
		matWorld = matScale*matTrans;
	}

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f,fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
//	Render_HitBoxObb();
//	Render_ObbLineD3D();
}

void CLaser::Release_GameObject()
{
}
