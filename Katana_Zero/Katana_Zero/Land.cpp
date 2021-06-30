#include "stdafx.h"
#include "Land.h"


CLand::CLand()
{
}


CLand::~CLand()
{
}

CMapObject * CLand::Create(POINT start, POINT end)
{
	CLand* pLand = new CLand();
	pLand->m_Start	= start;
	pLand->m_End = end;
	return pLand;
}

void CLand::Update_MapObject()
{
}

void CLand::LateUpdate_MapObject()
{
}

void CLand::Render_MapObject()
{
}

void CLand::Release_MapObject()
{
}
