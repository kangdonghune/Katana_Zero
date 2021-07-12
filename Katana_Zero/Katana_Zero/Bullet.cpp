#include "stdafx.h"
#include "Bullet.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "Gangster.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

CGameObject * CBullet::Create(CGameObject * pShooter)
{
	CBullet* pBullet = new CBullet();
	pBullet->Set_Shooter(pShooter);
	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Delete(pBullet);
		return pBullet;
	}
	return pBullet;
}

HRESULT CBullet::Ready_GameObject()
{
	m_pUnitInfo = new UNITINFO{};
	m_pUnitInfo->wstrKey = L"Projectile";
	m_pUnitInfo->wstrState = L"Bullet";
	m_fRatio = 1.f;
	m_pUnitInfo->D3VecPos.x = m_pShooter->Get_Pivot().x + m_pShooter->Get_UnitDir()*m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Width >> 1)+ m_pShooter->Get_UnitDir()*m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Width >> 1);
	m_pUnitInfo->D3VecPos.y = m_pShooter->Get_Pivot().y - m_pShooter->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pShooter->Get_UnitInfo()->wstrKey, m_pShooter->Get_UnitInfo()->wstrState, 0)->tImageInfo.Height >> 1);
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 20.f;
	
	m_fTargetAngle = m_pShooter->Get_TargetAngle();
	m_fRotateAngle = m_pShooter->Get_RotateAngle();
 	m_iUnitDir = m_pShooter->Get_UnitDir();

	return S_OK;
}

void CBullet::Update_GameObject()
{

	m_pUnitInfo->D3VecPos.x += cosf(D3DXToRadian(m_fTargetAngle))*15.f;
	m_pUnitInfo->D3VecPos.y -= sinf(D3DXToRadian(m_fTargetAngle))*15.f;
	Update_HitBoxOBB();
	//방향만 이동.

}

void CBullet::LateUpdate_GameObject()

{
}

void CBullet::Render_GameObject()
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
	D3DXMatrixScaling(&matScale, m_fRatio, m_fRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
	matWorld = matScale * matRolateZ *matTrans;
	
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	Render_HitBoxObb();
	Render_ObbLineD3D();
}

void CBullet::Release_GameObject()
{
}
