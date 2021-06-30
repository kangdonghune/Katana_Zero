#pragma once
#include "MapObject.h"
class CWall :
	public CMapObject
{
private:
	CWall();
public:
	~CWall();
public:
	static CMapObject* Create(RECT tRect);

public:
	// CMapObject을(를) 통해 상속됨
	virtual void Update_MapObject() override;
	virtual void LateUpdate_MapObject() override;
	virtual void Render_MapObject() override;
	virtual void Release_MapObject() override;

private:
	RECT	m_tRect;
};

