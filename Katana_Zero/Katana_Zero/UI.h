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


	// CGameObject��(��) ���� ��ӵ�
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

