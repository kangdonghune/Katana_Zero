#include "stdafx.h"
#include "Celling.h"


CCelling::CCelling()
{
}


CCelling::~CCelling()
{
}

CMapObject * CCelling::Create(POINT start, POINT end)
{
	CCelling* pCelling = new CCelling();
	pCelling->m_Start = start;
	pCelling->m_End = end;
	return pCelling;
}

void CCelling::Update_MapObject()
{
}

void CCelling::LateUpdate_MapObject()
{
}

void CCelling::Render_MapObject()
{
}

void CCelling::Release_MapObject()
{
}
