#pragma once
#include "GameObject.h"
class CPlayerAttack :
	public CGameObject
{
private:
	CPlayerAttack();
public:
	~CPlayerAttack();

public:
	static CGameObject* Create(CGameObject* pTarget);
private:
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

