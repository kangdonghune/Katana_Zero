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
	void	Collider_LandandProjectile(vector<MYLINE> pLandvec, CGameObject* pProjectile);
	void	Collider_WallandProjectile(vector<MYLINE> pWallvec, CGameObject* pProjectile);
	void	Collider_CellingandProjectile(vector<MYLINE> pCellingVec, CGameObject* pProjectile);
	void	Collider_Projectile(vector<MYLINE> pCellingVec, list<CGameObject*> pProjectilelist);
};

