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
	//�÷��̾� �����浹
	void	Collider_Land(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_LandAndEnemy(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_PassAble(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_Wall(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_Celling(vector<MYLINE> pCellingVec, CGameObject* pUnit);
	void	Collider_StageChange(vector<MYLINE> pChangeVec, CGameObject* pUnit);
	//���ָ���Ʈ �����浹
	void	Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_PassAbleAndEnemy(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst);
	void	Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst);
	//���� �浹
	void	Collider_LandAndBoss(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_WallBoss(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_BulletAndBoss(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_AttckAndBoss(CGameObject* pObject1, CGameObject* pObject2);
	//OBB źȯ�� ���� ����Ʈ �浹
	void	Collider_Obb(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_Obb(list<CGameObject*>& pObjectvec1, list<CGameObject*>& pObjectvec2);

	//źȯ�� ����ü �浹
	void	Collider_BulletAndProjectile(CGameObject* pBullet, CGameObject* pProjectile);
	void	Collider_BulletAndProjectile(list<CGameObject*>& pBulletList, list<CGameObject*>& pProjectilelst);

	//�Ѿ� �� �浹
	void	Collider_Bullet(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Bullet(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	//�Ѿ� ���� �浹
	void	Collider_BulletAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_BulletAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	//����ü �� �浹
	void	Collider_Projectile(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Projectile(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	//����ü ���� �浹
	void	Collider_ProjectileAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_ProjectileAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	
	//����ü ���� �浹
	void	ColliderAttckAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	ColliderAttckAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);
	//����Ʈ���̽�
	bool	Collide_TerrainAndRay(vector<MYLINE>pTerrainvec, CGameObject* pUnit);
};

