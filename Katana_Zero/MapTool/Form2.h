#pragma once
#include "afxwin.h"



// CForm2 폼 뷰입니다.

class CForm2 : public CFormView
{
	DECLARE_DYNCREATE(CForm2)

protected:
	CForm2();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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


