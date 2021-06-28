#pragma once
class CRECT
{

	DECLARE_SINGLETON(CRECT)

private:
	CRECT();
	~CRECT();

public:
	vector<MYRECT*>& Get_Rects() { return m_vecRect; }
	vector<MYLINE*>& Get_Land() { return m_vecLine[LINETYPE::LAND]; }
	vector<MYLINE*>& Get_Celling() { return m_vecLine[LINETYPE::CELLING]; }
public:

	void Insert_Rect(D3DXVECTOR3 LeftTop, D3DXVECTOR3 RightBottom, TERRAINTYPE::TYPE type);
	void Insert_Line(D3DXVECTOR3 start, D3DXVECTOR3 end, TERRAINTYPE::TYPE type);
	void Release_Rect();

private:
	//RECT, 타입. 위치는 필요없나.
	vector<MYRECT*> m_vecRect;
	vector<MYLINE*> m_vecLine[LINETYPE::END];

};

