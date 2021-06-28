#include "stdafx.h"
#include "Texture_Manager.h"
#include "Multi_Texture.h"

IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_Texture_Manager();
}
//L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png",L"Player", L"Attack", 6
//		L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png",L"Player", L"Dash", 11
//// L"../Texture/Cube.png", L"Cube"
HRESULT CTexture_Manager::Insert_Texture_Manager(const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwCount)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey); //해당 이름과 같은 오브젝트가 있는지 체크
	CTexture* pTexture = nullptr;
	if (m_mapTexture.end() == iter_find) //해당 오브젝트와 같은 이름의 키가 없다면
	{
		pTexture = new CMulti_Texture;
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))//해당 오브젝트의 상태 키와 이미지 개수를 넘겨줘서 생성
			goto ERR;

		m_mapTexture.emplace(wstrObjectKey, pTexture); //오브젝트 키와 만들어진 멀티 텍스쳐를 맵에 추가
	}
	else {
		//m_mapTexture[wstrObjectKey] - 여기까지가 CMulti_Texture 
		//해당 오브젝트가 있다면
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, dwCount))) //해당 오브젝트 키에 상태 키와 이미지 추가
			goto ERR;
	}

	return S_OK;
ERR:
	ERR_MSG(L"Texture Insert Failed");
	return E_FAIL;
}

const TEXINFO * CTexture_Manager::Get_TexInfo_Manager(const wstring & wstrOjbectKey, const wstring & wstrStateKey, const DWORD & dwIndex)
{
	auto& iter_find = m_mapTexture.find(wstrOjbectKey);// 해당 이름과 같은 오브젝트  키 있는 지 체크
	if (m_mapTexture.end() == iter_find) //없다면
		return nullptr;
	return iter_find->second->Get_TexInfo(wstrStateKey, dwIndex); //있으면 해당 오브젝트의 텍스쳐에서 특정 상태의 특정 인덱스 이미지 정보 가져오기.
	//return m_mapTexture[wstrOjbectKey]->Get_TexInfo(wstrStateKey, dwIndex); 
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}
