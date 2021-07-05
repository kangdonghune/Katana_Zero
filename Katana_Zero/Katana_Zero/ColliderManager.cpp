#include "stdafx.h"
#include "ColliderManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"

IMPLEMENT_SINGLETON(CColliderManager)
CColliderManager::CColliderManager()
{
}


CColliderManager::~CColliderManager()
{
}

void CColliderManager::Collider_Land(vector<MYLINE> pLandvec, CGameObject * pUnit)
{

	MYLINE* pLand = nullptr;
	MYLINE** ppLand = &pLand;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height>>1);
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Width>>1);
	for (auto& tLine : pLandvec)
	{
		if (pLand != nullptr)
			break;
		if (tLine.Start.x <= Pivot.x + iWidth  && tLine.End.x >= Pivot.x - iWidth)
		{
			if (tLine.Start.y == tLine.End.y) // 평면 직선이면
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 0 && lDistance >= -10)
				{
					pUnit->Set_PivotY(tLine.Start.y);
					*ppLand = &tLine;
				}

			}
			else // 아니면
			{ 
		
			}
		}
	}

	if (pLand != nullptr)
		pUnit->Set_Info()->iCollide = C_LAND;
	if (pLand == nullptr)
		pUnit->Set_Info()->iCollide = C_NONE;
	
}

