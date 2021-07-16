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
		pUnit->Set_Info()->iCollide |= C_LAND; //�浹 �߰�
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pLand == nullptr)
	{
		if(iCollide & C_LAND)
			pUnit->Set_Info()->iCollide ^= C_LAND; //�浹 ����
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING))
			return;

		pUnit->Set_Info()->iCollide |= C_NONE; //�浹 ����
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
		if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top) //ĳ���Ͱ� �� ���� ���̿� �Ϻζ� ���� ���� ��
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
		pUnit->Set_Info()->iCollide |= C_WALL| dir;
		if (pUnit->Get_UnitInfo()->iCollide  & C_NONE)
			pUnit->Set_Info()->iCollide ^= C_NONE; //�浹 ����
	}

	if (pWall == nullptr)
	{
		if (iCollide & C_WALL)
			pUnit->Set_Info()->iCollide ^= C_WALL|C_WALLL|C_WALLR;
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING)) //�� �� ���� �� �ϳ��� ��ġ��
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
		if (pUnit->Get_UnitInfo()->iCollide & (C_LAND | C_WALL | C_CELLING))
			return;

		pUnit->Set_Info()->iCollide |= C_NONE; //�浹 ����
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
			if (tLine.Start.x <= Pivot.x && tLine.End.x > Pivot.x)
			{
				if (tLine.Start.y == tLine.End.y) // ��� �����̸�
				{
					LONG lDistance = tLine.Start.y - (*iter)->Get_Hitbox().bottom;
					if (lDistance <= 0 && lDistance >= -1 * iHeight)
					{
						(*iter)->Set_PivotY(tLine.Start.y);
						*ppLand = &tLine;
					}

				}
				else // �ƴϸ�
				{
					float fLineHeight = (tLine.End.y - tLine.Start.y) / (tLine.End.x - tLine.Start.x)*(Pivot.x - tLine.Start.x) + tLine.Start.y;
					LONG lDistance = fLineHeight - Pivot.y;
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
			(*iter)->Set_Info()->iCollide |= C_LAND; //�浹 �߰�
			if ((*iter)->Get_UnitInfo()->iCollide  & C_NONE)
				(*iter)->Set_Info()->iCollide ^= C_NONE; //�浹 ����
		}

		if (pLand == nullptr)
		{
			if (iCollide & C_LAND)
				(*iter)->Set_Info()->iCollide ^= C_LAND; //�浹 ����
			if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING))
				return;

			(*iter)->Set_Info()->iCollide |= C_NONE; //�浹 ����
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
			if (tLine.Start.y <= tHitBox.bottom  && tLine.End.y >= tHitBox.top) //ĳ���Ͱ� �� ���� ���̿� �Ϻζ� ���� ���� ��
			{
				LONG lDistanceL = tLine.Start.x - (Pivot.x - iWidth);
				LONG lDistanceR = (Pivot.x + iWidth) - tLine.Start.x;
				if (lDistanceL >= 0 && lDistanceL <= iWidth) //���ʺ��� ���� ���
				{
					(*iter)->Set_PivotX(Pivot.x + lDistanceL);
					*ppWall = &tLine;
					dir = C_WALLL;
					continue;
				}
				if (lDistanceR >= 0 && lDistanceR <= iWidth) //������ ��
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
					(*iter)->Set_Info()->iCollide ^= C_NONE; //�浹 ����
			}

			if (pWall == nullptr)
			{
				if (iCollide & C_WALL)
					(*iter)->Set_Info()->iCollide ^= C_WALL | C_WALLL | C_WALLR;
				if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALLL | C_WALLR | C_CELLING)) //�� �� ���� �� �ϳ��� ��ġ��
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
				if (tLine.Start.y == tLine.End.y) // ��� �����̸�
				{
					LONG lDistance = tLine.Start.y - (*iter)->Get_Hitbox().top;
					if (lDistance >= 0 && lDistance <= iHeight)
					{
						(*iter)->Set_PivotY(Pivot.y + lDistance);
						*ppCelling = &tLine;
					}

				}
				else // �ƴϸ�
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
				(*iter)->Set_Info()->iCollide ^= C_NONE; //�浹 ����
		}

		if (pCelling == nullptr)
		{
			if (iCollide & C_CELLING)
				(*iter)->Set_Info()->iCollide ^= C_CELLING;
			if ((*iter)->Get_UnitInfo()->iCollide & (C_LAND | C_WALL | C_CELLING))
				return;

			(*iter)->Set_Info()->iCollide |= C_NONE; //�浹 ����
		}
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

void CColliderManager::Collider_Projectile(MYLINE tLine, CGameObject * pProjectile)
{
	switch (tLine.type)
	{
	case TERRAINTYPE::LAND:
		if (tLine.Start.y == tLine.End.y)
		{
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


void CColliderManager::Collider_Projectile(vector<MYLINE> pCellingVec, list<CGameObject*> pProjectilelist)
{
	for (auto& tCelling : pCellingVec)
	{
		for (auto* pProject : pProjectilelist)
		{
			Collider_Projectile(tCelling, pProject);
		}
	}
	
}

void CColliderManager::Collider_ProjectileAndUnit(CGameObject * pObject1, CGameObject * pObject2)
{
	if (pObject2->Get_UnitInfo()->wstrKey != L"Player" && pObject1->Get_ObjState() == NONE) //�÷��̾ �ƴϰ� �Ѿ��� �ݻ�� ���°� �ƴϸ� �浹 x
		return;
	if (pObject2->Get_UnitInfo()->wstrKey == L"Player" && pObject1->Get_ObjState() == COLLIDE) //�÷��̾� �̰� �Ѿ��� �ݻ�� ���¶�� �浹 x
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
		//pObject2->Set_State(PLAYERSTATE::HURTFLY_BEGIN);
		break;
	case UNITTYPE::GANGSTER:
		pObject2->Set_State(GANGSTERSTATE::HURTFLY);
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

