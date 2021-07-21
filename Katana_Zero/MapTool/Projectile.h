#pragma once
class CMapToolView;
class CProjectile
{
	DECLARE_SINGLETON(CProjectile)
private:
	CProjectile();
public:
	~CProjectile();

public:
	list<ITEMINFO*>& Get_ItemList(int index) { return m_lstItem[index]; }

public:
	void Insert_Item(ITEMINFO* pItemInfo, ITEMTYPE::TYPE type) { m_lstItem[type].emplace_back(pItemInfo); }
	void Render_Projectile();
	void Release_Projectile();
private:
	list<ITEMINFO*> m_lstItem[ITEMTYPE::END];
	CMapToolView* m_pView;
};

