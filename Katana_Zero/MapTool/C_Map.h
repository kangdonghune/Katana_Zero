#pragma once
class CMapToolView;
class C_Map
{

public:
	enum Stages {Stage1, Stage2, Stage3, Stage4, END};

public:
	C_Map();
	~C_Map();

public:
	HRESULT Ready_Map();
	void Update_Map();
	void Render_Map();
	void Release_Map();

public:
	void ChangeMap(Stages StageNum);
	void SetView(CMapToolView* pView) { m_pView = pView; }
private:
	Stages m_NowStage;
	CMapToolView* m_pView;
};

