#pragma once
class CScene;
class CSceneManager
{
public:
	enum ID { SCENE_LODING, SCENE_MENU, SCENE_EDIT, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3, SCENE_STAGE4,SCENE_BOSS, SCENE_END };

	DECLARE_SINGLETON(CSceneManager)
private:
	CSceneManager();
	~CSceneManager();

public:
	void Change_SceneManager(ID eNextScene);
	void Change_SceneManager(int SceneID);
	CScene* GetScene() { return m_pScene; }
	ID	Get_CurScene() { return m_eCurScene; }
public:

public:
	HRESULT Ready_SceneManager();
	void Update_SceneManager();
	void Render_SceneManager();
	void LateUpdate_ScnenManager();
	void Release_SceneManager();
private:
	CScene* m_pScene;
	ID		m_eCurScene;
	ID		m_eNextScene;
	CScene* m_pOldScene;
};

