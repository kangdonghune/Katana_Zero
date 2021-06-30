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





void CRECT::Insert_Line(D3DXVECTOR3 start, D3DXVECTOR3 end, TERRAINTYPE::TYPE type)
{
	MYLINE* pLine = new MYLINE;
	pLine->Start.x = start.x;
	pLine->Start.y = start.y;
	pLine->End.x = end.x;
	pLine->End.y = end.y;
	pLine->type = type;


	switch (type)
	{
	case TERRAINTYPE::LAND:
		m_vecLine[TERRAINTYPE::LAND].emplace_back(pLine);
		break;
	case TERRAINTYPE::WALL:
		m_vecLine[TERRAINTYPE::WALL].emplace_back(pLine);
		break;
	case TERRAINTYPE::CELLING:
		m_vecLine[TERRAINTYPE::CELLING].emplace_back(pLine);
		break;
	default:
		break;
	}
	
}

void CRECT::Insert_Line(POINT start, POINT end, TERRAINTYPE::TYPE type)
{
	MYLINE* pLine = new MYLINE;
	pLine->Start.x = start.x;
	pLine->Start.y = start.y;
	pLine->End.x = end.x;
	pLine->End.y = end.y;
	pLine->type = type;


	switch (type)
	{
	case TERRAINTYPE::LAND:
		m_vecLine[TERRAINTYPE::LAND].emplace_back(pLine);
		break;
	case TERRAINTYPE::WALL:
		m_vecLine[TERRAINTYPE::WALL].emplace_back(pLine);
		break;
	case TERRAINTYPE::CELLING:
		m_vecLine[TERRAINTYPE::CELLING].emplace_back(pLine);
		break;
	default:
		break;
	}
}

void CRECT::Release_Rect()
{

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
