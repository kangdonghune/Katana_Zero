#pragma once
class CSaveLoadManager
{
	DECLARE_SINGLETON(CSaveLoadManager)
private:
	CSaveLoadManager();
public:
	~CSaveLoadManager();

public:
	HRESULT	LoadUnit(TCHAR* pFilePath);
};

