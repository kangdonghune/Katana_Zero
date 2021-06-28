#include "stdafx.h"
#include "GameObjectManager.h"

IMPLEMENT_SINGLETON(CGameObjectManager);
CGameObjectManager::CGameObjectManager()
{
}


CGameObjectManager::~CGameObjectManager()
{
	Release_GameObjectManager();
}

void CGameObjectManager::Update_GameObjectManager()
{
}

void CGameObjectManager::LateUpdate_GameObjectManager()
{
}

void CGameObjectManager::Render_GameObjectManager()
{
}

void CGameObjectManager::Release_GameObjectManager()
{
	for (auto& pVector : m_vecGameObj)
	{
		for (auto& pGameObj : pVector)
		{
			Safe_Delete(pGameObj);
		}
		pVector.clear();
		pVector.shrink_to_fit();
	}
	m_vecGameObj->clear();
	m_vecGameObj->shrink_to_fit();
}
