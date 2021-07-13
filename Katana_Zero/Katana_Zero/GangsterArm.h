#pragma once
#include "GameObject.h"
class CGangsterArm :
	public CGameObject
{
	CGangsterArm();
public:
	~CGangsterArm();

public:
	static CGameObject* Create(CGameObject* pTarget);


public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

