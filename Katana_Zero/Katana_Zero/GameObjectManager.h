#pragma once
class CGameObjectManager;
class CGameObjectManager
{
	DECLARE_SINGLETON(CGameObjectManager)
private:
	CGameObjectManager();
public:
	~CGameObjectManager();

public:
	void Update_GameObjectManager();
	void LateUpdate_GameObjectManager();
	void Render_GameObjectManager();
	void Release_GameObjectManager();

private:
	vector<CGameObjectManager*> m_vecGameObj[GAMEOBJECT::END];
};

