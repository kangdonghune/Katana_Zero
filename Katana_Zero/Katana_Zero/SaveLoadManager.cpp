#include "stdafx.h"
#include "SaveLoadManager.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Gangster.h"

IMPLEMENT_SINGLETON(CSaveLoadManager)
CSaveLoadManager::CSaveLoadManager()
{
}


CSaveLoadManager::~CSaveLoadManager()
{
}


HRESULT CSaveLoadManager::LoadUnit(TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//게임오브젝트에서 유닛들 벡터 초기화해줄 것.
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
		ReadFile(hFile, &pUnit->iCollide, sizeof(int), &dwByte, nullptr);
		switch (pUnit->type)
		{
		case UNITTYPE::PLAYER:
			GameObjectManager->Insert_GameObjectManager(CPlayer::Create(pUnit), GAMEOBJECT::PLAYER);
			break;
		case UNITTYPE::GANGSTER:
			if (GameObjectManager->Get_GameObjectVec(GAMEOBJECT::PLAYER).empty())
			{
				ERR_MSG(L"적을 만들기 전에 대상으로 삼을 플레이어가 없습니다.");
				return E_FAIL;
			}
			GameObjectManager->Insert_GameObjectManager(CGangster::Create(GameObjectManager->Get_GameObjectVec(GAMEOBJECT::PLAYER).at(0),pUnit), GAMEOBJECT::GANGSTER);
			break;
		default:
			Safe_Delete(pUnit);
			break;
		}
	}
	CloseHandle(hFile);
	return S_OK;
}
