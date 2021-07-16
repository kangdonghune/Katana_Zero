#pragma once
class CGameObject;
class CColliderManager
{
	DECLARE_SINGLETON(CColliderManager)
private:
	CColliderManager();
public:
	~CColliderManager();

public:
	//플레이어 라인충돌
	void	Collider_Land(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_Wall(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_Celling(vector<MYLINE> pCellingVec, CGameObject* pUnit);
	//유닛리스트 라인충돌
	void	Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst);
	void	Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst);
	
	//OBB 탄환과 공격 이펙트 충돌
	void	Collider_Obb(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_Obb(list<CGameObject*>& pObjectvec1, list<CGameObject*>& pObjectvec2);
	//투사체와 벽 충돌
	void	Collider_Projectile(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Projectile(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	void	Collider_ProjectileAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_ProjectileAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);

	//레이트레이싱
	bool	Collide_TerrainAndRay(vector<MYLINE>pTerrainvec, CGameObject* pUnit);
};

