#include "stdafx.h"
#include "Multi_Texture.h"


CMulti_Texture::CMulti_Texture()
{
}


CMulti_Texture::~CMulti_Texture()
{
	Release_Texture();
}

//ex) L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png",L"Player", L"Dash", 11
HRESULT CMulti_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD & dwCount) // 주소, 상태 키, 이미지 개수
{
	auto& iter_find = m_mapMultiTex.find(wstrStateKey); //맵에서 해당 상태 키가 있는지 체크
	if (iter_find == m_mapMultiTex.end()) // 해당 상태키가 없다면
	{
		TCHAR szFilePath[MAX_PATH] = L"";
		TEXINFO* pTexInfo = nullptr;
		for (int i = 0; i < dwCount; ++i) //사진 개수 만큼 반복
		{
			swprintf_s(szFilePath, wstrFilePath.c_str(), i);//위에서 준 파일 경로를 szFilePath에 넣어준다.
			pTexInfo = new TEXINFO;
			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImageInfo))) //해당 경로의 사진의 정보를 pTextInfo에 저장
				goto ERR;
			if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
				szFilePath,  //경로
				pTexInfo->tImageInfo.Width, //이미지 넓이 
				pTexInfo->tImageInfo.Height, //이미지 높이
				pTexInfo->tImageInfo.MipLevels, //이미지 밉레벨
				0, //
				pTexInfo->tImageInfo.Format, //이미지 포멧
				D3DPOOL_MANAGED, //pool
				D3DX_DEFAULT,//필터
				D3DX_DEFAULT,//밉필터
				0, //0으로 지정식 사진 형식과 상관없이 32 비트 ARGB 
				nullptr, 
				nullptr,
				&pTexInfo->pTexture))) //위 이미지 관련 내용들을 pTextInfo에 저장
				goto ERR;

			m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo); //맵에다가 추가
			//m_mapMultiTex[wstrStateKey].여기까지가 벡터 
			//emplace_back(pTexInfo); 에 원소 삽입. 
		}
	}

	return S_OK;
ERR:
	wstring wstrCombine = wstrFilePath + L" Insert MultiTex Failed";
	ERR_MSG(wstrCombine.c_str());
	return E_FAIL;
}

const TEXINFO * CMulti_Texture::Get_TexInfo(const wstring & wstrStateKey /*= L""*/, const DWORD & dwIndex /*= 0*/)
{
	map<wstring, vector<TEXINFO*>>::iterator iter_find = m_mapMultiTex.find(wstrStateKey);
	if (iter_find == m_mapMultiTex.end())
		return nullptr;

	if (iter_find->second.size() <= dwIndex)
		return nullptr;

	return iter_find->second[dwIndex];
	//return m_mapMultiTex[wstrStateKey][dwIndex];
}

void CMulti_Texture::Release_Texture()
{
	for (auto& rPair : m_mapMultiTex)
	{
		for (auto& pTexInfo : rPair.second)
			Safe_Delete(pTexInfo);

		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_mapMultiTex.clear();
}
