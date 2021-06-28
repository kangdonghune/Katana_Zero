#include "stdafx.h"
#include "RECT.h"

	IMPLEMENT_SINGLETON(CRECT)

CRECT::CRECT()
{
}


CRECT::~CRECT()
{
	Release_Rect();
}



void CRECT::Insert_Rect(D3DXVECTOR3 LeftTop, D3DXVECTOR3 RightBottom, TERRAINTYPE::TYPE type)
{
	MYRECT* pRect = new MYRECT;
	pRect->rc.left = LeftTop.x;
	pRect->rc.top = LeftTop.y;
	pRect->rc.right = RightBottom.x;
	pRect->rc.bottom = RightBottom.y;
	pRect->type	= type;

	m_vecRect.emplace_back(pRect);
}

void CRECT::Insert_Line(D3DXVECTOR3 start, D3DXVECTOR3 end, TERRAINTYPE::TYPE type)
{
	MYLINE* pLine = new MYLINE;
	pLine->Start.x = start.x;
	pLine->Start.y = start.y;
	pLine->End.x = end.x;
	pLine->End.y = end.y;
	pLine->type = type;
	if (type == TERRAINTYPE::LAND)
	{
		m_vecLine[LINETYPE::LAND].emplace_back(pLine);
	}
	if (type == TERRAINTYPE::CELLING)
	{
		m_vecLine[LINETYPE::CELLING].emplace_back(pLine);
	}
	
}

void CRECT::Release_Rect()
{
	for (auto& pRect : m_vecRect)
	{
		Safe_Delete(pRect);
	}
	m_vecRect.clear();
	m_vecRect.shrink_to_fit();

	for (auto& pLineVec : m_vecLine)
	{
		for (auto& pLine : pLineVec)
		{
			Safe_Delete(pLine);
		}
		pLineVec.clear();
		pLineVec.shrink_to_fit();
	}
	

}
