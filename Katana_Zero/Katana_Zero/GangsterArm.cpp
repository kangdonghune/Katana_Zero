#include "stdafx.h"
#include "GangsterArm.h"


CGangsterArm::CGangsterArm()
{

}


CGangsterArm::~CGangsterArm()
{
}

CGameObject * CGangsterArm::Create(CGameObject * pTarget)
{
	CGangsterArm* pArm = new CGangsterArm();
	pArm->Set_Target(pTarget);
	if (FAILED(pArm->Ready_GameObject()))
	{
		delete pArm;
	}
	return nullptr;
}

HRESULT CGangsterArm::Ready_GameObject()
{
	return E_NOTIMPL;
}

void CGangsterArm::Update_GameObject()
{
}

void CGangsterArm::LateUpdate_GameObject()
{
}

void CGangsterArm::Render_GameObject()
{
}

void CGangsterArm::Release_GameObject()
{
}
