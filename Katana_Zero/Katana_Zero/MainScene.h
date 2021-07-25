#pragma once
#include "Scene.h"
class CMainScene :
	public CScene
{

private:
	CMainScene();
public:
	virtual ~CMainScene();

public:
	static CScene* Create();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	float	fScrool;
	int		iPlantCount;
	bool	reverse;
};

