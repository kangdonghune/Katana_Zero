#include "stdafx.h"
#include "MapObjectManager.h"
#include "ScrollManager.h"


IMPLEMENT_SINGLETON(CMapObjectManager)
CMapObjectManager::CMapObjectManager()
{
}


CMapObjectManager::~CMapObjectManager()
{
}

void CMapObjectManager::Load_Terrain(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	Release_MapObjectManager();
	DWORD dwByte = 0;
	MYLINE tLine;
	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(MYLINE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		switch (tLine.type)
		{
		case TERRAINTYPE::LAND:
			m_vecTerrain[TERRAINTYPE::LAND].emplace_back(tLine);
			break;
		case TERRAINTYPE::WALL:
			m_vecTerrain[TERRAINTYPE::WALL].emplace_back(tLine);
			break;
		case TERRAINTYPE::CELLING:
			m_vecTerrain[TERRAINTYPE::CELLING].emplace_back(tLine);
			break;
		case TERRAINTYPE::PASSABLE:
			m_vecTerrain[TERRAINTYPE::PASSABLE].emplace_back(tLine);
			break;
		case TERRAINTYPE::STAGECHANGE:
			m_vecTerrain[TERRAINTYPE::STAGECHANGE].emplace_back(tLine);
			break;
		default:
			break;
		}
	}
	CloseHandle(hFile);
	//MessageBox(nullptr, L"불러오기 완료", L"불러오기 시스템", MB_OK);
}

HRESULT CMapObjectManager::Init_MapObjectManager()
{
	Load_Terrain(L"../Data/Stage1/Terrain/Terrain.dat");
	return S_OK;
}

void CMapObjectManager::Update_MapObjectManager()
{
	//for (auto& tLinevec : m_vecTerrain)
	//{
	//	for (auto& tLine : tLinevec)
	//	{
	//		tLine.Start.x = tLine.Start.x - CScrollManager::Get_ScroolX();
	//		tLine.Start.y = tLine.Start.y - CScrollManager::Get_ScroolY();
	//		tLine.End.x = tLine.End.x - CScrollManager::Get_ScroolX();
	//		tLine.End.y = tLine.End.y - CScrollManager::Get_ScroolY();
	
	//	}
	//}
}

void CMapObjectManager::LateUpdate_MapObjectManager()
{
}

void CMapObjectManager::Render_MapObjectManager()
{
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(5.f);
	for (auto& tLine : m_vecTerrain[TERRAINTYPE::WALL])
	{
		D3DXVECTOR2	vLine[2]{ { float(tLine.Start.x - CScrollManager::Get_ScroolX()),float(tLine.Start.y - CScrollManager::Get_ScroolY()) },{ float(tLine.End.x- CScrollManager::Get_ScroolX()),float(tLine.End.y - CScrollManager::Get_ScroolY()) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

	}

	for (auto& tLine : m_vecTerrain[TERRAINTYPE::LAND])
	{
		D3DXVECTOR2	vLine[2]{ { float(tLine.Start.x - CScrollManager::Get_ScroolX()),float(tLine.Start.y - CScrollManager::Get_ScroolY()) },{ float(tLine.End.x - CScrollManager::Get_ScroolX()),float(tLine.End.y - CScrollManager::Get_ScroolY()) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	for (auto& tLine : m_vecTerrain[TERRAINTYPE::CELLING])
	{
		D3DXVECTOR2	vLine[2]{ { float(tLine.Start.x - CScrollManager::Get_ScroolX()),float(tLine.Start.y - CScrollManager::Get_ScroolY()) },{ float(tLine.End.x - CScrollManager::Get_ScroolX()),float(tLine.End.y - CScrollManager::Get_ScroolY()) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 0, 0, 255));
	}
	
	for (auto& tLine : m_vecTerrain[TERRAINTYPE::PASSABLE])
	{
		D3DXVECTOR2	vLine[2]{ { float(tLine.Start.x - CScrollManager::Get_ScroolX()),float(tLine.Start.y - CScrollManager::Get_ScroolY()) },{ float(tLine.End.x - CScrollManager::Get_ScroolX()),float(tLine.End.y - CScrollManager::Get_ScroolY()) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 0, 255));
	}

	for (auto& tLine : m_vecTerrain[TERRAINTYPE::STAGECHANGE])
	{
		D3DXVECTOR2	vLine[2]{ { float(tLine.Start.x - CScrollManager::Get_ScroolX()),float(tLine.Start.y - CScrollManager::Get_ScroolY()) },{ float(tLine.End.x - CScrollManager::Get_ScroolX()),float(tLine.End.y - CScrollManager::Get_ScroolY()) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMapObjectManager::Release_MapObjectManager()
{
	for (auto& pTerrainvec : m_vecTerrain)
	{
		pTerrainvec.clear();
		pTerrainvec.shrink_to_fit();
	}
	m_vecTerrain->clear();
	m_vecTerrain->shrink_to_fit();
}
