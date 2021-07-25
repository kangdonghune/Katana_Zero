#include "stdafx.h"
#include "ButcherKnife.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"

CButcherKnife::CButcherKnife()
{
}


CButcherKnife::~CButcherKnife()
{
}

CGameObject * CButcherKnife::Create(ITEMINFO * pInfo)
{
	CButcherKnife* pButcherKnife = new CButcherKnife();
	pButcherKnife->Set_Info(pInfo);
 	if (FAILED(pButcherKnife->Ready_GameObject()))
	{
		Safe_Delete(pButcherKnife);
		return pButcherKnife;
	}
	return pButcherKnife;;
}

CGameObject * CButcherKnife::Create(CGameObject * pTarget)
{
	CButcherKnife* pButcherKnife = new CButcherKnife();
	pButcherKnife->Set_Target(pTarget);
	pButcherKnife->m_pItemInfo = new ITEMINFO{};
	if (FAILED(pButcherKnife->Ready_GameObject()))
	{
		Safe_Delete(pButcherKnife);
		return pButcherKnife;
	}
	return pButcherKnife;;
}

HRESULT CButcherKnife::Ready_GameObject()
{
	if (m_pTarget != nullptr)
	{
		m_pItemInfo->wstrKey = L"Projectile";
		m_pItemInfo->wstrState = L"Butcherknife";
		m_pItemInfo->type = ITEMTYPE::BUTCHERKNIFE;
		m_pItemInfo->D3VecPos.x = m_pTarget->Get_UnitInfo()->D3VecPos.x;
		m_pItemInfo->D3VecPos.y = m_pTarget->Get_UnitInfo()->D3VecPos.y;
		m_pItemInfo->D3VecPos.z = 0.f;
		m_fRatio = 0.8f;
		m_fRotateAngle = 0.f;
		m_fRotateSpeed = 40.f;
		m_fTargetAngle = m_pTarget->Get_TargetAngle();
		m_tFrame.fFrameStart = 0.f;
		m_iUnitDir = m_pTarget->Get_UnitDir();
		m_iObjState = THROW;
		m_fUnitSpeed = 30.f;

		return S_OK;
	}

	m_fRatio = 0.8f;
	m_fSpeed = 2.5f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pItemInfo->wstrKey, m_pItemInfo->wstrState);
	return S_OK;

}

void CButcherKnife::Update_GameObject()
{
	if (m_pTarget == nullptr)
	{
		FrameMove(m_fSpeed);
		if (m_iObjState != COLLIDE)
			m_pItemInfo->D3VecPos.y += 3.f;
	}
	if (m_pTarget)
	{
		if (m_iObjState != COLLIDE)
		{
			m_fRotateAngle += m_fRotateSpeed;
			m_pItemInfo->D3VecPos.x += cosf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
			m_pItemInfo->D3VecPos.y -= sinf(D3DXToRadian(m_fTargetAngle))*m_fUnitSpeed;
		}
	}
	Update_ProjectileHitBoxOBB();

}

void CButcherKnife::LateUpdate_GameObject()
{
}

void CButcherKnife::Render_GameObject()
{
	if (nullptr == m_pItemInfo)
	{
		ERR_MSG(L"item 정보가 없습니다.");
		return;
	}
	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;
	if (m_iObjState == THROW)
		int a = 1;
	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pItemInfo->wstrKey, m_pItemInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Knife에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pItemInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pItemInfo->D3VecPos.y - CScrollManager::Get_ScroolY(), 0.f);
 	matWorld = matScale * matRolateZ *matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//Render_HitBoxObb();
	//Render_ObbLineD3D();
}

void CButcherKnife::Release_GameObject()
{
	if (m_pItemInfo != nullptr)
	{
		Safe_Delete(m_pItemInfo);
	}
}
