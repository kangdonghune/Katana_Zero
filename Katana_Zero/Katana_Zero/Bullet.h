#pragma once
#include "GameObject.h"
class CBullet :
	public CGameObject
{
private:

	CBullet();
public:
	virtual ~CBullet();


	//총알은 타겟 방향으로 자전과  쏘는 사람 기준으로 공전 둘 다 이루어져야 한다.
public:
	static	CGameObject*	Create(CGameObject* pShooter);
	
public:
	void	Set_Shooter(CGameObject* pShooter) { m_pShooter = pShooter; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float			m_fProjectileAngle;
	float			x;
};

