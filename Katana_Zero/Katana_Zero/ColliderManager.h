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
	void	Collider_LandAndEnemy(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_PassAble(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_Wall(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_Celling(vector<MYLINE> pCellingVec, CGameObject* pUnit);
	void	Collider_StageChange(vector<MYLINE> pChangeVec, CGameObject* pUnit);
	//유닛리스트 라인충돌
	void	Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst);
	void	Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst);
	//보스 충돌
	void	Collider_LandAndBoss(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_WallBoss(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_BulletAndBoss(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_AttckAndBoss(CGameObject* pObject1, CGameObject* pObject2);
	//OBB 탄환과 공격 이펙트 충돌
	void	Collider_Obb(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_Obb(list<CGameObject*>& pObjectvec1, list<CGameObject*>& pObjectvec2);

	//탄환과 투사체 충돌
	void	Collider_BulletAndProjectile(CGameObject* pBullet, CGameObject* pProjectile);
	void	Collider_BulletAndProjectile(list<CGameObject*>& pBulletList, list<CGameObject*>& pProjectilelst);

	//총알 벽 충돌
	void	Collider_Bullet(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Bullet(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	//총알 유닛 충돌
	void	Collider_BulletAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_BulletAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	//투사체 벽 충돌
	void	Collider_Projectile(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Projectile(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	//투사체 유닛 충돌
	void	Collider_ProjectileAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_ProjectileAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	
	//투사체 유닛 충돌
	void	ColliderAttckAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	ColliderAttckAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	//레이트레이싱
	bool	Collide_TerrainAndRay(vector<MYLINE>pTerrainvec, CGameObject* pUnit);
};

