#pragma once
#include "Texture.h"
class CMulti_Texture final :
	public CTexture
{
public:
	explicit CMulti_Texture();
	virtual ~CMulti_Texture();

public:
	const size_t Get_TexInfo_Frame(const wstring & wstrStateKey = L"");

public:
	// CTexture��(��) ���� ��ӵ�
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & dwCount) override;
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const DWORD & dwIndex = 0) override;
	virtual void Release_Texture() override;
private:
	map<wstring, vector<TEXINFO*>> m_mapMultiTex; //������Ʈ Ű�� ���� �̹����� ���� ��
};
