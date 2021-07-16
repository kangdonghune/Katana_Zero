#include "stdafx.h"
#include "ScrollManager.h"
#include "Texture_Manager.h"

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

	if (m_fScroolX > Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 0)->tImageInfo.Width - WINCX)
		m_fScroolX = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 0)->tImageInfo.Width - WINCX;
	if (m_fScroolY > Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 0)->tImageInfo.Height - WINCY)
		m_fScroolY = Texture_Maneger->Get_TexInfo_Manager(L"Map", L"Stage", 0)->tImageInfo.Height - WINCY;
}
