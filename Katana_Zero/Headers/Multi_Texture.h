#pragma once
#include "Texture.h"
class CMulti_Texture final :
	public CTexture
{
public:
	explicit CMulti_Texture();
	virtual ~CMulti_Texture();
public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & dwCount) override;
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const DWORD & dwIndex = 0) override;
	virtual void Release_Texture() override;
private:
	map<wstring, vector<TEXINFO*>> m_mapMultiTex; //스테이트 키에 따른 이미지에 정보 맵
};

