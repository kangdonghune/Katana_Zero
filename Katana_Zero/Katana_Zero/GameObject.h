#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	UNITINFO*&	Set_Info() { return m_pUnitInfo; }
	void		Set_Info(UNITINFO* pInfo) { m_pUnitInfo = pInfo; }
	void		Set_PivotX(float fPos) { m_vecPivot.x = fPos; }
	void		Set_PivotY(float fPos) { m_vecPivot.y = fPos; }
	void		Set_State(PLAYERSTATE::State state) { m_State = state; }
	void		Set_UnitDir(int iDir) { m_iUnitDir = iDir; }
	void		Set_OldCollide(int Collide) { m_iOldCollide = Collide; }
	void		Set_FrameStart(float fIndex) { m_tFrame.fFrameStart = fIndex; }
	void		Set_ObjState(int State) { m_iObjState = State; }
	void		Set_TargetAngle(float Angle) { m_fTargetAngle += Angle; }


	void Update_HitBox();
	void Update_HitBoxOBB();
	void FrameMove(float fSpeed);
	void Render_HitBox();
	void Render_HitBoxObb();
	void Render_ObbLine();
	void Render_ObbLineD3D();
	void Update_Frame();
	bool Check_FrameEnd();

	const D3DXVECTOR3*			Get_HitboxObb() { return m_tHitBoxObb; }
	const int					Get_ObjState() { return m_iObjState; }
	const float					Get_TargetAngle() { return m_fTargetAngle; }
	const float					Get_RotateAngle() { return m_fRotateAngle; }
	const int					Get_UnitDir() { return m_iUnitDir; }
	const PLAYERSTATE::State	Get_State() { return m_State; }
	const int					Get_OldCollide() { return m_iOldCollide; }
	const D3DXVECTOR3			Get_Pivot() { return m_vecPivot; }
	const float					Get_Ratio() { return m_fRatio; }
	const RECT					Get_Hitbox() { return m_tHitBox; }
	const UNITINFO*				Get_UnitInfo() { return m_pUnitInfo; }
	const float					Get_UnitRatio() { return m_fRatio; }

	UNITINFO*&					Get_DeleteInfo() { return m_pUnitInfo; }

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual void	Update_GameObject() PURE;
	virtual void	LateUpdate_GameObject() PURE;
	virtual void	Render_GameObject()	PURE;
	virtual	void	Release_GameObject() PURE;
	
protected:
	UNITINFO*				m_pUnitInfo;
	PLAYERSTATE::State		m_State;
	RECT					m_tHitBox;
	D3DXVECTOR3				m_tHitBoxObb[4];
	FRAME					m_tFrame;
	float					m_fSpeed;
	float					m_fRatio;
	wstring					m_wstrOldState;
	wstring					m_wstrCurState;
	int						m_iUnitDir;// y축 회전 시 적용 값으로 바라보는 방향을 결정.
	D3DXVECTOR3				m_vecPivot;
	float					m_fUnitSpeed;
	float					m_fDefaultUnitSpeed;
	float					m_fRotateAngle;
	int						m_iOldCollide;
	float					m_fAttackLimit;
	float					m_fAttackCool;
	CGameObject*			m_pTarget;
	float					m_fTargetAngle;
	int						m_iObjState;

};

