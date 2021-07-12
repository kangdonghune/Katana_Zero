#include "stdafx.h"
#include "Effect.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"

CEffect::CEffect()
{
}


CEffect::~CEffect()
{
}

CGameObject * CEffect::Create(CGameObject * pTarget, TCHAR* wstrStateKey)
{
	CEffect* pEffect = new CEffect();
	pEffect->Set_Target(pTarget);
	pEffect->m_pUnitInfo = new UNITINFO{};
	pEffect->m_pUnitInfo->wstrState = wstrStateKey;
	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Delete(pEffect);
		return pEffect;
	}
	return pEffect;
}

HRESULT CEffect::Ready_GameObject()
{
	m_pUnitInfo->wstrKey = L"Effect";
	m_pUnitInfo->type = UNITTYPE::EFFECT;
	m_pUnitInfo->D3VecPos.x = m_pTarget->Get_UnitInfo()->D3VecPos.x;
	m_pUnitInfo->D3VecPos.y = m_pTarget->Get_UnitInfo()->D3VecPos.y;
	m_pUnitInfo->D3VecPos.z = 0.f;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 40.f;
	m_fRatio = 1.5f;
	m_fRotateAngle = 0.f;
	m_fTargetAngle = m_pTarget->Get_TargetAngle();
	m_iUnitDir = m_pTarget->Get_UnitDir();
	return S_OK;
}

void CEffect::Update_GameObject()
{
	m_fTargetAngle = m_pTarget->Get_TargetAngle(); 
	m_fRotateAngle = m_pTarget->Get_RotateAngle();
	m_iUnitDir = m_pTarget->Get_UnitDir();
	Update_HitBoxOBB();
	if (Check_FrameEnd())
		m_iObjState = DEAD;
}

void CEffect::LateUpdate_GameObject()
{
}

void CEffect::Render_GameObject()
{

	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"Effect 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Effect에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	//D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
	matWorld = matScale * matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	Render_HitBoxObb();
	Render_ObbLineD3D();
}

void CEffect::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}
