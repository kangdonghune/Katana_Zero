#include "stdafx.h"
#include "FrameManager.h"
#include "TimeManager.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CFrameManager)
CFrameManager::CFrameManager()
	:m_szFPS(L"")
	,m_szFrame(L"")
	,m_szFrameName(L"")
	, m_fFPSTime(0.f)
	, m_iFPS(0)
	, m_fSPF(0.f)
{
}


CFrameManager::~CFrameManager()
{
}

void CFrameManager::Ready_Frame_Manager(float fFPS) //현재는 초당 60프레임.
{
	m_fSPF = 1.f / fFPS; //0.166 초마다 1프레임. 
	m_fDeltaTime = 0.f;
	QueryPerformanceCounter(&m_tBeginTime);
	QueryPerformanceCounter(&m_tEndTime);
	QueryPerformanceFrequency(&m_tCpuTick);
}

void CFrameManager::Render_Frame_Manager()
{
	m_fFPSTime += TimeManager->Get_DeltaTime();
	if (1.f <= m_fFPSTime)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_fFPSTime = 0.f;
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 100.f, 0.f);

	Device->Get_Sprite()->SetTransform(&matTrans);
	Device->Get_Font()->DrawTextW(Device->Get_Sprite(), m_szFPS, lstrlen(m_szFPS), nullptr, 0, D3DCOLOR_ARGB(255, 255,255, 255));
}

void CFrameManager::Render_Frame_Manager_FrameNum(size_t fFrameStart)
{

	swprintf_s(m_szFrame, L"FrameNum : %d", fFrameStart);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 130.f, 0.f);

	Device->Get_Sprite()->SetTransform(&matTrans);
	Device->Get_Font()->DrawTextW(Device->Get_Sprite(), m_szFrame, lstrlen(m_szFrame), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CFrameManager::Render_Frame_Manager_FrameName(wstring wstrStateKey)
{
	swprintf_s(m_szFrameName, L"State : %s", wstrStateKey.c_str());

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 150.f, 0.f);

	Device->Get_Sprite()->SetTransform(&matTrans);
	Device->Get_Font()->DrawTextW(Device->Get_Sprite(), m_szFrameName, lstrlen(m_szFrameName), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool CFrameManager::Frame_Lock()
{
	QueryPerformanceCounter(&m_tEndTime);
	m_fDeltaTime += float(m_tEndTime.QuadPart - m_tBeginTime.QuadPart) / m_tCpuTick.QuadPart;
	m_tBeginTime = m_tEndTime;
	if (m_fSPF <= m_fDeltaTime) //프레임 한 번 도는데 걸리는 시간보다 델타 타임(cpu의 지난 시간)가 커지면 1프레임 증가 시키기.
	{
		QueryPerformanceFrequency(&m_tCpuTick);
		m_fDeltaTime = 0.f;
		++m_iFPS;
		return true;	
	}
	return false;
}
