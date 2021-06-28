// Form2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "Form2.h"
#include "RECT.h"
#include "MainFrm.h"
#include "MapToolView.h";

// CForm2

IMPLEMENT_DYNCREATE(CForm2, CFormView)

CForm2::CForm2()
	: CFormView(IDD_FORM2)
	, m_iToolState(Tool_Idle)
{

}

CForm2::~CForm2()
{
}

void CForm2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm2, CFormView)



	ON_BN_CLICKED(IDC_RADIO2, &CForm2::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CForm2::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CForm2::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON3, &CForm2::OnBnClickedDeleteWall)
	ON_BN_CLICKED(IDC_BUTTON4, &CForm2::OnBnClickedLand)
	ON_BN_CLICKED(IDC_BUTTON5, &CForm2::OnBnClickedCelling)
END_MESSAGE_MAP()


// CForm2 �����Դϴ�.

#ifdef _DEBUG
void CForm2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm2 �޽��� ó�����Դϴ�.





void CForm2::CreateWall()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	RECTS->Insert_Rect(pView->m_MouseDownPos, pView->m_MouseUpPos, TERRAINTYPE::WALL);

}

void CForm2::CreateCelling()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	RECTS->Insert_Line(pView->m_MouseDownPos, pView->m_MouseUpPos, TERRAINTYPE::CELLING);
}

void CForm2::CreateLine()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	RECTS->Insert_Line(pView->m_MouseDownPos, pView->m_MouseUpPos, TERRAINTYPE::LAND);
}



void CForm2::OnBnClickedRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO2))
	{
		m_iToolState = TOOL_Land;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedRadio3() // wall
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO3))
	{
		m_iToolState = TOOL_Wall;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedRadio4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO4))
	{
		m_iToolState = TOOL_Celling;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeleteWall()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (RECTS->Get_Rects().empty())
		return;
	RECTS->Get_Rects().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedLand()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (RECTS->Get_Land().empty())
		return;
	RECTS->Get_Land().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedCelling()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (RECTS->Get_Celling().empty())
		return;
	RECTS->Get_Celling().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}
