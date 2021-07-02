// Form2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "Form2.h"
#include "RECT.h"
#include "Unit.h"
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
	ON_BN_CLICKED(IDC_BUTTON1, &CForm2::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CForm2::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_RADIO5, &CForm2::OnBnClickedPlayer)
	ON_BN_CLICKED(IDC_BUTTON6, &CForm2::OnBnClickedDeletePlayer)
END_MESSAGE_MAP()


// CForm2 진단입니다.

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


// CForm2 메시지 처리기입니다.





void CForm2::CreateWall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		flatPoint = { pView->m_MouseDownPos.x,pView->m_MouseUpPos.y, 0.f };
	RECTS->Insert_Line(pView->m_MouseDownPos, flatPoint, TERRAINTYPE::WALL);
}

void CForm2::CreateCelling()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		flatPoint = { pView->m_MouseUpPos.x,pView->m_MouseDownPos.y, 0.f };
	RECTS->Insert_Line(pView->m_MouseDownPos, flatPoint, TERRAINTYPE::CELLING);
}

void CForm2::CreatePlayer()
{
	if (!UNITS->Get_UnitVec(UNITTYPE::PLAYER).empty())
		return;
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	UNITINFO* pUnit = new UNITINFO;
	pUnit->D3VecPos = pView->m_MouseDownPos;
	pUnit->wstrKey = L"Player";
	pUnit->type = UNITTYPE::PLAYER;
	UNITS->Insert_Unit(pUnit, pUnit->type);
}

void CForm2::AutoSave()
{
}

void CForm2::Save_Terrain(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwByte = 0;
	for (int i = 0; i < TERRAINTYPE::END; i++)
	{
		for (auto& pObject : RECTS->Get_Line(i))
		{
			WriteFile(hFile, pObject, sizeof(MYLINE), &dwByte, nullptr);
		}
	}
	CloseHandle(hFile);
}

void CForm2::Load_Terrain(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	RECTS->Release_Rect();
	DWORD dwByte = 0;
	MYLINE tLine;
	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(MYLINE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		switch (tLine.type)
		{
		case TERRAINTYPE::LAND:
			RECTS->Insert_Line(tLine.Start, tLine.End, TERRAINTYPE::LAND);
			break;
		case TERRAINTYPE::WALL:
			RECTS->Insert_Line(tLine.Start, tLine.End, TERRAINTYPE::WALL);
			break;
		case TERRAINTYPE::CELLING:
			RECTS->Insert_Line(tLine.Start, tLine.End, TERRAINTYPE::CELLING);
			break;
		default:
			break;
		}
	}
	CloseHandle(hFile);
}

void CForm2::Save_Unit(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwByte = 0;
	DWORD dwKeyCount = 0;
	DWORD dwStateCount = 0;
	for (int i = 0; i < UNITTYPE::END; i++)
	{
		for (auto& pUnit : UNITS->Get_UnitVec(i))
		{
			dwKeyCount = (pUnit->wstrKey.length()+1)*sizeof(wchar_t);
			dwStateCount = (pUnit->wstrState.length()+1) * sizeof(wchar_t);
			WriteFile(hFile, &dwKeyCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, &dwStateCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, pUnit->wstrKey.c_str(), dwKeyCount, &dwByte, nullptr);
			WriteFile(hFile, pUnit->wstrState.c_str(), dwStateCount, &dwByte, nullptr);
			WriteFile(hFile, &pUnit->D3VecPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pUnit->type, sizeof(UNITTYPE::TYPE), &dwByte, nullptr);
			WriteFile(hFile, &pUnit->iCollide, sizeof(int), &dwByte, nullptr);
		}
	}
	CloseHandle(hFile);

}

void CForm2::Load_Unit(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	UNITS->Release_Unit();
	DWORD dwByte = 0;
	DWORD dwKeyCount = 0;
	DWORD dwStateCount = 0;
	TCHAR* szKeyBuf = nullptr;
	TCHAR* szStateBuf = nullptr;
	while (true)
	{
		UNITINFO* pUnit = new UNITINFO;
		ReadFile(hFile, &dwKeyCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		ReadFile(hFile, &dwStateCount, sizeof(DWORD), &dwByte, nullptr);
		szKeyBuf = new TCHAR[dwKeyCount];
		szStateBuf = new TCHAR[dwStateCount];
		ReadFile(hFile, szKeyBuf, dwKeyCount, &dwByte, nullptr);
		ReadFile(hFile, szStateBuf, dwStateCount, &dwByte, nullptr);
		pUnit->wstrKey = szKeyBuf;
		pUnit->wstrState = szStateBuf;
		Safe_Delete_Array(szKeyBuf);
		Safe_Delete_Array(szStateBuf);
		ReadFile(hFile, &pUnit->D3VecPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pUnit->type, sizeof(UNITTYPE::TYPE), &dwByte, nullptr);
		ReadFile(hFile, &pUnit->iCollide, sizeof(int), &dwByte, nullptr);
		switch (pUnit->type)
		{
		case UNITTYPE::PLAYER:
			UNITS->Insert_Unit(pUnit, pUnit->type);
			break;
		default:
			ERR_MSG(L"유닛 타입 문제 확인 바람");
			break;
		}
	}
	CloseHandle(hFile);
}

void CForm2::CreateLine()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		flatPoint = { pView->m_MouseUpPos.x,pView->m_MouseDownPos.y, 0.f };
	RECTS->Insert_Line(pView->m_MouseDownPos, flatPoint, TERRAINTYPE::LAND);
}



void CForm2::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (RECTS->Get_Wall().empty())
		return;
	Safe_Delete(RECTS->Get_Wall().back());
	RECTS->Get_Wall().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedLand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (RECTS->Get_Land().empty())
		return;
	Safe_Delete(RECTS->Get_Land().back());
	RECTS->Get_Land().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedCelling()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (RECTS->Get_Celling().empty())
		return;
	Safe_Delete(RECTS->Get_Celling().back());
	RECTS->Get_Celling().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Save_Terrain(L"../Data/Stage1/Terrain/Terrain.dat");
	Save_Unit(L"../Data/Stage1/Unit/Unit.dat");
	MessageBox(L"저장 완료", L"저장 시스템", MB_OK);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedLoad()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Load_Terrain(L"../Data/Stage1/Terrain/Terrain.dat");
	Load_Unit(L"../Data/Stage1/Unit/Unit.dat");
	MessageBox(L"불러오기 완료", L"저장 시스템", MB_OK);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedPlayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO5))
	{
		m_iToolState = TOOL_Player;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeletePlayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (UNITS->Get_UnitVec(UNITTYPE::PLAYER).empty())
		return;
	Safe_Delete(UNITS->Get_UnitVec(UNITTYPE::PLAYER).back());
	UNITS->Get_UnitVec(UNITTYPE::PLAYER).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}
