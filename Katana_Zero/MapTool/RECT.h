#pragma once
class CRECT
{

	DECLARE_SINGLETON(CRECT)

private:
	CRECT();
	~CRECT();

public:

	vector<MYLINE*>& Get_Wall() { return m_vecLine[TERRAINTYPE::WALL]; }
	vector<MYLINE*>& Get_Land() { return m_vecLine[TERRAINTYPE::LAND]; }
	vector<MYLINE*>& Get_Celling() { return m_vecLine[TERRAINTYPE::CELLING]; }
	vector<MYLINE*>& Get_Line(int index) { return m_vecLine[index]; }
public:

	void Insert_Line(D3DXVECTOR3 start, D3DXVECTOR3 end, TERRAINTYPE::TYPE type);
	void Insert_Line(POINT start, POINT end, TERRAINTYPE::TYPE type);
	void Release_Rect();

private:
	//RECT, 타입. 위치는 필요없나.
	vector<MYLINE*> m_vecLine[TERRAINTYPE::END];

};

