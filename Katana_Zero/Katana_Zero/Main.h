#pragma once
class CMain
{
private:
	CMain();
public:
	~CMain();
public:
	static CMain* Create();

public:
	HRESULT Ready_Main();
	void Update_Main();
	void LateUpdate_Main();
	void Render_Main();
	void Release_Main();

private:

};

