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
HRESULT CMulti_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD & dwCount) // �ּ�, ���� Ű, �̹��� ����
{
	auto& iter_find = m_mapMultiTex.find(wstrStateKey); //�ʿ��� �ش� ���� Ű�� �ִ��� üũ
	if (iter_find == m_mapMultiTex.end()) // �ش� ����Ű�� ���ٸ�
	{
		TCHAR szFilePath[MAX_PATH] = L"";
		TEXINFO* pTexInfo = nullptr;
		for (int i = 0; i < dwCount; ++i) //���� ���� ��ŭ �ݺ�
		{
			swprintf_s(szFilePath, wstrFilePath.c_str(), i);//������ �� ���� ��θ� szFilePath�� �־��ش�.
			pTexInfo = new TEXINFO;
			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImageInfo))) //�ش� ����� ������ ������ pTextInfo�� ����
				goto ERR;
			if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
				szFilePath,  //���
				pTexInfo->tImageInfo.Width, //�̹��� ���� 
				pTexInfo->tImageInfo.Height, //�̹��� ����
				pTexInfo->tImageInfo.MipLevels, //�̹��� �ӷ���
				0, //
				pTexInfo->tImageInfo.Format, //�̹��� ����
				D3DPOOL_MANAGED, //pool
				D3DX_DEFAULT,//����
				D3DX_DEFAULT,//������
				0, //0���� ������ ���� ���İ� ������� 32 ��Ʈ ARGB 
				nullptr, 
				nullptr,
				&pTexInfo->pTexture))) //�� �̹��� ���� ������� pTextInfo�� ����
				goto ERR;

			m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo); //�ʿ��ٰ� �߰�
			//m_mapMultiTex[wstrStateKey].��������� ���� 
			//emplace_back(pTexInfo); �� ���� ����. 
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
