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


	CButton m_TerrainRadio[TERRAINTYPE::END];

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedDeleteWall();
	afx_msg void OnBnClickedLand();
	afx_msg void OnBnClickedCelling();
};


