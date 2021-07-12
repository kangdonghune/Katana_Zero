#include "stdafx.h"
#include "ColliderManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "Effect.h"
#include "GameObjectManager.h"

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
	int	iCollide = pInfo->iCollide;
	const PLAYERSTATE::State State = pUnit->Get_State();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height>>1);
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Width>>1);
	for (auto& tLine : pLandvec)
	{
		if (tLine.Start.x <= Pivot.x && tLine.End.x >= Pivot.x)
		{	
			if (pLand != nullptr)
				break;
			if (tLine.Start.y == tLine.End.y)
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 0 && lDistance >= -1 * iHeight)
				{
					pUnit->Set_PivotY(tLine.Start.y);
					*ppLand = &tLine;
				}
			}

			else
			{
				float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
				if (tLine.Start.y > tLine.End.y)
					fLineHeight = (tLine.Start.y - tLine.End.y) / (tLine.Start.x - tLine.End.x)*(Pivot.x - tLine.Start.x) + tLine.End.y;
				LONG lDistance = fLineHeight - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 10 && lDistance >= -iHeight)
				{
					if (State != PLAYERSTATE::JUMP && State != PLAYERSTATE::ATTACK)
					{
						pUnit->Set_PivotY(fLineHeight);
						*ppLand = &tLine;
						continue;
					}
					if (State == PLAYERSTATE::ATTACK)
					{
						if (Pivot.y >= fLineHeight)
						{
							pUnit->Set_PivotY(fLineHeight);
							*ppLand = &tLine;
							continue;
						}
					}

				}
			}
		}
	}

	if (pLand != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_LAND; //충돌 추가
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //충돌 제거
	}

	if (pLand == nullptr)
	{
		if(iCollide & C_LAND)
			pUnit->Set_Info()->iCollide ^= C_LAND; //충돌 제거
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING))
			return;

		pUnit->Set_Info()->iCollide |= C_NONE; //충돌 제거
	}
		
	
}

void CColliderManager::Collider_Wall(vector<MYLINE> pWallvec, CGameObject * pUnit)
{
	int dir = 0;
	MYLINE* pWall = nullptr;
	MYLINE** ppWall = &pWall;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	const RECT	tHitBox = pUnit->Get_Hitbox();
	int	iCollide = pInfo->iCollide;
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Wallslide", 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pWallvec)
	{
		if (pWall != nullptr)
			break;
		if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top) //캐릭터가 벽 라인 높이에 일부라도 걸쳐 있을 때
		{
			LONG lDistanceL = tLine.Start.x - (Pivot.x - iWidth);
			LONG lDistanceR = (Pivot.x + iWidth) - tLine.Start.x;
			if (lDistanceL >= 0 && lDistanceL <= iWidth) //왼쪽벽에 닿은 경우
			{
				pUnit->Set_PivotX(Pivot.x + lDistanceL);
				*ppWall = &tLine;
				dir = C_WALLL;
				continue;
			}
			if (lDistanceR >= 0 && lDistanceR <= iWidth) //오른쪽 벽
			{
				pUnit->Set_PivotX(Pivot.x - lDistanceR);
				*ppWall = &tLine;
				dir = C_WALLR;
				continue;
			}
		}
	}

	if (pWall != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_WALL| dir;
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //충돌 제거
	}

	if (pWall == nullptr)
	{
		if (iCollide & C_WALL)
			pUnit->Set_Info()->iCollide ^= C_WALL|C_WALLL|C_WALLR;
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING)) //세 개 상태 중 하나라도 겹치면
			return;

		pUnit->Set_Info()->iCollide |= C_NONE;
	}
	
}

void CColliderManager::Collider_Celling(vector<MYLINE> pCellingVec, CGameObject * pUnit)
{
	MYLINE* pCelling = nullptr;
	MYLINE** ppCelling = &pCelling;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	const PLAYERSTATE::State State = pUnit->Get_State();
	const RECT	tHitBox = pUnit->Get_Hitbox();
	int	iCollide = pInfo->iCollide;
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Jump", 0)->tImageInfo.Height >> 1);
	for (auto& tLine : pCellingVec)
	{
		if (pCelling != nullptr)
			break;
		if (tLine.Start.x <= Pivot.x && tLine.End.x >= Pivot.x)
		{
			if (tLine.Start.y == tLine.End.y) // 평면 직선이면
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().top;
				if (lDistance >= 0 && lDistance <= iHeight)
				{
					pUnit->Set_PivotY(Pivot.y + lDistance);
					*ppCelling = &tLine;
				}

			}
			else // 아니면
			{
				float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
				LONG lDistance = fLineHeight - pUnit->Get_Hitbox().top;
				if (lDistance >= 0 && lDistance <= iHeight)
				{
					if (State != PLAYERSTATE::FALL)
						pUnit->Set_PivotY(Pivot.y + lDistance);
					*ppCelling = &tLine;
				}
			}
		}
	}

	if (pCelling != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_CELLING;
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //충돌 제거
	}

	if (pCelling == nullptr)
	{
		if (iCollide & C_CELLING)
			pUnit->Set_Info()->iCollide ^= C_CELLING;
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALL | C_CELLING))
			return;

		pUnit->Set_Info()->iCollide |= C_NONE; //충돌 제거
	}
}

void CColliderManager::Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst)
{
	MYLINE* pLand = nullptr;
	MYLINE** ppLand = &pLand;
	for (auto& iter = pUnitlst.begin(); iter != pUnitlst.end(); iter++)
	{
		const UNITINFO* pInfo = (*iter)->Get_UnitInfo();
		int	iCollide = pInfo->iCollide;
		const PLAYERSTATE::State State = (*iter)->Get_State();
		const D3DXVECTOR3 Pivot = (*iter)->Get_Pivot();
		int iHeight = (*iter)->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height >> 1);
		int iWidth = (*iter)->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Width >> 1);
		for (auto& tLine : pLandvec)
		{
			if (pLand != nullptr)
				break;
			if (tLine.Start.x <= Pivot.x && tLine.End.x >= Pivot.x)
			{
				if (tLine.Start.y == tLine.End.y) // 평면 직선이면
				{
					LONG lDistance = tLine.Start.y - (*iter)->Get_Hitbox().bottom;
					if (lDistance <= 0 && lDistance >= -1 * iHeight)
					{
						(*iter)->Set_PivotY(tLine.Start.y);
						*ppLand = &tLine;
					}

				}
				else // 아니면
				{
					float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
					LONG lDistance = fLineHeight - (*iter)->Get_Hitbox().bottom;
					if (lDistance <= 10 && lDistance >= -iHeight)
					{
						if (State != PLAYERSTATE::JUMP && State != PLAYERSTATE::ATTACK)
						{
							(*iter)->Set_PivotY(fLineHeight);
							*ppLand = &tLine;
							continue;
						}
						if (State == PLAYERSTATE::ATTACK)
						{
							if (Pivot.y >= fLineHeight)
							{
								(*iter)->Set_PivotY(fLineHeight);
								*ppLand = &tLine;
								continue;
							}
						}

					}
				}
			}
		}

		if (pLand != nullptr)
		{
			(*iter)->Set_Info()->iCollide |= C_LAND; //충돌 추가
			if ((*iter)->Get_UnitInfo()->iCollide  & C_NONE)
				(*iter)->Set_Info()->iCollide ^= C_NONE; //충돌 제거
		}

		if (pLand == nullptr)
		{
			if (iCollide & C_LAND)
				(*iter)->Set_Info()->iCollide ^= C_LAND; //충돌 제거
			if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING))
				return;

			(*iter)->Set_Info()->iCollide |= C_NONE; //충돌 제거
		}
	}
	
}

void CColliderManager::Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst)
{
	int dir = 0;
	MYLINE* pWall = nullptr;
	MYLINE** ppWall = &pWall;
	for (auto& iter = pUnitlst.begin(); iter != pUnitlst.end(); iter++)
	{
		const UNITINFO* pInfo = (*iter)->Get_UnitInfo();
		const D3DXVECTOR3 Pivot = (*iter)->Get_Pivot();
		const RECT	tHitBox = (*iter)->Get_Hitbox();
		int	iCollide = pInfo->iCollide;
		int iWidth = (*iter)->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Width >> 1);
		for (auto& tLine : pWallvec)
		{
			if (pWall != nullptr)
				break;
			if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top) //캐릭터가 벽 라인 높이에 일부라도 걸쳐 있을 때
			{
				LONG lDistanceL = tLine.Start.x - (Pivot.x - iWidth);
				LONG lDistanceR = (Pivot.x + iWidth) - tLine.Start.x;
				if (lDistanceL >= 0 && lDistanceL <= iWidth) //왼쪽벽에 닿은 경우
				{
					(*iter)->Set_PivotX(Pivot.x + lDistanceL);
					*ppWall = &tLine;
					dir = C_WALLL;
					continue;
				}
				if (lDistanceR >= 0 && lDistanceR <= iWidth) //오른쪽 벽
				{
					(*iter)->Set_PivotX(Pivot.x - lDistanceR);
					*ppWall = &tLine;
					dir = C_WALLR;
					continue;
				}
			}

			if (pWall != nullptr)
			{
				(*iter)->Set_Info()->iCollide |= C_WALL | dir;
				if ((*iter)->Get_UnitInfo()->iCollide  & C_NONE)
					(*iter)->Set_Info()->iCollide ^= C_NONE; //충돌 제거
			}

			if (pWall == nullptr)
			{
				if (iCollide & C_WALL)
					(*iter)->Set_Info()->iCollide ^= C_WALL | C_WALLL | C_WALLR;
				if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING)) //세 개 상태 중 하나라도 겹치면
					return;

				(*iter)->Set_Info()->iCollide |= C_NONE;
			}

		}
	}
}
void CColliderManager::Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst)
{
	MYLINE* pCelling = nullptr;
	MYLINE** ppCelling = &pCelling;
	for (auto& iter = pUnitlst.begin(); iter != pUnitlst.end(); iter++)
	{
		const UNITINFO* pInfo = (*iter)->Get_UnitInfo();
		const D3DXVECTOR3 Pivot = (*iter)->Get_Pivot();
		const PLAYERSTATE::State State = (*iter)->Get_State();
		const RECT	tHitBox = (*iter)->Get_Hitbox();
		int	iCollide = pInfo->iCollide;
		int iHeight = (*iter)->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Height >> 1);
		for (auto& tLine : pCellingVec)
		{
			if (pCelling != nullptr)
				break;
			if (tLine.Start.x <= Pivot.x && tLine.End.x >= Pivot.x)
			{
				if (tLine.Start.y == tLine.End.y) // 평면 직선이면
				{
					LONG lDistance = tLine.Start.y - (*iter)->Get_Hitbox().top;
					if (lDistance >= 0 && lDistance <= iHeight)
					{
						(*iter)->Set_PivotY(Pivot.y + lDistance);
						*ppCelling = &tLine;
					}

				}
				else // 아니면
				{
					float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
					LONG lDistance = fLineHeight - (*iter)->Get_Hitbox().top;
					if (lDistance >= 0 && lDistance <= iHeight)
					{
						if (State != PLAYERSTATE::FALL)
							(*iter)->Set_PivotY(Pivot.y + lDistance);
						*ppCelling = &tLine;
					}
				}
			}
		}

		if (pCelling != nullptr)
		{
			(*iter)->Set_Info()->iCollide |= C_CELLING;
			if ((*iter)->Get_UnitInfo()->iCollide  & C_NONE)
				(*iter)->Set_Info()->iCollide ^= C_NONE; //충돌 제거
		}

		if (pCelling == nullptr)
		{
			if (iCollide & C_CELLING)
				(*iter)->Set_Info()->iCollide ^= C_CELLING;
			if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALL | C_CELLING))
				return;

			(*iter)->Set_Info()->iCollide |= C_NONE; //충돌 제거
		}
	}
	
}

void CColliderManager::Collider_Obb(CGameObject* pObject1, CGameObject* pObject2)
{
	if (pObject2->Get_ObjState() == COLLIDE)
		return;
	//두 대상의 거리 벡터
	D3DXVECTOR3 Dist = pObject1->Get_UnitInfo()->D3VecPos - pObject2->Get_UnitInfo()->D3VecPos;
	//obj1의 라이트 투영
	D3DXVECTOR3 R1 = {pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width>>1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle())), -1* pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle())), 0.f };
	//obj1의 up 투영
	D3DXVECTOR3 U1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), -1* pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), 0.f };
	//obj2의 right 투영
	D3DXVECTOR3 R2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle())),  -1* pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle())), 0.f };
	//obj2의 up투영
	D3DXVECTOR3 U2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), -1* pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), 0.f };
	
	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//투영할 방향벡터
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i번째 선분을 정규화, 크기가 1인 방향벡터로
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}
	pObject2->Set_ObjState(COLLIDE);//충돌 상태로 변환
	GameObjectManager->Insert_GameObjectManager(CEffect::Create(pObject2,L"Bulletreflect"), GAMEOBJECT::EFFECT);
	pObject2->Set_TargetAngle(180.f);//날아가던 방향을 반대로
}

void CColliderManager::Collider_Obb(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2)
{
	if (pObjectlist1.empty() || pObjectlist2.empty())
		return;
	for (auto& pObject1 : pObjectlist1)
	{
		for (auto& pObject2 : pObjectlist2)
		{
			Collider_Obb(pObject1, pObject2);
		}
	}
}

