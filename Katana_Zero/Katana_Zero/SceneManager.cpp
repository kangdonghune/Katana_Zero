#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeManager.h"
#include "GameObjectManager.h"
#include "MapObjectManager.h"
#include "ColliderManager.h"
#include "SaveLoadManager.h"
#include "Texture_Manager.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"

IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
	Release_SceneManager();
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
			m_pScene = CStage1::Create();
			break;
		case CSceneManager::SCENE_STAGE2:
			m_pScene = CStage2::Create();
			break;
		case CSceneManager::SCENE_STAGE3:
			m_pScene = CStage3::Create();
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

HRESULT CSceneManager::Ready_SceneManager()
{
	//Ÿ�� �Ŵ��� ����
	TimeManager->Ready_TimeManager();

	//����̽� ����
	if (FAILED(Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return E_FAIL;
	}
	//�ؽ��� ����
	if (FAILED(Texture_Maneger->Init_Texture_Manager()))
		return E_FAIL;


	return S_OK;
}

void CSceneManager::Update_SceneManager()
{

	TimeManager->Update_TimeManager();
	GameObjectManager->Update_GameObjectManager();
	MapObjectManager->Update_MapObjectManager();
	m_pScene->Update_Scene();
	//�÷��̾�� �����浹
	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());

	//�����͵�� �����浹
	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));

	//źȯ�� ��������Ʈ �浹
	ColliderManager->Collider_Obb(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	//źȯ�� �����浹
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER));

	//����ü�� �����浹
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));

}

void CSceneManager::Render_SceneManager()
{
	m_pScene->Render_Scene();
}

void CSceneManager::LateUpdate_ScnenManager()
{
}

void CSceneManager::Release_SceneManager()
{
	Safe_Delete(m_pScene);
}
