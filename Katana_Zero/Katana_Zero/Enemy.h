#pragma once
#include "GameObject.h"
class CEnemy :
	public CGameObject
{
public:
	CEnemy();
	virtual ~CEnemy();


public:
	void	Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

	void	Find_TargetAngle();

	const	float	Get_TargetAngle() { return m_fTargetAngle; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:

	CGameObject*			m_pTarget;
	float					m_fTargetAngle;
};

