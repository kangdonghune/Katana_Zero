#include "stdafx.h"
#include "Enemy.h"
#include "Texture_Manager.h"


CEnemy::CEnemy()
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Find_TargetAngle()
{
	m_fTargetAngle = (m_pTarget->Get_Pivot().y - m_pTarget->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pTarget->Get_UnitInfo()->wstrKey, m_pTarget->Get_UnitInfo()->wstrState, 0)->tImageInfo.Height >> 1) - m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Height >> 1)) / (m_pTarget->Get_Pivot().x - m_vecPivot.x);
}

HRESULT CEnemy::Ready_GameObject()
{
	return E_NOTIMPL;
}

void CEnemy::Update_GameObject()
{
}

void CEnemy::LateUpdate_GameObject()
{
}

void CEnemy::Render_GameObject()
{
}

void CEnemy::Release_GameObject()
{
}
