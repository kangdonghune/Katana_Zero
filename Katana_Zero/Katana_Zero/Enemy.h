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


protected:
	void	Render_Raytraise();
	bool	Ray_Traising();
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:



};

