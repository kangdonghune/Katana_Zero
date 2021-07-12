#pragma once
#include "GameObject.h"

class CGameObjectManager
{
	DECLARE_SINGLETON(CGameObjectManager)
private:
	CGameObjectManager();
public:
	~CGameObjectManager();

public:
	void Insert_GameObjectManager(CGameObject* pObject,GAMEOBJECT::TYPE type) { m_lstGameObj[type].emplace_back(pObject); }
	list<CGameObject*>& Get_GameObjectlist(GAMEOBJECT::TYPE type) { return m_lstGameObj[type]; }
	list<CGameObject*>& Get_DeleteGameObjectlist(GAMEOBJECT::TYPE type) { return m_lstGameObj[type]; }

public:
	void Update_GameObjectManager();
	void LateUpdate_GameObjectManager();
	void Render_GameObjectManager();
	void Release_GameObjectManager();

private:
	list<CGameObject*> m_lstGameObj[GAMEOBJECT::END];
};

