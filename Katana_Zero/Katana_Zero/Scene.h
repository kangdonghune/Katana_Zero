#pragma once
class CScene
{
public:
	explicit CScene();
	virtual ~CScene();

public:
	float	Get_MapWidth() { return m_fMapWidth; }
	float	Get_MapHeight() { return m_fMapHeight; }

public:
	virtual HRESULT Ready_Scene()PURE;
	virtual void Update_Scene() PURE;
	virtual void Render_Scene()PURE;
	virtual void Release_Scene() PURE;

protected:
	float m_fMapWidth;
	float m_fMapHeight;
};

