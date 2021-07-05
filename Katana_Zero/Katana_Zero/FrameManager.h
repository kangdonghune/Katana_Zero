#pragma once
class CFrameManager
{
	DECLARE_SINGLETON(CFrameManager)
public:
	CFrameManager();
	~CFrameManager();

public:
	void	Ready_Frame_Manager(float fFPS);
	void	Render_Frame_Manager();
	void	Render_Frame_Manager_FrameNum(size_t fFrameStart);
	void	Render_Frame_Manager_FrameName(wstring wstrStateKey);
	bool	Frame_Lock();
	const float	Get_SPF() { return m_fSPF; }

private:
	LARGE_INTEGER	m_tBeginTime;
	LARGE_INTEGER	m_tEndTime;
	LARGE_INTEGER	m_tCpuTick;
	float			m_fDeltaTime;
	float			m_fSPF;

	TCHAR			m_szFPS[32];
	TCHAR			m_szFrame[32];
	TCHAR			m_szFrameName[32];
	float			m_fFPSTime;
	int				m_iFPS;
};

