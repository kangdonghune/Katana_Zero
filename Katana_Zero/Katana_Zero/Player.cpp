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

HRESULT CPlayer::Load_Player(TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
//	UNITS->Release_Unit();
	DWORD dwByte = 0;
	DWORD dwKeyCount = 0;
	DWORD dwStateCount = 0;
	TCHAR* szKeyBuf = nullptr;
	TCHAR* szStateBuf = nullptr;
	while (true)
	{
		UNITINFO* pUnit = new UNITINFO;
		ReadFile(hFile, &dwKeyCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		ReadFile(hFile, &dwStateCount, sizeof(DWORD), &dwByte, nullptr);
		szKeyBuf = new TCHAR[dwKeyCount];
		szStateBuf = new TCHAR[dwStateCount];
		ReadFile(hFile, szKeyBuf, dwKeyCount, &dwByte, nullptr);
		ReadFile(hFile, szStateBuf, dwStateCount, &dwByte, nullptr);
		pUnit->wstrKey = szKeyBuf;
		pUnit->wstrState = szStateBuf;
		Safe_Delete_Array(szKeyBuf);
		Safe_Delete_Array(szStateBuf);
		ReadFile(hFile, &pUnit->D3VecPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pUnit->type, sizeof(UNITTYPE::TYPE), &dwByte, nullptr);
		switch (pUnit->type)
		{
		case UNITTYPE::PLAYER:
			//UNITS->Insert_Unit(pUnit, pUnit->type);
			break;
		default:
			Safe_Delete(pUnit);
			break;
		}
	}
	CloseHandle(hFile);
	return E_NOTIMPL;
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
