#include "stdafx.h"
#include "Scene.h"
#include "MapObjectManager.h"
#include "GameObjectManager.h"
#include "FrameManager.h"
CScene::CScene()
	:m_fMapHeight(0.f)
	,m_fMapWidth(0.f)
{
}


CScene::~CScene()
{
}

HRESULT CScene::Ready_Scene()
{
	return E_NOTIMPL;
}

void CScene::Update_Scene()
{
}

void CScene::Render_Scene()
{
	MapObjectManager->Render_MapObjectManager();
	GameObjectManager->Render_GameObjectManager();
	FrameManager->Render_Frame_Manager();
}

void CScene::Release_Scene()
{
}
