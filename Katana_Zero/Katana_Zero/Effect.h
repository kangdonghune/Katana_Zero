#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
private:
	CEffect();
public:
	virtual ~CEffect();
public:
	static CGameObject* Create(CGameObject* pTarget, TCHAR* wstrStateKey);

private:
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual void Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	CGameObject* m_pTarget; // �������׼� �߻��ϴ� ����Ʈ������
};

