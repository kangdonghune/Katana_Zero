#pragma once
#include "GameObject.h"
class CBullet :
	public CGameObject
{
private:

	CBullet();
public:
	virtual ~CBullet();


	//�Ѿ��� Ÿ�� �������� ������  ��� ��� �������� ���� �� �� �̷������ �Ѵ�.
public:
	static	CGameObject*	Create(CGameObject* pShooter);
	
public:
	void	Set_Shooter(CGameObject* pShooter) { m_pShooter = pShooter; }

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float			m_fProjectileAngle;
	float			x;
};

