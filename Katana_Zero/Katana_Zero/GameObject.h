#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	UNITINFO*&	Set_Info() { return m_pUnitInfo; }
	void Set_Info(UNITINFO* pInfo) { m_pUnitInfo = pInfo; }
	void Set_PivotY(float fPos) { m_vecPivot.y = fPos; }
	

	void Update_HitBox();
	void FrameMove(float fSpeed);
	void Render_HitBox();
	void Update_Frame();
	bool Check_FrameEnd();


	const D3DXVECTOR3 Get_Pivot() { return m_vecPivot; }
	const float		Get_Ratio() { return m_fRatio; }
	const RECT		Get_Hitbox() { return m_tHitBox; }
	const UNITINFO* Get_UnitInfo() { return m_pUnitInfo; }
	const float		Get_UnitRatio() { return m_fRatio; }

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
	FRAME					m_tFrame;
	float					m_fSpeed;
	float					m_fRatio;
	wstring					m_wstrOldState;
	wstring					m_wstrCurState;
	int						m_iUnitDir;// y축 회전 시 적용 값으로 바라보는 방향을 결정.
	D3DXVECTOR3				m_vecPivot;

};

