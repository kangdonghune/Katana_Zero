#include "stdafx.h"
#include "Wall.h"


CWall::CWall()
{
}


CWall::~CWall()
{
}

CMapObject * CWall::Create(RECT tRect)
{
	CWall* pWall = new CWall();
	pWall->m_tRect = tRect;
	return pWall;
}

void CWall::Update_MapObject()
{
}

void CWall::LateUpdate_MapObject()
{
}

void CWall::Render_MapObject()
{
}

void CWall::Release_MapObject()
{
}
