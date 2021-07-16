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
	void	Collider_Wall(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_Celling(vector<MYLINE> pCellingVec, CGameObject* pUnit);
	//���ָ���Ʈ �����浹
	void	Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst);
	void	Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst);
	
	//OBB źȯ�� ���� ����Ʈ �浹
	void	Collider_Obb(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_Obb(list<CGameObject*>& pObjectvec1, list<CGameObject*>& pObjectvec2);
	//����ü�� �� �浹
	void	Collider_Projectile(MYLINE tLine, CGameObject* pProjectile);
	void	Collider_Projectile(vector<MYLINE> pLineVec, list<CGameObject*> pProjectilelist);
	void	Collider_ProjectileAndUnit(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_ProjectileAndUnit(list<CGameObject*>& pObjectlist1, list<CGameObject*>& pObjectlist2);

	//����Ʈ���̽�
	bool	Collide_TerrainAndRay(vector<MYLINE>pTerrainvec, CGameObject* pUnit);
};

