#pragma once
#include "MapObject.h"
class CLand :
	public CMapObject
{
private:
	CLand();
public:
	~CLand();

public:
	static CMapObject* Create(POINT start, POINT end);

public:
	// CMapObject을(를) 통해 상속됨
	virtual void Update_MapObject() override;
	virtual void LateUpdate_MapObject() override;
	virtual void Render_MapObject() override;
	virtual void Release_MapObject() override;

private:
	POINT	m_Start;
	POINT	m_End;
};

