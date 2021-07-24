#pragma once
#include "GameObject.h"
class CGranade:
	public CGameObject
{
private:

	CGranade();
public:
	virtual ~CGranade();


	//�Ѿ��� Ÿ�� �������� ������  ��� ��� �������� ���� �� �� �̷������ �Ѵ�.
public:
	static	CGameObject*	Create(CGameObject* pShooter);

public:
	void	Set_Shooter(CGameObject* pShooter) { m_pShooter = pShooter; }
	void	 DrawRange();
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float			m_fProjectileAngle;
	CGameObject*	m_pShooter;
	float			x;
	float			m_fRadius;
	float			m_fHeight;
};
