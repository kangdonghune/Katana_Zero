#include "stdafx.h"
#include "HitEffect.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"
#include "FrameManager.h"

CHitEffect::CHitEffect()
{
}


CHitEffect::~CHitEffect()
{
	Release_GameObject();
}

CGameObject * CHitEffect::Create(CGameObject * pTarget)
{
	CHitEffect* pEffect = new CHitEffect();
	pEffect->Set_Target(pTarget);
	pEffect->m_pUnitInfo = new UNITINFO{};
	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Delete(pEffect);
		return pEffect;
	}
	return pEffect;
}

HRESULT CHitEffect::Ready_GameObject()
{
	m_pUnitInfo->wstrKey = L"Effect";
	m_pUnitInfo->wstrState = L"Slash";
	m_pUnitInfo->type = UNITTYPE::EFFECT;
	m_pUnitInfo->D3VecPos.x = m_pTarget->Get_UnitInfo()->D3VecPos.x;
	m_pUnitInfo->D3VecPos.y = m_pTarget->Get_UnitInfo()->D3VecPos.y;
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 20.f;
	m_fRatio = 3.f;
	m_fRotateAngle = 0.f;
	m_fTargetAngle = m_pTarget->Get_TargetAngle();
	m_iUnitDir = m_pTarget->Get_UnitDir();
	m_fTargetAngle = m_pTarget->Get_TargetAngle();
	m_fRotateAngle = 360 - m_fTargetAngle;
	return S_OK;
}

void CHitEffect::Update_GameObject()
{


	m_iUnitDir = m_pTarget->Get_UnitDir();
	FrameMove(m_fSpeed);
	Update_HitBoxOBB();
	if (Check_FrameEnd())
	{
		m_iObjState = DEAD;
	}

}

void CHitEffect::LateUpdate_GameObject()
{
}

void CHitEffect::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"Effect ������ �����ϴ�.");
		return;
	}
	if (m_iObjState == DEAD)
		return;
	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Effect���� �ؽ��� ã�� ����");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_fRatio, m_fRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fTargetAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY(), 0.f);
	matWorld = matScale *matRolateZ* matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//Render_HitBoxObb();
	Render_ObbLineD3D();
}

void CHitEffect::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}
