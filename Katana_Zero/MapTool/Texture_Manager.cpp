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
	auto& iter_find = m_mapTexture.find(wstrObjectKey); //�ش� �̸��� ���� ������Ʈ�� �ִ��� üũ
	CTexture* pTexture = nullptr;
	if (m_mapTexture.end() == iter_find) //�ش� ������Ʈ�� ���� �̸��� Ű�� ���ٸ�
	{
		pTexture = new CMulti_Texture;
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))//�ش� ������Ʈ�� ���� Ű�� �̹��� ������ �Ѱ��༭ ����
			goto ERR;

		m_mapTexture.emplace(wstrObjectKey, pTexture); //������Ʈ Ű�� ������� ��Ƽ �ؽ��ĸ� �ʿ� �߰�
	}
	else {
		//m_mapTexture[wstrObjectKey] - ��������� CMulti_Texture 
		//�ش� ������Ʈ�� �ִٸ�
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, dwCount))) //�ش� ������Ʈ Ű�� ���� Ű�� �̹��� �߰�
			goto ERR;
	}

	return S_OK;
ERR:
	ERR_MSG(L"Texture Insert Failed");
	return E_FAIL;
}

const TEXINFO * CTexture_Manager::Get_TexInfo_Manager(const wstring & wstrOjbectKey, const wstring & wstrStateKey, const DWORD & dwIndex)
{
	auto& iter_find = m_mapTexture.find(wstrOjbectKey);// �ش� �̸��� ���� ������Ʈ  Ű �ִ� �� üũ
	if (m_mapTexture.end() == iter_find) //���ٸ�
		return nullptr;
	return iter_find->second->Get_TexInfo(wstrStateKey, dwIndex); //������ �ش� ������Ʈ�� �ؽ��Ŀ��� Ư�� ������ Ư�� �ε��� �̹��� ���� ��������.
	//return m_mapTexture[wstrOjbectKey]->Get_TexInfo(wstrStateKey, dwIndex); 
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}
