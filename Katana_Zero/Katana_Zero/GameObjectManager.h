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
	void Insert_GameObjectManager(CGameObject* pObject,GAMEOBJECT::TYPE type) { m_vecGameObj[type].emplace_back(pObject); }

public:
	void Update_GameObjectManager();
	void LateUpdate_GameObjectManager();
	void Render_GameObjectManager();
	void Release_GameObjectManager();

private:
	vector<CGameObject*> m_vecGameObj[GAMEOBJECT::END];
};

