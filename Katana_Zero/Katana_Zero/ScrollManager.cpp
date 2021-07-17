#include "stdafx.h"
#include "ScrollManager.h"
#include "SceneManager.h"
#include "Scene.h"

float CScrollManager::m_fScroolX = 0.f;
float CScrollManager::m_fScroolY = 0.f;

CScrollManager::CScrollManager()

{
}


CScrollManager::~CScrollManager()
{
}

void CScrollManager::ScrollLock()
{
	if (m_fScroolX < 0)
		m_fScroolX = 0;

	if (m_fScroolY < 0)
		m_fScroolY = 0;

	if (m_fScroolX > SceneManager->GetScene()->Get_MapWidth() - WINCX)
		m_fScroolX = SceneManager->GetScene()->Get_MapWidth() - WINCX;
	if (m_fScroolY > SceneManager->GetScene()->Get_MapHeight() - WINCY)
		m_fScroolY = SceneManager->GetScene()->Get_MapHeight() - WINCY;
}
