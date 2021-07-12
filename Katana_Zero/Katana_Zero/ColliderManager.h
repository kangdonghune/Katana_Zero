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
	void	Collider_Land(vector<MYLINE> pLandvec, CGameObject* pUnit);
	void	Collider_Wall(vector<MYLINE> pWallvec, CGameObject* pUnit);
	void	Collider_Celling(vector<MYLINE> pCellingVec, CGameObject* pUnit);
	void	Collider_Land(vector<MYLINE> pLandvec, list<CGameObject*> pUnitlst);
	void	Collider_Wall(vector<MYLINE> pWallvec, list<CGameObject*> pUnitlst);
	void	Collider_Celling(vector<MYLINE> pCellingVec, list<CGameObject*> pUnitlst);
	
	void	Collider_Obb(CGameObject* pObject1, CGameObject* pObject2);
	void	Collider_Obb(list<CGameObject*>& pObjectvec1, list<CGameObject*>& pObjectvec2);
};

