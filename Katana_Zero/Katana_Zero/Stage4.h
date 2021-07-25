#pragma once
#include "Scene.h"
class CStage4 :
	public CScene
{
private:
	CStage4();
public:
	virtual ~CStage4();

public:
	static CScene* Create();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	int	MapNum;
};
