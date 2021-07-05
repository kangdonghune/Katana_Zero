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

private:
	void Idle();
	void Idle_to_walk();
	void Idle_to_run();
	void Run();
	void Run_to_idle();
	void Walk();
	void Attack();
	void Precrouch();
	void Crouch();
	void Postcrouch();
	void Dooropen();
	void Doorbreakfull();
	void Fall();
	void Hurtfly_begin();
	void Hurtfly_loob();
	void Hurtground();
	void Hurtrecover();
	void Jump();
	void Dance();
	void Flip();
	void Roll();
	void Wallslide();


public:

//	void Update_KeyInput();
	void Update_UnitState();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;

	virtual void Update_GameObject() override;

	virtual void LateUpdate_GameObject() override;

	virtual void Render_GameObject() override;

	virtual void Release_GameObject() override;

private:
	float	m_fDefaultSpeed;
	float	m_fJumpDistance; //�ִ� ���� ����
	int		m_iActing; //������ �ϰ� �ִ��� ����.
	float	m_fJumpAngle;
	float	m_fOldJumpAngle;
	float	m_fFallAngle;
};

