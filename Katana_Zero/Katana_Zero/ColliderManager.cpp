#include "stdafx.h"
#include "ColliderManager.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "Effect.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "Player.h"
#include "HitEffect.h"

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
		if (tLine.Start.x < Pivot.x && tLine.End.x > Pivot.x)
		{	
			if (pLand != nullptr)
				break;
			if (tLine.Start.y == tLine.End.y)
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 0 && lDistance >= -1 * iHeight)
				{
				
					pUnit->Set_PivotY(tLine.Start.y);
					pUnit->Set_CurLine(tLine);
					*ppLand = &tLine;
				}
			}

			else
			{
				float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
				LONG lDistance = fLineHeight - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 10 && lDistance >= -iHeight)
				{
					if (State != PLAYERSTATE::JUMP && State != PLAYERSTATE::ATTACK)
					{
						pUnit->Set_PivotY(fLineHeight);
						pUnit->Set_CurLine(tLine);
						*ppLand = &tLine;
						continue;
					}
					if (State == PLAYERSTATE::ATTACK)
					{
						if (Pivot.y >= fLineHeight)
						{
							pUnit->Set_PivotY(fLineHeight);
							pUnit->Set_CurLine(tLine);
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
		pUnit->Set_Info()->iCollide |= C_LAND; //�浹 �߰�
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pLand == nullptr)
	{
		if(iCollide & C_LAND)
			pUnit->Set_Info()->iCollide ^= C_LAND; //�浹 ����
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING | C_PASSABLE))
			return;

		if (pUnit->Set_Info()->iCollide != C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
	}
		
	
}

void CColliderManager::Collider_LandAndEnemy(vector<MYLINE> pLandvec, CGameObject * pUnit)
{
	MYLINE* pLand = nullptr;
	MYLINE** ppLand = &pLand;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	int	iCollide = pInfo->iCollide;
	const PLAYERSTATE::State State = pUnit->Get_State();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height >> 1);
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pLandvec)
	{

		if (tLine.Start.x < Pivot.x && tLine.End.x > Pivot.x)
		{
			if (pLand != nullptr)
				break;
			if (tLine.Start.y == tLine.End.y)
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 10 && lDistance >= -1 * iHeight)
				{
					pUnit->Set_PivotY(tLine.Start.y);
					pUnit->Set_CurLine(tLine);
					*ppLand = &tLine;
				}
			}

			else
			{
				float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
				LONG lDistance = fLineHeight - pUnit->Get_Hitbox().bottom;
				if (lDistance <= 10 && lDistance >= -iHeight)
				{
					pUnit->Set_PivotY(fLineHeight);
					pUnit->Set_CurLine(tLine);
					*ppLand = &tLine;
					continue;
				}
			}
		}
	}

	if (pLand != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_LAND; //�浹 �߰�
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pLand == nullptr)
	{
		if (iCollide & C_LAND)
			pUnit->Set_Info()->iCollide ^= C_LAND; //�浹 ����
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING | C_PASSABLE))
			return;

		if (pUnit->Set_Info()->iCollide != C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
	}
}

void CColliderManager::Collider_PassAble(vector<MYLINE> pLandvec, CGameObject * pUnit)
{
	MYLINE* pLand = nullptr;
	MYLINE** ppLand = &pLand;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	int	iCollide = pInfo->iCollide;
	const PLAYERSTATE::State State = pUnit->Get_State();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height >> 1);
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pLandvec)
	{
		if (GetAsyncKeyState('S') & 0x8000)
			break;
		if (pUnit->Get_State() == PLAYERSTATE::JUMP)
			break;
		if (pUnit->Get_State() == PLAYERSTATE::ATTACK)
			break;
		if (tLine.Start.x < Pivot.x && tLine.End.x > Pivot.x)
		{
			if (pLand != nullptr)
				break;
			if (pInfo->D3VecPos.y > tLine.Start.y)
				continue;
			LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
			if (lDistance <= 10 && lDistance >= -10)
			{
				pUnit->Set_PivotY(tLine.Start.y);
				pUnit->Set_CurLine(tLine);
				*ppLand = &tLine;
			}
		}
	}

	if (pLand != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_PASSABLE; //�浹 �߰�
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pLand == nullptr)
	{
		if (iCollide & C_PASSABLE)
			pUnit->Set_Info()->iCollide ^= C_PASSABLE; //�浹 ����
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING| C_PASSABLE))
			return;
		if (pUnit->Set_Info()->iCollide != C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
	}


}

void CColliderManager::Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, CGameObject * pUnit)
{
	MYLINE* pLand = nullptr;
	MYLINE** ppLand = &pLand;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	int	iCollide = pInfo->iCollide;
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Height >> 1);
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, pInfo->wstrState, 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pLandvec)
	{
		if (pUnit->Get_NextLine().type == TERRAINTYPE::LAND && pUnit->Get_NowLine().type == TERRAINTYPE::LAND &&((fabs(pUnit->Get_NextLine().Start.x - Pivot.x) < 5 || fabs(pUnit->Get_NextLine().End.x - Pivot.x))))
			break;

		if (tLine.Start.x < Pivot.x && tLine.End.x > Pivot.x)
		{
			if (pLand != nullptr)
				break;
			if (pInfo->D3VecPos.y > tLine.Start.y)
				continue;
			LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().bottom;
			if (lDistance <= 10 && lDistance >= -10)
			{
				pUnit->Set_PivotY(tLine.Start.y);
				pUnit->Set_CurLine(tLine);
				*ppLand = &tLine;
			}
		}
	}

	if (pLand != nullptr)
	{
		pUnit->Set_Info()->iCollide |= C_PASSABLE; //�浹 �߰�
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pLand == nullptr)
	{
		if (iCollide & C_PASSABLE)
			pUnit->Set_Info()->iCollide ^= C_PASSABLE; //�浹 ����
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING | C_PASSABLE))
			return;
		if (pUnit->Set_Info()->iCollide != C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
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
	float fHeight = tHitBox.bottom - tHitBox.top;
	int	iCollide = pInfo->iCollide;
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(L"Player", L"Wallslide", 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pWallvec)
	{
		if (pWall != nullptr)
			break;
		if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top+fHeight*3/4) //ĳ���Ͱ� �� ���� ���̿� �Ϻζ� ���� ���� ��
		{
			LONG lDistanceL = tLine.Start.x - (Pivot.x - iWidth);
			LONG lDistanceR = (Pivot.x + iWidth) - tLine.Start.x;
			if (lDistanceL >= 0 && lDistanceL <= iWidth) //���ʺ��� ���� ���
			{
				pUnit->Set_PivotX(Pivot.x + lDistanceL);
				*ppWall = &tLine;
				dir = C_WALLL;
				continue;
			}
			if (lDistanceR >= 0 && lDistanceR <= iWidth) //������ ��
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
		pUnit->Set_Info()->iCollide |= C_WALL | dir;
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pWall == nullptr)
	{
		if (iCollide & C_WALL)
			pUnit->Set_Info()->iCollide ^= C_WALL;
		if (iCollide & C_WALLL)
			pUnit->Set_Info()->iCollide ^= C_WALLL;
		if (iCollide & C_WALLR)
			pUnit->Set_Info()->iCollide ^= C_WALLR;
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND |C_CELLING | C_PASSABLE))
			return;

		if (pUnit->Set_Info()->iCollide != C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
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
	int iHeight = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(L"Player", L"Jump", 0)->tImageInfo.Height >> 1);
	for (auto& tLine : pCellingVec)
	{
		if (pCelling != nullptr)
			break;
		if (tLine.Start.x <= Pivot.x && tLine.End.x >= Pivot.x)
		{
			if (tLine.Start.y == tLine.End.y) // ��� �����̸�
			{
				LONG lDistance = tLine.Start.y - pUnit->Get_Hitbox().top;
				if (lDistance >= 0 && lDistance <= iHeight)
				{
					pUnit->Set_PivotY(Pivot.y + lDistance);
					*ppCelling = &tLine;
				}

			}
			else // �ƴϸ�
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
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pCelling == nullptr)
	{
		if (iCollide & C_CELLING)
			pUnit->Set_Info()->iCollide ^= C_CELLING;
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING | C_PASSABLE))
			return;

		if (!pUnit->Set_Info()->iCollide & C_NONE)
			pUnit->Set_Info()->iCollide = C_NONE;
	}
}

void CColliderManager::Collider_StageChange(vector<MYLINE> pChangeVec, CGameObject * pUnit)
{
	int dir = 0;
	MYLINE* pChange = nullptr;
	MYLINE** ppChange = &pChange;
	const UNITINFO* pInfo = pUnit->Get_UnitInfo();
	const D3DXVECTOR3 Pivot = pUnit->Get_Pivot();
	const RECT	tHitBox = pUnit->Get_Hitbox();
	int	iCollide = pInfo->iCollide;
	int iWidth = pUnit->Get_Ratio() * (Texture_Maneger->Get_TexInfo_Manager(pInfo->wstrKey, L"Idle", 0)->tImageInfo.Width >> 1);
	for (auto& tLine : pChangeVec)
	{
		if (pChange != nullptr)
			break;
		if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top) //ĳ���Ͱ� �� ���� ���̿� �Ϻζ� ���� ���� ��
		{
			LONG lDistanceL = tLine.Start.x - (Pivot.x - iWidth);
			LONG lDistanceR = (Pivot.x + iWidth) - tLine.Start.x;
			if (lDistanceL >= 0 && lDistanceL <= iWidth) //���ʺ��� ���� ���
			{
				*ppChange = &tLine;
				continue;
			}
			if (lDistanceR >= 0 && lDistanceR <= iWidth) //������ ��
			{
				*ppChange = &tLine;
				continue;
			}
		}
	}

	if (pChange != nullptr)
	{
		SceneManager->Change_SceneManager(SceneManager->Get_CurScene() + 1);
	}

	if (pChange == nullptr)
	{
		return;
	}
}

void CColliderManager::Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst)
{
	for (auto& pUnit : pUnitlst)
	{
		Collider_LandAndEnemy(pLandvec, pUnit);
	}
	
}

void CColliderManager::Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst)
{
	for (auto& pUnit : pUnitlst)
	{
		Collider_PassAbleAndEnemy(pLandvec, pUnit);
	}
}

void CColliderManager::Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst)
{
	for (auto& pUnit : pUnitlst)
	{
		Collider_Wall(pWallvec, pUnit);
	}
}
void CColliderManager::Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst)
{
	for (auto& pUnit : pUnitlst)
	{
		Collider_Celling(pCellingVec, pUnit);
	}
}

void CColliderManager::Collider_Obb(CGameObject* pObject1, CGameObject* pObject2)
{
	if (pObject2->Get_ObjState() == COLLIDE)
		return;
	//�� ����� �Ÿ� ����
	D3DXVECTOR3 Dist = pObject1->Get_UnitInfo()->D3VecPos - pObject2->Get_UnitInfo()->D3VecPos;
	//obj1�� ����Ʈ ����
	D3DXVECTOR3 R1 = {pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width>>1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle())), -1* pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle())), 0.f };
	//obj1�� up ����
	D3DXVECTOR3 U1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), -1* pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), 0.f };
	//obj2�� right ����
	D3DXVECTOR3 R2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle())),  -1* pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle())), 0.f };
	//obj2�� up����
	D3DXVECTOR3 U2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), -1* pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), 0.f };
	
	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//������ ���⺤��
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i��° ������ ����ȭ, ũ�Ⱑ 1�� ���⺤�ͷ�
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}
	pObject2->Set_ObjState(COLLIDE);//�浹 ���·� ��ȯ
	GameObjectManager->Insert_GameObjectManager(CEffect::Create(pObject2,L"Bulletreflect"), GAMEOBJECT::EFFECT);
	pObject2->Set_TargetAngle(180.f);//���ư��� ������ �ݴ��
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

void CColliderManager::Collider_BulletAndProjectile(CGameObject * pBullet, CGameObject * pProjectile)
{
	if (pBullet->Get_ObjState() == COLLIDE)
		return;
	if (pProjectile->Get_ObjState() != THROW)
		return;
	//�� ����� �Ÿ� ����
	D3DXVECTOR3 Dist = pBullet->Get_UnitInfo()->D3VecPos - pProjectile->Get_ItemInfo()->D3VecPos;
	//obj1�� ����Ʈ ����
	D3DXVECTOR3 R1 = { pBullet->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pBullet->Get_UnitInfo()->wstrKey,pBullet->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pBullet->Get_TargetAngle())), -1 * pBullet->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pBullet->Get_UnitInfo()->wstrKey,pBullet->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pBullet->Get_TargetAngle())), 0.f };
	//obj1�� up ����
	D3DXVECTOR3 U1 = { pBullet->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pBullet->Get_UnitInfo()->wstrKey,pBullet->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pBullet->Get_TargetAngle() + 90)), -1 * pBullet->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pBullet->Get_UnitInfo()->wstrKey,pBullet->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pBullet->Get_TargetAngle() + 90)), 0.f };
	//obj2�� right ����
	D3DXVECTOR3 R2 = { pProjectile->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pProjectile->Get_ItemInfo()->wstrKey,pProjectile->Get_ItemInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pProjectile->Get_TargetAngle())),  -1 * pProjectile->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pProjectile->Get_ItemInfo()->wstrKey,pProjectile->Get_ItemInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pProjectile->Get_TargetAngle())), 0.f };
	//obj2�� up����
	D3DXVECTOR3 U2 = { pProjectile->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pProjectile->Get_ItemInfo()->wstrKey,pProjectile->Get_ItemInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pProjectile->Get_TargetAngle() + 90)), -1 * pProjectile->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pProjectile->Get_ItemInfo()->wstrKey,pProjectile->Get_ItemInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pProjectile->Get_TargetAngle() + 90)), 0.f };

	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//������ ���⺤��
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i��° ������ ����ȭ, ũ�Ⱑ 1�� ���⺤�ͷ�
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}
	pBullet->Set_ObjState(DEAD);
	pProjectile->Set_ObjState(DEAD);
}

void CColliderManager::Collider_BulletAndProjectile(list<CGameObject*>& pBulletList, list<CGameObject*>& pProjectilelst)
{
	if (pBulletList.empty() || pProjectilelst.empty())
		return;
	for (auto& pObject1 : pBulletList)
	{
		for (auto& pObject2 : pProjectilelst)
		{
			Collider_BulletAndProjectile(pObject1, pObject2);
		}
	}
}

void CColliderManager::Collider_Bullet(MYLINE tLine, CGameObject * pProjectile)
{


	switch (tLine.type)
	{
	case TERRAINTYPE::LAND:
		if (tLine.Start.y == tLine.End.y)
		{
			if (pProjectile->Get_UnitInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_UnitInfo()->D3VecPos.x > tLine.End.x)
				return;
			float Center = tLine.Start.y - pProjectile->Get_UnitInfo()->D3VecPos.y;
			for (int i = 0; i < 4; i++)
			{
				float Dot = tLine.Start.y - pProjectile->Get_HitboxObb()[i].y;
				if (0 >= Center* Dot)
				{
					pProjectile->Set_ObjState(DEAD);
					return;
				}
			}
		}
		{
			if (pProjectile->Get_UnitInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_UnitInfo()->D3VecPos.x > tLine.End.x)
				return;
			float LineAngle = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x);
			float LinePointCenter = LineAngle*(pProjectile->Get_UnitInfo()->D3VecPos.x - tLine.Start.x) + tLine.Start.y;
			float Center = LinePointCenter - pProjectile->Get_UnitInfo()->D3VecPos.y;// ���κ��� �Ʒ��� ���� ���� ���
			for (int i = 0; i < 4; i++)
			{
				float LinePointDot = LineAngle*(pProjectile->Get_HitboxObb()[i].x - tLine.Start.x) + tLine.Start.y;
				float Dot = LinePointDot - pProjectile->Get_HitboxObb()[i].y; //���κ��� �Ʒ��� ���� ���� ���
				if (0 >= Center* Dot)
				{
					pProjectile->Set_ObjState(DEAD);
					return;
				}
			}
		}
		break;
	case TERRAINTYPE::WALL:
		{
			if (pProjectile->Get_UnitInfo()->D3VecPos.y < tLine.Start.y || pProjectile->Get_UnitInfo()->D3VecPos.y > tLine.End.y)
				return;
			float Center = tLine.Start.x - pProjectile->Get_UnitInfo()->D3VecPos.x;
			for (int i = 0; i < 4; i++)
			{
				float Dot = tLine.Start.x - pProjectile->Get_HitboxObb()[i].x;
				if (0 >= Center* Dot)
				{
					pProjectile->Set_ObjState(DEAD);
					return;
				}
			}
		}
		break;
	case TERRAINTYPE::CELLING:
		{
			if (pProjectile->Get_UnitInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_UnitInfo()->D3VecPos.x > tLine.End.x)
				return;
			float Center = tLine.Start.y - pProjectile->Get_UnitInfo()->D3VecPos.y;
			for (int i = 0; i < 4; i++)
			{
				float Dot = tLine.Start.y - pProjectile->Get_HitboxObb()[i].y;
				if (0 >= Center* Dot)
				{
					pProjectile->Set_ObjState(DEAD);
					return;
				}
			}
		}	
		break;
	default:
		break;
	}
	
}


void CColliderManager::Collider_Bullet(vector<MYLINE> pCellingVec, list<CGameObject*> pProjectilelist)
{
	for (auto& tCelling : pCellingVec)
	{
		for (auto* pProject : pProjectilelist)
		{
			Collider_Bullet(tCelling, pProject);
		}
	}
	
}

void CColliderManager::Collider_BulletAndUnit(CGameObject * pObject1, CGameObject * pObject2)
{
	if (pObject2->Get_UnitInfo()->wstrKey != L"Player" && pObject1->Get_ObjState() == NONE) //�÷��̾ �ƴϰ� �Ѿ��� �ݻ�� ���°� �ƴϸ� �浹 x
		return;
	if (pObject2->Get_UnitInfo()->wstrKey == L"Player" && pObject1->Get_ObjState() == COLLIDE) //�÷��̾� �̰� �Ѿ��� �ݻ�� ���¶�� �浹 x
		return;
	if (pObject2->Get_ObjState() == DOWN)
		return;

	//�� ����� �Ÿ� ����
	D3DXVECTOR3 Dist = pObject1->Get_UnitInfo()->D3VecPos - pObject2->Get_UnitInfo()->D3VecPos;
	//obj1�� ����Ʈ ����
	D3DXVECTOR3 R1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle())), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle())), 0.f };
	//obj1�� up ����
	D3DXVECTOR3 U1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), 0.f };
	//obj2�� right ����
	D3DXVECTOR3 R2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle())),  -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle())), 0.f };
	//obj2�� up����
	D3DXVECTOR3 U2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), 0.f };

	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//������ ���⺤��
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i��° ������ ����ȭ, ũ�Ⱑ 1�� ���⺤�ͷ�
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}
	pObject1->Set_ObjState(DEAD);
	switch (pObject2->Get_UnitInfo()->type)
	{
	case UNITTYPE::PLAYER:
	/*	pObject2->Set_State(PLAYERSTATE::HURTFLY_BEGIN);
		pObject2->Set_ObjState(DOWN);
		pObject2->Set_HitAngle(pObject1->Get_TargetAngle());
		pObject2->Set_HitDir(pObject1->Get_UnitDir());
		pObject2->Set_HitSpeed(pObject1->Get_UnitSpeed()/4*pObject1->Get_UnitDir());
		pObject2->Set_UnitDir(-pObject1->Get_UnitDir());*/
		break;
	case UNITTYPE::GANGSTER:
		pObject2->Set_State(GANGSTERSTATE::HURTFLY);
		pObject2->Set_ObjState(DOWN);
		pObject2->Set_HitAngle(pObject1->Get_TargetAngle());
		pObject2->Set_HitDir(pObject1->Get_UnitDir());
		pObject2->Set_HitSpeed(pObject1->Get_UnitSpeed()/4*pObject1->Get_UnitDir());
		pObject2->Set_UnitDir(-pObject1->Get_UnitDir());
		GameObjectManager->Insert_GameObjectManager(CHitEffect::Create(pObject2), GAMEOBJECT::EFFECT);
		break;
	default:
		break;
	}
}

void CColliderManager::Collider_BulletAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2)
{
	if (pObjectlist1.empty() || pObjectlist2.empty())
		return;
	for (auto& pObject1 : pObjectlist1)
	{
		for (auto& pObject2 : pObjectlist2)
		{
			Collider_BulletAndUnit(pObject1, pObject2);
		}
	}
}

void CColliderManager::Collider_Projectile(MYLINE tLine, CGameObject * pProjectile)
{
	switch (tLine.type)
	{
	case TERRAINTYPE::LAND:
		if (tLine.Start.y == tLine.End.y)
		{
			if (pProjectile->Get_ItemInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_ItemInfo()->D3VecPos.x > tLine.End.x)
				return;
			float Center = tLine.Start.y - pProjectile->Get_ItemInfo()->D3VecPos.y;
			for (int i = 0; i < 4; i++)
			{
				float Dot = tLine.Start.y - pProjectile->Get_HitboxObb()[i].y;
				if (0 >= Center* Dot)
				{
					if (pProjectile->Get_ObjState() == THROW)
					{
						if (pProjectile->Get_ItemInfo()->wstrState == L"Butcherknife")
						{	
							pProjectile->Set_RotateAngle(0.f);
							pProjectile->Set_ObjState(COLLIDE);
							return;
						}
						pProjectile->Set_ObjState(DEAD);
						return;
					}
					
					if(pProjectile->Get_ObjState() != DEAD)
						pProjectile->Set_ObjState(COLLIDE);
				
					return;
				}
			}
		}
		{
			if (pProjectile->Get_ItemInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_ItemInfo()->D3VecPos.x > tLine.End.x)
				return;
			float LineAngle = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x);
			float LinePointCenter = LineAngle*(pProjectile->Get_ItemInfo()->D3VecPos.x - tLine.Start.x) + tLine.Start.y;
			float Center = LinePointCenter - pProjectile->Get_ItemInfo()->D3VecPos.y;// ���κ��� �Ʒ��� ���� ���� ���
			for (int i = 0; i < 4; i++)
			{
				float LinePointDot = LineAngle*(pProjectile->Get_HitboxObb()[i].x - tLine.Start.x) + tLine.Start.y;
				float Dot = LinePointDot - pProjectile->Get_HitboxObb()[i].y; //���κ��� �Ʒ��� ���� ���� ���
				if (0 >= Center* Dot)
				{
					if (pProjectile->Get_ObjState() == THROW)
					{
						if (pProjectile->Get_ItemInfo()->wstrState == L"Butcherknife")
						{
							pProjectile->Set_RotateAngle(LineAngle);
							pProjectile->Set_ObjState(COLLIDE);
							return;
						}
					}
					if (pProjectile->Get_ObjState() == THROW)
						pProjectile->Set_ObjState(DEAD);
					return;
				}
			}
		}
		break;
	case TERRAINTYPE::WALL:
	{
		if (pProjectile->Get_ItemInfo()->D3VecPos.y < tLine.Start.y || pProjectile->Get_ItemInfo()->D3VecPos.y > tLine.End.y)
			return;
		float Center = tLine.Start.x - pProjectile->Get_ItemInfo()->D3VecPos.x;
		for (int i = 0; i < 4; i++)
		{
			float Dot = tLine.Start.x - pProjectile->Get_HitboxObb()[i].x;
			if (0 >= Center* Dot)
			{
				if (pProjectile->Get_ObjState() == THROW)
				{
					if (pProjectile->Get_ItemInfo()->wstrState == L"Butcherknife")
					{
						if (tLine.Start.x > pProjectile->Get_ItemInfo()->D3VecPos.x)
							pProjectile->Set_RotateAngle(180.f);
						if (tLine.Start.x <= pProjectile->Get_ItemInfo()->D3VecPos.x)
							pProjectile->Set_RotateAngle(90.f);
		
					
						pProjectile->Set_ObjState(COLLIDE);
						return;
					}
				}
				if (pProjectile->Get_ObjState() == THROW)
					pProjectile->Set_ObjState(DEAD);
				return;
			}
		}
	}
	break;
	case TERRAINTYPE::CELLING:
	{
		if (pProjectile->Get_ItemInfo()->D3VecPos.x < tLine.Start.x || pProjectile->Get_ItemInfo()->D3VecPos.x > tLine.End.x)
			return;
		float Center = tLine.Start.y - pProjectile->Get_ItemInfo()->D3VecPos.y;
		for (int i = 0; i < 4; i++)
		{
			float Dot = tLine.Start.y - pProjectile->Get_HitboxObb()[i].y;
			if (0 >= Center* Dot)
			{
				if (pProjectile->Get_ObjState() == THROW)
				{
					if (pProjectile->Get_ItemInfo()->wstrState == L"Butcherknife")
					{
						pProjectile->Set_RotateAngle(180.f);
						pProjectile->Set_ObjState(COLLIDE);
						return;
					}
				}
				if (pProjectile->Get_ObjState() == THROW)
					pProjectile->Set_ObjState(DEAD);
				return;
			}
		}
	}
	break;
	default:
		break;
	}
}

void CColliderManager::Collider_Projectile(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist)
{
	for (auto& tLine : pLineVec)
	{
		for (auto* pProject : pProjectilelist)
		{
			Collider_Projectile(tLine, pProject);
		}
	}
}

void CColliderManager::Collider_ProjectileAndUnit(CGameObject * pObject1, CGameObject * pObject2) //����ü�� ����
{
	if (pObject2->Get_ObjState() == DOWN)
		return;
	if (pObject1->Get_ObjState() == NONE && (pObject2->Get_UnitInfo()->wstrKey != L"Player")) //���� �ŵ� �ƴϰ� �÷��̾ �ƴϸ� �Ѿ��
		return;
	if (pObject1->Get_ObjState() == THROW && pObject2->Get_UnitInfo()->wstrKey == L"Playey")//�����µ� �浹 ����� �÷��̾�� �Ѿ��
		return;
	//�� ����� �Ÿ� ����
	D3DXVECTOR3 Dist = pObject1->Get_ItemInfo()->D3VecPos - pObject2->Get_UnitInfo()->D3VecPos;
	//obj1�� ����Ʈ ����
	D3DXVECTOR3 R1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_ItemInfo()->wstrKey,pObject1->Get_ItemInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle())), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_ItemInfo()->wstrKey,pObject1->Get_ItemInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle())), 0.f };
	//obj1�� up ����
	D3DXVECTOR3 U1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_ItemInfo()->wstrKey,pObject1->Get_ItemInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_ItemInfo()->wstrKey,pObject1->Get_ItemInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), 0.f };
	//obj2�� right ����
	D3DXVECTOR3 R2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle())),  -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle())), 0.f };
	//obj2�� up����
	D3DXVECTOR3 U2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), 0.f };

	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//������ ���⺤��
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i��° ������ ����ȭ, ũ�Ⱑ 1�� ���⺤�ͷ�
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}

	switch (pObject2->Get_UnitInfo()->type)
	{
	case UNITTYPE::PLAYER:
		if (dynamic_cast<CPlayer*>(pObject2)->Get_Itme() != nullptr)
			return;
		if (GetAsyncKeyState('E') & 0X8000)
		{
			dynamic_cast<CPlayer*>(pObject2)->Set_Item(pObject1);
			pObject1->Set_ObjState(DEAD);
		}
		break;
	case UNITTYPE::GANGSTER:
		pObject2->Set_State(GANGSTERSTATE::HURTFLY);
		pObject1->Set_ObjState(DEAD);
		pObject2->Set_ObjState(DOWN);
		pObject2->Set_HitAngle(pObject1->Get_TargetAngle());
		pObject2->Set_HitDir(pObject1->Get_UnitDir());
		pObject2->Set_HitSpeed(pObject1->Get_UnitSpeed()/4*pObject1->Get_UnitDir());
		pObject2->Set_UnitDir(-pObject1->Get_UnitDir());
		GameObjectManager->Insert_GameObjectManager(CHitEffect::Create(pObject2), GAMEOBJECT::EFFECT);
		break;
	default:
		break;
	}
}

void CColliderManager::Collider_ProjectileAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2)
{
	if (pObjectlist1.empty() || pObjectlist2.empty())
		return;
	for (auto& pObject1 : pObjectlist1)
	{
		for (auto& pObject2 : pObjectlist2)
		{
			Collider_ProjectileAndUnit(pObject1, pObject2);
		}
	}
}

void CColliderManager::ColliderAttckAndUnit(CGameObject * pObject1, CGameObject * pObject2)
{
	if (pObject2->Get_ObjState() == DOWN)
		return;
	if (pObject1->Get_ObjState() == THROW && pObject2->Get_UnitInfo()->wstrKey == L"Playey")//�����µ� �浹 ����� �÷��̾�� �Ѿ��
		return;
	//�� ����� �Ÿ� ����
	D3DXVECTOR3 Dist = pObject1->Get_UnitInfo()->D3VecPos - pObject2->Get_UnitInfo()->D3VecPos;
	//obj1�� ����Ʈ ����
	D3DXVECTOR3 R1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle())), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle())), 0.f };
	//obj1�� up ����
	D3DXVECTOR3 U1 = { pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), -1 * pObject1->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject1->Get_UnitInfo()->wstrKey,pObject1->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*sinf(D3DXToRadian(pObject1->Get_TargetAngle() + 90)), 0.f };
	//obj2�� right ����
	D3DXVECTOR3 R2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle())),  -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Width >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle())), 0.f };
	//obj2�� up����
	D3DXVECTOR3 U2 = { pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)*cosf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), -1 * pObject2->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(pObject2->Get_UnitInfo()->wstrKey,pObject2->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1)* sinf(D3DXToRadian(pObject2->Get_TargetAngle() + 90)), 0.f };

	D3DXVECTOR3 v[4] = { R1,R2,U1,U2 };
	for (int i = 0; i < 4; i++)
	{
		//������ ���⺤��
		float sum = 0.f;
		D3DXVECTOR3 Dir{};
		D3DXVec3Normalize(&Dir, &v[i]);//i��° ������ ����ȭ, ũ�Ⱑ 1�� ���⺤�ͷ�
		for (int j = 0; j < 4; j++)
		{
			sum += fabs(D3DXVec3Dot(&Dir, &v[j]));
		}
		if (fabs(D3DXVec3Dot(&Dir, &Dist)) > sum)
			return;
	}

	switch (pObject2->Get_UnitInfo()->type)
	{
	case UNITTYPE::GANGSTER:
		pObject2->Set_State(GANGSTERSTATE::HURTFLY);
		pObject2->Set_ObjState(DOWN);
		pObject2->Set_HitAngle(pObject1->Get_TargetAngle());
		pObject2->Set_HitDir(pObject1->Get_UnitDir());
		pObject2->Set_HitSpeed(pObject1->Get_UnitSpeed() / 4 * pObject1->Get_UnitDir());
		pObject2->Set_UnitDir(-pObject1->Get_UnitDir());
		GameObjectManager->Insert_GameObjectManager(CHitEffect::Create(pObject2), GAMEOBJECT::EFFECT);
		break;
	default:
		break;
	}
}

void CColliderManager::ColliderAttckAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2)
{
	if (pObjectlist1.empty() || pObjectlist2.empty())
		return;
	for (auto& pObject1 : pObjectlist1)
	{
		for (auto& pObject2 : pObjectlist2)
		{
			ColliderAttckAndUnit(pObject1, pObject2);
		}
	}
}

bool CColliderManager::Collide_TerrainAndRay(vector<MYLINE> pTerrainvec, CGameObject* pUnit)
{
	D3DXVECTOR3 Left, Right;
	Left = pUnit->Get_Target()->Get_UnitInfo()->D3VecPos;
	Right = pUnit->Get_UnitInfo()->D3VecPos;
	if (Left.x > Right.x)
	{
		D3DXVECTOR3 Temp = Left;
		Left = Right;
		Right = Temp;
	}
	float RayAngle = (Right.y - Left.y) / (Right.x - Left.x); // ������ ����
	for (auto& pTerrain : pTerrainvec)
	{
		if (pTerrain.type == TERRAINTYPE::PASSABLE)
			return false;
		//���� ����1 Ÿ�ٺ��� ��������Ʈ�� x��ǥ ���� ���� ����
		//���� ����2 �����ͺ��� ��ŸƮ����Ʈ x��ǥ�� ū�� ����
		if (pTerrain.End.x <= Left.x)
			continue;
		if (pTerrain.Start.x >= Right.x)
			continue;
		if (Left.y <= Right.y)
		{
			//���� 1 ��ŸƮ ����Ʈ�� ���κ��� y���� ũ�� ��������Ʈy�� ���κ��� y���� ���� ��
			//-��ŸƮ�� ����Ʈ���� ���ʿ� ������ ��������Ʈ�� ���⺸�ٸ� �۸� �ȴ�.
			//-��ŸƮ�� ����Ʈ���� ���ʿ� ���� ���� ��������Ʈ�� ����Ʈ�� ����Ʈ ���̿� �־�� �Ѵ�,
			if (pTerrain.Start.y >= RayAngle*(pTerrain.Start.x - Left.x) + Left.y)//���� ������ ��ŸƮ�� �Ʒ��� �ִ�.
			{
				if (pTerrain.Start.x >= Left.x) //�������� ���� �� �ʿ� ������
					if (pTerrain.End.y <= RayAngle*(pTerrain.End.x - Left.x) + Left.y &&  pTerrain.End.x <= Right.x)
						return true;
				if (pTerrain.Start.x < Left.x)
				{
					float TerrainAngle = (pTerrain.End.y - pTerrain.Start.y) / (pTerrain.End.x - pTerrain.Start.x);
					float TLA = (Left.y-pTerrain.Start.y) / (Left.x - pTerrain.Start.x);
					float TRA = (Right.y - pTerrain.Start.y) / (Right.x - pTerrain.Start.x);
					if ((pTerrain.End.y <= RayAngle*(pTerrain.End.x - Left.x) + Left.y) && (TerrainAngle >= TLA && TerrainAngle <= TRA))
						return true;
				}
			}
			//���� 2 ��ŸƮ ����Ʈ�� ���κ��� y���� ������
			//-��ŸƮ�� ����Ʈ���� ���ʿ� ������ ���尡 ���κ��� y���� ũ�⸸ �ϸ� �ȴ�
			//-��ŸƮ�� ����Ʈ���� ���ʿ� ������ ���尡 ���κ��� y���� ũ�� ����Ʈ�� ����Ʈ ���̿� ����.
			if (pTerrain.Start.y <= RayAngle*(pTerrain.Start.x - Left.x) + Left.y) //��ŸƮ ����Ʈ�� ���̺��� ��
			{
				if (pTerrain.Start.x >= Left.x)
					if (pTerrain.End.y >= RayAngle*(pTerrain.End.x - Left.x) + Left.y  && pTerrain.End.x <= Right.x)
						return true;
				if (pTerrain.Start.x < Left.x)
				{
					float TerrainAngle = (pTerrain.End.y - pTerrain.Start.y) / (pTerrain.End.x - pTerrain.Start.x);
					float TLA = (Left.y - pTerrain.Start.y) / (Left.x - pTerrain.Start.x);
					float TRA = (Right.y - pTerrain.Start.y) / (Right.x - pTerrain.Start.x);
					if (pTerrain.End.y >= RayAngle*(pTerrain.End.x - Left.x) + Left.y && (TerrainAngle <= TLA && TerrainAngle >= TRA))
						return true;
				}
				
			}
		}
		if (Left.y > Right.y)
		{
			if (pTerrain.Start.y >= RayAngle*(pTerrain.Start.x - Left.x) + Left.y)
			{
				if (pTerrain.Start.x >= Left.x)
					if (pTerrain.End.y <= RayAngle*(pTerrain.End.x - Left.x) + Left.y &&  pTerrain.End.x <= Right.x)
						return true;
				if (pTerrain.Start.x < Left.x)
				{
					float TerrainAngle = (pTerrain.End.y - pTerrain.Start.y) / (pTerrain.End.x - pTerrain.Start.x);
					float TLA = (Left.y - pTerrain.Start.y) / (Left.x - pTerrain.Start.x);
					float TRA = (Right.y - pTerrain.Start.y) / (Right.x - pTerrain.Start.x);
					if ((pTerrain.End.y <= RayAngle*(pTerrain.End.x - Left.x) + Left.y) && (TerrainAngle >= TLA && TerrainAngle <= TRA))
						return true;
				}
			}

			if (pTerrain.Start.y <= RayAngle*(pTerrain.Start.x - Left.x) + Left.y)
			{
				if (pTerrain.Start.x >= Left.x)
					if (pTerrain.End.y >= RayAngle*(pTerrain.End.x - Left.x) + Left.y  && pTerrain.End.x <= Right.x)
						return true;
				if (pTerrain.Start.x < Left.x)
				{
					float TerrainAngle = (pTerrain.End.y - pTerrain.Start.y) / (pTerrain.End.x - pTerrain.Start.x);
					float TLA = (Left.y - pTerrain.Start.y) / (Left.x - pTerrain.Start.x);
					float TRA = (Right.y - pTerrain.Start.y) / (Right.x - pTerrain.Start.x);
					if (pTerrain.End.y >= RayAngle*(pTerrain.End.x - Left.x) + Left.y && (TerrainAngle <= TLA && TerrainAngle >= TRA))
						return true;
				}

			}
		}
	}


	return false;
}

