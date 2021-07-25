#pragma once
#include "GameObject.h"
class CUI :
	public CGameObject
{
private:
	CUI();
public:
	~CUI();

public:
	void Setting_Clear();
public:
	static CGameObject* Create();


	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;

	virtual void Update_GameObject() override;

	virtual void LateUpdate_GameObject() override;

	virtual void Render_GameObject() override;

	virtual void Release_GameObject() override;

private:
	float	m_fTimer;
	float	m_fSlowTimer;
	int		InventoryNum;
};

