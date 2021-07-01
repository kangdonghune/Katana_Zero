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
	for (auto& tLine : pLandvec)
	{
		if (tLine.Start.x <= pInfo->D3VecPos.x && tLine.End.x >= pInfo->D3VecPos.x)
		{
			if (tLine.Start.y == tLine.End.y) // 평면 직선이면
			{
				float fdistance = tLine.Start.y - (pUnit->Get_UnitRatio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height >> 1)) - 5;
				if (fdistance <= pInfo->D3VecPos.y)
					*ppLand = &tLine;
			}
			else // 아니면
			{ 
		
			}
		}
	}

	if (pLand != nullptr)
	{
		ERR_MSG(L"땅과 충돌");
	}
}

