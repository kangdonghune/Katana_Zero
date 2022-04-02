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
	virtual HRESULT Ready_Scene();
	virtual void Update_Scene();
	virtual void Render_Scene();
	virtual void Release_Scene();

protected:
	float m_fMapWidth;
	float m_fMapHeight;
};

