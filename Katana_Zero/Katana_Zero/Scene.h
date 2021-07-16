#pragma once
class CScene
{
public:
	explicit CScene();
	virtual ~CScene();

public:
	virtual HRESULT Ready_Scene()PURE;
	virtual void Update_Scene() PURE;
	virtual void Render_Scene(HDC hDC)PURE;
	virtual void Release_Scene() PURE;
};

