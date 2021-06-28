#pragma once

class CTexture;
class CTexture_Manager
{
	DECLARE_SINGLETON(CTexture_Manager)

private:
	explicit CTexture_Manager();
	virtual ~CTexture_Manager();
public:
	HRESULT Insert_Texture_Manager(
		const wstring& wstrFilePath,
		const wstring& wstrOjbectKey,
		const wstring& wstrStateKey,
		const DWORD& dwCount);
	const TEXINFO* Get_TexInfo_Manager(const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const DWORD& dwIndex);
	void Release_Texture_Manager();
private:
	// ObjectKey ;
	map<wstring, CTexture*> m_mapTexture; // 오브젝트 이름 키와 멀티텍스쳐 포인터

};

