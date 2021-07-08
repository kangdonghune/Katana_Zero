#pragma once
#include "afxwin.h"



// CForm2 �� ���Դϴ�.

class CForm2 : public CFormView
{
	DECLARE_DYNCREATE(CForm2)

protected:
	CForm2();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm2();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM2 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	int		m_iToolState;


public:	
	void CreateWall();
	void CreateLine();
	void CreateCelling();
	void CreatePlayer();
	void CreateGangster();

	void AutoSave();
	void Save_Terrain(TCHAR* pFilePath);
	void Load_Terrain(TCHAR* pFilePath);
	void Save_Unit(TCHAR* pFilePath);
	void Load_Unit(TCHAR* pFilePath);


	CButton m_TerrainRadio[TERRAINTYPE::END];

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedDeleteWall();
	afx_msg void OnBnClickedLand();
	afx_msg void OnBnClickedCelling();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedPlayer();
	afx_msg void OnBnClickedDeletePlayer();
	afx_msg void OnBnClickedGangster();
};


