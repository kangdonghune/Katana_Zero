#pragma once
class CMapObject;
class CMapObjectManager
{
	DECLARE_SINGLETON(CMapObjectManager)
private:
	explicit CMapObjectManager();
public:
	~CMapObjectManager();

public:
	//void	Insert_Terrain(CMapObject* pMapObject, TERRAINTYPE::TYPE type) { m_vecTerrain[type].emplace_back(pMapObject); }
	const vector<MYLINE>	Get_TerrainVector(TERRAINTYPE::TYPE type) { return m_vecTerrain[type]; }
	const vector<MYLINE>	Get_TerrainVector(int index) { return m_vecTerrain[index]; }
	void	Load_Terrain(TCHAR* pFilePath);

public:
	HRESULT	Init_MapObjectManager();
	void	Update_MapObjectManager();
	void	LateUpdate_MapObjectManager();
	void	Render_MapObjectManager();
	void	Release_MapObjectManager();

private:
	vector<MYLINE> m_vecTerrain[TERRAINTYPE::END];



};

