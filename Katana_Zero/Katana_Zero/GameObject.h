#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	UNITINFO*&	Set_Info() { return m_pUnitInfo; }
	void		Set_Info(UNITINFO* pInfo) { m_pUnitInfo = pInfo; }
	void		Set_Info(ITEMINFO* pInfo) { m_pItemInfo = pInfo; }
	void		Set_PivotX(float fPos) { m_vecPivot.x = fPos; }
	void		Set_PivotY(float fPos) { m_vecPivot.y = fPos; }
	void		Set_State(PLAYERSTATE::State state) { m_State = state; }
	void		Set_State(GANGSTERSTATE::State state) { m_GangState = state; }
	void		Set_UnitDir(int iDir) { m_iUnitDir = iDir; }
	void		Set_OldCollide(int Collide) { m_iOldCollide = Collide; }
	void		Set_FrameStart(float fIndex) { m_tFrame.fFrameStart = fIndex; }
	void		Set_ObjState(int State) { m_iObjState = State; }
	void		Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void		Set_TargetAngle(float Angle) { m_fTargetAngle += Angle; }
	void		Set_TargetAngle2(float Angle) { m_fTargetAngle = Angle; }
	void		Set_ItemPosY(float fPoint) { m_pItemInfo->D3VecPos.y = fPoint; }
	void		Set_RotateAngle(float fAngle) { m_fRotateAngle = fAngle; }
	void		Set_HitSpeed(float fSpeed) { m_fHitSpeed = fSpeed; }
	void		Set_HitAngle(float fAngle) { m_fHitAngle = fAngle; }
	void		Set_HitDir(int Dir) { m_iHitDir = Dir; }
	void		Set_CurLine(MYLINE tLine) { m_tCurLand = tLine; }
	void		Set_OldLine(MYLINE tLine) { m_tOldLand = tLine; }
	void		Set_UnitSpeed(float speed) { m_fUnitSpeed = speed; }
	void		Set_WidthRatio(float wRatio) { m_fWidthRatio = wRatio; }
	void		Set_HeightRatio(float hRatio) { m_fHeightRatio = hRatio; }
	void		Set_Pos(float x, float y) { m_pUnitInfo->D3VecPos.x = x; m_pUnitInfo->D3VecPos.y = y; }
	void		Set_FrameSpeed(float speed) { m_fSpeed = speed; }
	void		Set_GangState(GANGSTERSTATE::State State) { m_GangState = State; }

	void Update_HitBox();
	void Update_HitBoxOBB();
	void Update_HitBoxLaser();
	void Update_ProjectileHitBoxOBB();
	void FrameMove(float fSpeed);
	void FrameMoveIndependence(float fSpeed);
	void Render_HitBox();
	void Render_HitBoxObb();
	void Render_ObbLine();
	void Render_ObbLineD3D();
	void Update_Frame();
	bool Check_FrameEnd();
	void Update_TargetDist();
	void Update_CurLand();
	bool Find_Root(const MYLINE StartLine, const MYLINE TargetLine);
	bool RotateRootL(const MYLINE StartLine, const MYLINE TargetLine);
	bool RotateRootR(const MYLINE StartLine, const MYLINE TargetLine);
	bool Check_ID(int ID);
	
	const float					Get_TargetDist() { return m_fTargetDist; }
	const MYLINE				Get_NowLine() { return m_NowLine; }
	const MYLINE				Get_NextLine() { return m_NextLine; }
	const MYLINE				Get_OldLine() { return m_tOldLand; }
	const MYLINE				Get_CurLine() { return m_tCurLand; }
	const float					Get_HitAngle() { return m_fHitAngle; }
	const int					Get_HitDir() { return m_iHitDir; }
	const float					Get_UnitSpeed() { return m_fUnitSpeed; }
	const ITEMINFO*				Get_ItemInfo() { return m_pItemInfo; }
	CGameObject*				Get_Target() { return m_pTarget; }
	const GANGSTERSTATE::State	Get_GangState() { return m_GangState; }
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
	ITEMINFO*				m_pItemInfo;
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
	int						r;
	int						g;
	int						b;
	float					m_fRotateSpeed;
	float					m_fTargetDist;
	float					m_fHitSpeed;
	float					m_fHitAngle;
	int						m_iHitDir;
	MYLINE					m_tCurLand;
	MYLINE					m_tOldLand;
	vector<int>				m_vecID;
	list<MYLINE>			m_lstRoot;
	MYLINE					m_NextLine;
	MYLINE					m_NowLine;
	GANGSTERSTATE::State	m_GangState;
	float					m_fWidthRatio;
	float					m_fHeightRatio;
	CGameObject*			m_pShooter;
};

