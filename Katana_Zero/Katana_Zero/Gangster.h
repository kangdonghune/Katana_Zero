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
	void Idle();
	void Aim();
	void Enterstair();
	void Fall();
	void Hurtfly();
	void Hurtground();
	void Leavestair();
	void Run();
	void Turn();
	void Walk();
	void Whip();

	void Shooting();

private:
	void	Update_TargetRotate();
	void	Update_D3DPos();
	void	Update_UnitState();
	void	Render_Raytraise();

	bool	Ray_Traising();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:


};

