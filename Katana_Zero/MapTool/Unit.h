#pragma once
class CMapToolView;
class CUnit
{
	DECLARE_SINGLETON(CUnit)
private:
	CUnit();
public:
	~CUnit();

public:
	vector<UNITINFO*>& Get_UnitVec(int index) { return m_vecUnit[index]; }

public:
	void Insert_Unit(UNITINFO* pUnitInfo, UNITTYPE::TYPE type) { m_vecUnit[type].emplace_back(pUnitInfo); }
	void Render_Unit();
	void Release_Unit();
private:
	vector<UNITINFO*> m_vecUnit[UNITTYPE::END];
	CMapToolView* m_pView;
};

