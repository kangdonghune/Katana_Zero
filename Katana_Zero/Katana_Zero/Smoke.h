#pragma once
#include "GameObject.h"
class CSmoke :
	public CGameObject
{
private:
	CSmoke();
public:
	virtual ~CSmoke();

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

