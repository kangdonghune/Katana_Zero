#include "stdafx.h"
#include "Main.h"


CMain::CMain()
{
}


CMain::~CMain()
{
	Release_Main();
}

CMain * CMain::Create()
{
	CMain* pMain = new CMain();
	if (FAILED(pMain->Ready_Main()))
	{
		Safe_Delete(pMain);
	}
	return pMain;
}

HRESULT CMain::Ready_Main()
{
	return S_OK;
}

void CMain::Update_Main()
{
}

void CMain::LateUpdate_Main()
{
}

void CMain::Render_Main()
{
}

void CMain::Release_Main()
{
}
