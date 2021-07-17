#pragma once
#include "Scene.h"
class CStage2 :
	public CScene
{
private:
	CStage2();
public:
	virtual ~CStage2();

public:
	static CScene* Create();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};