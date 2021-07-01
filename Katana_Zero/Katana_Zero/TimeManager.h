#pragma once
class CTimeManager
{
	DECLARE_SINGLETON(CTimeManager)
private:
	CTimeManager();
public:
	~CTimeManager();
public:
	float	Get_DeltaTime() { return m_fDeltaTime; }

public:
	void Ready_TimeManager();
	void Update_TimeManager();

private:
	LARGE_INTEGER	m_tBeginTime;
	LARGE_INTEGER	m_tEndTime;
	LARGE_INTEGER	m_tCpuTick;
	float			m_fDeltaTime;

};

