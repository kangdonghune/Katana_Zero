#pragma once
class CScrollManager
{
public:
	CScrollManager();
	~CScrollManager();

public:
	static float Get_ScroolX() { return m_fScroolX; }
	static float Get_ScroolY() { return m_fScroolY; }
	static void Set_ScroolX(float x) { m_fScroolX = x; }
	static void Set_ScroolY(float y) { m_fScroolY = y; }

	static void ScrollLock();

private:
	static float m_fScroolX;
	static float m_fScroolY;
};

