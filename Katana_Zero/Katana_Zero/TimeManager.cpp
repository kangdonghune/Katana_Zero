#include "stdafx.h"
#include "TimeManager.h"

IMPLEMENT_SINGLETON(CTimeManager)
CTimeManager::CTimeManager()
	:m_fDeltaTime(0.f)
	,m_tBeginTime({})
{
	ZeroMemory(&m_tEndTime, sizeof(m_tEndTime));
	ZeroMemory(&m_tCpuTick, sizeof(LARGE_INTEGER));
}


CTimeManager::~CTimeManager()
{
}

void CTimeManager::Ready_TimeManager()
{
	QueryPerformanceFrequency(&m_tCpuTick);
	QueryPerformanceCounter(&m_tBeginTime);
	QueryPerformanceCounter(&m_tEndTime);
}

void CTimeManager::Update_TimeManager()
{
	QueryPerformanceCounter(&m_tEndTime);
	QueryPerformanceFrequency(&m_tCpuTick);

	m_fDeltaTime = float(m_tEndTime.QuadPart - m_tBeginTime.QuadPart) / m_tCpuTick.QuadPart; //델타 타임은 시작과 끝 기준의 시간 차를 1초로 나눴다고 생각하면 된다.
	m_tBeginTime = m_tEndTime;
}
