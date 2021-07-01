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
};

