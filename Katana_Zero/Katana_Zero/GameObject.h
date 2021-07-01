#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	void Set_Info(UNITINFO* pInfo) { m_pUnitInfo = pInfo; }
	void Update_HitBox();
	void FrameMove(float fSpeed);
	void Render_HitBox();
	void Update_Frame();
	bool Check_FrameEnd();

	const UNITINFO* Get_UnitInfo() { return m_pUnitInfo; }
	const float		Get_UnitRatio() { return m_fRatio; }

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual void	Update_GameObject() PURE;
	virtual void	LateUpdate_GameObject() PURE;
	virtual void	Render_GameObject()	PURE;
	virtual	void	Release_GameObject() PURE;
	
protected:
	UNITINFO*			m_pUnitInfo;
	PLAYERSTATE::State	m_State;
	RECT				m_tHitBox;
	FRAME				m_tFrame;
	float				m_fSpeed;
	float				m_fRatio;
	wstring				m_wstrOldState;
	wstring				m_wstrCurState;
	int					m_iUnitDir;// y�� ȸ�� �� ���� ������ �ٶ󺸴� ������ ����.
};

