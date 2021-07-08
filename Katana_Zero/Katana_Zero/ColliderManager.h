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
	void	Collider_Land(vector<MYLINE> pLandvec, vector<CGameObject*> pUnitVec);
	void	Collider_Wall(vector<MYLINE> pWallvec, vector<CGameObject*> pUnitVec);
	void	Collider_Celling(vector<MYLINE> pCellingVec, vector<CGameObject*> pUnitVec);
};

