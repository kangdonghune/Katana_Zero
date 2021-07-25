#include "stdafx.h"
#include "Main.h"
#include "Graphic_Device.h"
#include "MapObjectManager.h"
#include "Texture_Manager.h"
#include "SaveLoadManager.h"
#include "GameObjectManager.h"
#include "FrameManager.h"
#include "TimeManager.h"
#include "ColliderManager.h"
#include "ScrollManager.h"
#include "SceneManager.h"
#include "UI.h"


CMain::CMain()
{
}


CMain::~CMain()
{
	Release_Main();
}

CMain * CMain::Create()
{
	CMain* pMain = new CMain();
	if (FAILED(pMain->Ready_Main()))
	{
		Safe_Delete(pMain);
		return pMain;
	}
	return pMain;
}

HRESULT CMain::Ready_Main()
{
	CSoundMgr::Get_Instance()->Initialize();
	if(FAILED(SceneManager->Ready_SceneManager()))
		return E_FAIL;
	SceneManager->Change_SceneManager(CSceneManager::SCENE_STAGE1);
	GameObjectManager->Insert_GameObjectManager(CUI::Create(), GAMEOBJECT::UI);


	return S_OK;
}

void CMain::Update_Main()
{
	SceneManager->Update_SceneManager();
}

void CMain::LateUpdate_Main()
{
	SceneManager->LateUpdate_ScnenManager();
}

void CMain::Render_Main()
{
	SceneManager->Render_SceneManager();
}

void CMain::Release_Main()
{

	SceneManager->Destroy_Instance();
	MapObjectManager->Destroy_Instance();
	GameObjectManager->Destroy_Instance();
	FrameManager->Destroy_Instance();
	SaveLoadManager->Destroy_Instance();
	TimeManager->Destroy_Instance();
	ColliderManager->Destroy_Instance();
	Device->Destroy_Instance();
	
}
