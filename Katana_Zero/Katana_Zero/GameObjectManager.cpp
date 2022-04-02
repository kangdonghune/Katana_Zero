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
	for (int i = 0; i < GAMEOBJECT::END; i++)
	{
		for (auto& iter = m_lstGameObj[i].begin(); iter != m_lstGameObj[i].end(); )
		{
			if ((*iter)->Get_ObjState() == DEAD)
			{
				Safe_Delete(*iter);
				iter = m_lstGameObj[i].erase(iter);
			}
			else
			{
				(*iter)->Update_GameObject();
				++iter;
			}
		}
	}
}

void CGameObjectManager::LateUpdate_GameObjectManager()
{

	for (int i = 0; i < GAMEOBJECT::END; i++)
	{
		for (auto& iter = m_lstGameObj[i].begin(); iter != m_lstGameObj[i].end(); )
		{
			if ((*iter)->Get_ObjState() == DEAD)
			{
				Safe_Delete(*iter);
				iter = m_lstGameObj[i].erase(iter);
			}
			else
			{
				(*iter)->LateUpdate_GameObject();
				++iter;
			}
		}
	}
}

void CGameObjectManager::Render_GameObjectManager()
{
	for (auto& pVector : m_lstGameObj)
	{
		for(auto& pUnit : pVector)
		{
			pUnit->Render_GameObject();
			pUnit->Render_HitBox();
			pUnit->Render_HitBox();
			pUnit->Render_ObbLine();
			pUnit->Render_HitBoxObb();
			pUnit->Render_ObbLineD3D();
		}
	}
}

void CGameObjectManager::Release_GameObjectManager()
{
	for (auto& pVector : m_lstGameObj)
	{
		for (auto& pGameObj : pVector)
		{
			Safe_Delete(pGameObj);
		}
		pVector.clear();
	}
	m_lstGameObj->clear();
}
