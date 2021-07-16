#include "stdafx.h"
#include "SceneManager.h"


CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::Change_SceneManager(ID eNextScene)
{
	m_eNextScene = eNextScene;
	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene);
	
		switch (m_eNextScene)
		{
		case CSceneManager::SCENE_LODING:
			break;
		case CSceneManager::SCENE_MENU:
			break;
		case CSceneManager::SCENE_EDIT:
			break;
		case CSceneManager::SCENE_STAGE1:
			break;
		case CSceneManager::SCENE_STAGE2:
			break;
		case CSceneManager::SCENE_STAGE3:
			break;
		case CSceneManager::SCENE_STAGE4:
			break;
		case CSceneManager::SCENE_BOSS:
			break;
		case CSceneManager::SCENE_END:
			break;
		default:
			break;
		}
		m_eCurScene = m_eNextScene;
	}
}

void CSceneManager::Update_SceneManager()
{
}
