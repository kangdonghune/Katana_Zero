#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release_GameObject();
}

CPlayer * CPlayer::Create()
{
	CPlayer* pPlayer = new CPlayer();
	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Delete(pPlayer);
		return pPlayer;
	}
	return pPlayer;
}

HRESULT CPlayer::Ready_GameObject()
{
	return S_OK;
}

void CPlayer::Update_GameObject()
{
}

void CPlayer::LateUpdate_GameObject()
{
}

void CPlayer::Render_GameObject()
{
}

void CPlayer::Release_GameObject()
{
}
