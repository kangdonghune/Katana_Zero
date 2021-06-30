#pragma once
class CMapObject abstract
{
public:
	explicit CMapObject();
	virtual ~CMapObject();

public:
	virtual void	Update_MapObject() PURE;
	virtual void	LateUpdate_MapObject() PURE;
	virtual void	Render_MapObject() PURE;
	virtual void	Release_MapObject() PURE;
};

