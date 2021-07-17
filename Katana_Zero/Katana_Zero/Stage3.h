#pragma once
#include "Scene.h"
class CStage3 :
	public CScene
{
private:
	CStage3();
public:
	virtual ~CStage3();

public:
	static CScene* Create();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

