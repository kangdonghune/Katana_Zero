#pragma once
#include "Enemy.h"
class CGameObject;
class CGangster :
	public CEnemy
{
private:
	CGangster();
public:
	~CGangster();

public:
	static	CGameObject*	 Create(CGameObject* pTarget, UNITINFO* pInfo);

private:
	void	Update_TargetRotate();



public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:


};

