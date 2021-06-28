#pragma once
class CTexture
{
public:
	explicit CTexture();

	virtual ~CTexture();
public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& dwCount = 0)PURE;
	// ObjectKey -> ex)Player, Monster1, 1StageBoss, , StateKey 
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"", const DWORD& dwIndex = 0)PURE;
	virtual void Release_Texture()PURE;

};

