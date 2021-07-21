#include "stdafx.h"
#include "PlayerAttack.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"

CPlayerAttack::CPlayerAttack()
{
}


CPlayerAttack::~CPlayerAttack()
{
}

CGameObject * CPlayerAttack::Create(CGameObject* pTarget)
{
	CPlayerAttack* pPlayerAttack = new CPlayerAttack();
	pPlayerAttack->Set_Target(pTarget);
	pPlayerAttack->m_pUnitInfo = new UNITINFO{};
	if (FAILED(pPlayerAttack->Ready_GameObject()))
	{
		Safe_Delete(pPlayerAttack);
		return pPlayerAttack;
	}
	return pPlayerAttack;;
}

HRESULT CPlayerAttack::Ready_GameObject()
{
	m_pUnitInfo->wstrKey = L"Effect";
	m_pUnitInfo->wstrState = L"PlayerAttack";
	m_pUnitInfo->type = UNITTYPE::TYPENONE;
	m_pUnitInfo->D3VecPos.x = m_pTarget->Get_Pivot().x;
	m_pUnitInfo->D3VecPos.y = (m_pTarget->Get_Hitbox().bottom - m_pTarget->Get_Hitbox().top) / 2.f;
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 20.f;
	m_fRatio = 1.5f;
	m_fRotateAngle = 0.f;
	m_fTargetAngle = m_pTarget->Get_TargetAngle();
	m_iUnitDir = m_pTarget->Get_UnitDir();
	m_fUnitSpeed = 20.f;
	return S_OK;
}

void CPlayerAttack::Update_GameObject()
{
	m_pUnitInfo->D3VecPos.x = m_pTarget->Get_Pivot().x;
	m_pUnitInfo->D3VecPos.y = m_pTarget->Get_Pivot().y - (m_pTarget->Get_Hitbox().bottom - m_pTarget->Get_Hitbox().top) / 2.f;
	m_fTargetAngle = m_pTarget->Get_TargetAngle();
	m_fRotateAngle = m_pTarget->Get_RotateAngle();
	m_iUnitDir = m_pTarget->Get_UnitDir();
	Update_HitBoxOBB();
	FrameMove(m_fSpeed);
	if (m_pTarget->Get_UnitInfo()->wstrState != L"Attack")
		m_iObjState = DEAD;
	if (Check_FrameEnd())
		m_iObjState = DEAD;
}

void CPlayerAttack::LateUpdate_GameObject()
{
}

void CPlayerAttack::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"PlayerAttack 정보가 없습니다.");
		return;
	}

	if (m_pTarget->Get_UnitInfo()->wstrState != L"Attack")
		return;

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"PlayerAttack에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY(), 0.f);
	matWorld = matScale * matRolateZ *matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	Render_HitBoxObb();
	Render_ObbLineD3D();
}

void CPlayerAttack::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}
