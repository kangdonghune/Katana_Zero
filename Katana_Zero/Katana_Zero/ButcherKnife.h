#pragma once
#include "GameObject.h"
class CButcherKnife :
	public CGameObject
{
private:
	CButcherKnife();
public:
	virtual ~CButcherKnife();

public:
	static CGameObject* Create(ITEMINFO* pInfo);
	static CGameObject* Create(CGameObject* pTarget);
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

