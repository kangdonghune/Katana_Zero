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
#include "Stage4.h"
#include "Stage5.h"
#include "GameObject.h"
#include "UI.h"
#include "MainScene.h"

IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
	:m_bstopFind(false)
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
			m_pScene = CMainScene::Create();
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
			m_pScene = CStage4::Create();
			break;
		case CSceneManager::SCENE_BOSS:
			m_pScene = CStage5::Create();
			break;
		case CSceneManager::SCENE_END:
			break;
		default:
			break;
		}
		m_eCurScene = m_eNextScene;
	}
}

void CSceneManager::Change_SceneManager(int SceneID)
{
	m_eNextScene = (CSceneManager::ID)SceneID;
	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eNextScene)
		{
		case CSceneManager::SCENE_LODING:
			break;
		case CSceneManager::SCENE_MENU:
			m_pScene = CMainScene::Create();
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
			m_pScene = CStage4::Create();
			break;
		case CSceneManager::SCENE_BOSS:
			m_pScene = CStage5::Create();
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
	//타임 매니저 생성
	TimeManager->Ready_TimeManager();

	//디바이스 생성
	if (FAILED(Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return E_FAIL;
	}
	//텍스쳐 삽입
	if (FAILED(Texture_Maneger->Init_Texture_Manager()))
		return E_FAIL;



	return S_OK;
}

void CSceneManager::Update_SceneManager()
{
	if (m_eCurScene == SCENE_MENU)
	{
		m_pScene->Update_Scene();
		return;
	}

	TimeManager->Update_TimeManager();
	GameObjectManager->Update_GameObjectManager();
	MapObjectManager->Update_MapObjectManager();
	m_pScene->Update_Scene();
	//플레이어와 라인충돌
	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_PassAble(MapObjectManager->Get_TerrainVector(TERRAINTYPE::PASSABLE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());
	ColliderManager->Collider_StageChange(MapObjectManager->Get_TerrainVector(TERRAINTYPE::STAGECHANGE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front());

	//갱스터들과 라인충돌
	ColliderManager->Collider_Land(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Wall(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_Celling(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_PassAbleAndEnemy(MapObjectManager->Get_TerrainVector(TERRAINTYPE::PASSABLE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));

	//보스 라인충돌
	if (!GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BOSS).empty())
	{
		ColliderManager->Collider_LandAndBoss(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BOSS).front());
		ColliderManager->Collider_WallBoss(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BOSS).front());
	}
	
	//탄환과 공격이펙트 충돌
	ColliderManager->Collider_Obb(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	
	//탄환과 유닛 충돌
	ColliderManager->Collider_BulletAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_BulletAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER));
	ColliderManager->Collider_BulletAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BOSS));

	//탄환과 벽충돌
	ColliderManager->Collider_Bullet(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	ColliderManager->Collider_Bullet(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	ColliderManager->Collider_Bullet(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET));
	
	//탄환과 투사체 충돌
	ColliderManager->Collider_BulletAndProjectile(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE));
	ColliderManager->Collider_BulletAndProjectile(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE));
	ColliderManager->Collider_BulletAndProjectile(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BULLET), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE));


	//공격이펙트와 적 충돌
	ColliderManager->ColliderAttckAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->ColliderAttckAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BOSS));

	//투사체와과 유닛충돌
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER));
	ColliderManager->Collider_ProjectileAndUnit(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER));




	//투사체와 라인충돌
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::BUTCHERKNIFE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::SMOKE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE));
	ColliderManager->Collider_Projectile(MapObjectManager->Get_TerrainVector(TERRAINTYPE::CELLING), GameObjectManager->Get_GameObjectlist(GAMEOBJECT::EXPLOSIVE));


	if (GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front()->Get_CurLine().ID == GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front()->Get_OldLine().ID)
		m_bstopFind = false;
	if (GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front()->Get_CurLine().ID != GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYER).front()->Get_OldLine().ID)
		m_bstopFind = true;
	for (auto& pGang : GameObjectManager->Get_GameObjectlist(GAMEOBJECT::GANGSTER))
	{
		if(m_bstopFind)//탐색이 됐다면.
		{
			if (pGang->Get_TargetDist() > 800.f)
				continue;
			if(pGang->Get_ObjState() != DOWN)
				pGang->Find_Root(pGang->Get_CurLine(), pGang->Get_Target()->Get_CurLine());
		}
	}
	
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
