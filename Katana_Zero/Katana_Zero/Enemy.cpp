#include "stdafx.h"
#include "Enemy.h"
#include "Texture_Manager.h"
#include "ScrollManager.h"
#include "ColliderManager.h"
#include "MapObjectManager.h"
CEnemy::CEnemy()
{
}


CEnemy::~CEnemy()
{
}



void CEnemy::Render_Raytraise()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		D3DXVECTOR2	vLine2[2]{ { m_pTarget->Get_UnitInfo()->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pTarget->Get_UnitInfo()->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, r, g, b));
	}
}

bool CEnemy::Ray_Traising()
{
	for (int i = 0; i < TERRAINTYPE::END; i++)
	{

		r = 255;
		g = 0;
		b = 0;
		if (ColliderManager->Collide_TerrainAndRay(MapObjectManager->Get_TerrainVector(i), this))
		{
			r = 0;
			g = 255;
			b = 0;
			return false;
		}
	}
	return true;
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
	Render_Raytraise();
}

void CEnemy::Release_GameObject()
{
}
