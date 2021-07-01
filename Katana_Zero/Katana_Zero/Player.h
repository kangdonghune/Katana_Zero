#pragma once
#include "GameObject.h"
class CPlayer :
	public CGameObject
{
private:
	CPlayer();
public:
	~CPlayer();
public:
	static CPlayer* Create(UNITINFO* pInfo);

public:

	void Update_KeyInput();
	void Update_UnitState();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;

	virtual void Update_GameObject() override;

	virtual void LateUpdate_GameObject() override;

	virtual void Render_GameObject() override;

	virtual void Release_GameObject() override;

private:

};

