#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameObject.h"

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
	for (auto& pVector : m_vecGameObj)
	{
		for (auto& pUnit : pVector)
		{
			pUnit->Update_GameObject();
		}
	}
}

void CGameObjectManager::LateUpdate_GameObjectManager()
{
}

void CGameObjectManager::Render_GameObjectManager()
{
	for (auto& pVector : m_vecGameObj)
	{
		for(auto& pUnit : pVector)
		{
			pUnit->Render_GameObject();
		}
	}
}

void CGameObjectManager::Release_GameObjectManager()
{
	for (auto& pVector : m_vecGameObj)
	{
		for (auto& pGameObj : pVector)
		{
			pGameObj->~CGameObject();
			Safe_Delete(pGameObj);
		}
		pVector.clear();
		pVector.shrink_to_fit();
	}
	m_vecGameObj->clear();
	m_vecGameObj->shrink_to_fit();
}
