#pragma once
#include "Enemy.h"
class CGameObject;
class CBoss :
	public CEnemy
{
private:
	CBoss();
public:
	~CBoss();

public:
	static	CGameObject*	 Create(CGameObject* pTarget, UNITINFO* pInfo);

private:
		void Aim();
		void Dash();
		void Dash_end();
		void Dash_slowmotion();
		void BDead();
		void Hurt();
		void Idle();
		void Predash();
		void Prejump();
		void Putback();
		void Recover();
		void Roll();
		void Sweep();
		void Takeout();
		void Teleport_in();
		void Teleport_out();
		void Teleport_in_sweepR();
		void Teleport_in_sweepL();
		void Teleport_out_sweep();
		void Teleport_in_ground();
		void Teleport_out_ground();
		void Jump();
		void Walljump();
		void Walljump_land();

private:
	void	Update_TargetRotate();
	void	Update_D3DPos();
	void	Update_UnitState();
	void	Render_shotLine();
	void	Shoot_Laser();
	void	Shoot_Granade();
	void	AngleToLWall();
	void	AngleToRWall();
	bool	AngleToLCelling();
	bool	AngleToRCelling(); 
	void	AngleToLLand();
	void	AngleToRLand();
	void	SpinShot();
	void	SweepLaser();
	void	Update_Function();
public:

	void	Set_BossState(BOSSSTATE::State state) { m_BossState = state; }
	const	BOSSSTATE::State		Get_BossState() { return m_BossState; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	BOSSSTATE::State m_BossState;
	POINT ShotLine[2];
	CGameObject* m_pLaser;
	CGameObject* m_pGranade;
	float			m_fJumpAngle;
	float			m_fCenter;
	float			m_fSpinAngle;
	BOSSSKILL::Skill	m_BossSkill;
	int				m_teleportCount;
	bool			m_bChange;
	float			m_fLaserAngle;
	bool			m_bGo;
	bool			m_bDown;
	int				m_iLife;
};