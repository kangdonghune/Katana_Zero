// Form2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "Form2.h"
#include "RECT.h"
#include "Unit.h"
#include "MainFrm.h"
#include "MapToolView.h";
#include "Projectile.h"

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
	ON_BN_CLICKED(IDC_RADIO6, &CForm2::OnBnClickedGangster)
	ON_BN_CLICKED(IDC_RADIO7, &CForm2::OnBnClickedStageChange)
	ON_BN_CLICKED(IDC_BUTTON7, &CForm2::OnBnClickedDeleteStageChange)
	ON_BN_CLICKED(IDC_RADIO8, &CForm2::OnBnClickedButcher)
	ON_BN_CLICKED(IDC_RADIO9, &CForm2::OnBnClickedSmoke)
	ON_BN_CLICKED(IDC_RADIO10, &CForm2::OnBnClickedExplosive)
	ON_BN_CLICKED(IDC_BUTTON8, &CForm2::OnBnClickedDeleteGang)
	ON_BN_CLICKED(IDC_BUTTON9, &CForm2::OnBnClickedDeleteKnife)
	ON_BN_CLICKED(IDC_BUTTON10, &CForm2::OnBnClickedDeleteSmoke)
	ON_BN_CLICKED(IDC_BUTTON11, &CForm2::OnBnClickedDeleteExplosive)
	ON_BN_CLICKED(IDC_RADIO1, &CForm2::OnBnClickedPasable)
	ON_BN_CLICKED(IDC_BUTTON12, &CForm2::OnBnClickedDeletePassable)
	ON_BN_CLICKED(IDC_RADIO11, &CForm2::OnBnClickedBoss)
	ON_BN_CLICKED(IDC_BUTTON13, &CForm2::OnBnClickedDeleteBoss)
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
	D3DXVECTOR3 LastPoint = pView->m_MouseDownPos;
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::CELLING).back()->End.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::CELLING).back()->End.y;
	}
	if (GetAsyncKeyState('E') & 0x8000) //랜드 시작부분을 마지막 점으로
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::CELLING).back()->Start.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::CELLING).back()->Start.y;
	}
	flatPoint = { LastPoint.x,flatPoint.y, 0.f };
	RECTS->Insert_Line(LastPoint, flatPoint, TERRAINTYPE::WALL);
}

void CForm2::CreateCelling()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	D3DXVECTOR3 LastPoint = pView->m_MouseDownPos;
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::WALL).back()->End.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::WALL).back()->End.y;
	}
	flatPoint = { flatPoint.x,LastPoint.y, 0.f };
	RECTS->Insert_Line(LastPoint, flatPoint, TERRAINTYPE::CELLING);
}

void CForm2::CreatePassable()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	D3DXVECTOR3 LastPoint = pView->m_MouseDownPos;
	if (GetAsyncKeyState('Q') & 0x8000) //랜드 마지막 부분을 시작점으로
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::LAND).back()->End.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::LAND).back()->End.y;
		flatPoint = { pView->m_MouseUpPos.x,LastPoint.y, 0.f };
	}
	if (GetAsyncKeyState('E') & 0x8000) //랜드 시작부분을 마지막 점으로
	{
		flatPoint.x = RECTS->Get_Line(TERRAINTYPE::LAND).back()->Start.x;
		flatPoint.y = RECTS->Get_Line(TERRAINTYPE::LAND).back()->Start.y;
		LastPoint = { LastPoint.x,flatPoint.y, 0.f };
	}

	RECTS->Insert_Line(LastPoint, flatPoint, TERRAINTYPE::PASSABLE);
}

void CForm2::CreateStageChange()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	D3DXVECTOR3 flatPoint = pView->m_MouseUpPos;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		flatPoint = { pView->m_MouseDownPos.x,pView->m_MouseUpPos.y, 0.f };
	RECTS->Insert_Line(pView->m_MouseDownPos, flatPoint, TERRAINTYPE::STAGECHANGE);
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

void CForm2::CreateGangster()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	UNITINFO* pUnit = new UNITINFO;
	pUnit->D3VecPos = pView->m_MouseDownPos;
	pUnit->wstrKey = L"Gangster";
	pUnit->type = UNITTYPE::GANGSTER;
	UNITS->Insert_Unit(pUnit, pUnit->type);
}

void CForm2::CreateBoss()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	UNITINFO* pUnit = new UNITINFO;
	pUnit->D3VecPos = pView->m_MouseDownPos;
	pUnit->wstrKey = L"Boss";
	pUnit->type = UNITTYPE::BOSS;
	UNITS->Insert_Unit(pUnit, pUnit->type);
}

void CForm2::CreateButcherKnife()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	ITEMINFO* pItem = new ITEMINFO;
	pItem->D3VecPos = pView->m_MouseDownPos;
	pItem->wstrKey = L"Projectile";
	pItem->wstrState = L"Butcherknife";
	pItem->type = ITEMTYPE::BUTCHERKNIFE;
	Projectile->Insert_Item(pItem, pItem->type);
}

void CForm2::CreateSmoke()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	ITEMINFO* pItem = new ITEMINFO;
	pItem->D3VecPos = pView->m_MouseDownPos;
	pItem->wstrKey = L"Projectile";
	pItem->wstrState = L"Smoke";
	pItem->type = ITEMTYPE::SMOKE;
	Projectile->Insert_Item(pItem, pItem->type);
}

void CForm2::CreateExplosive()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	ITEMINFO* pItem = new ITEMINFO;
	pItem->D3VecPos = pView->m_MouseDownPos;
	pItem->wstrKey = L"Projectile";
	pItem->wstrState = L"Explosivevial";
	pItem->type = ITEMTYPE::EXPLOSIVEVIAL;
	Projectile->Insert_Item(pItem, pItem->type);
}

void CForm2::AutoSave()
{

}

void CForm2::Save_Terrain(TCHAR * pFilePath)
{
	RECTS->Set_LineID();
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
		case TERRAINTYPE::PASSABLE:
			RECTS->Insert_Line(tLine.Start, tLine.End, TERRAINTYPE::PASSABLE);
			break;
		case TERRAINTYPE::STAGECHANGE:
			RECTS->Insert_Line(tLine.Start, tLine.End, TERRAINTYPE::STAGECHANGE);
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
		case UNITTYPE::GANGSTER:
			UNITS->Insert_Unit(pUnit, pUnit->type);
		default:
			ERR_MSG(L"유닛 타입 문제 확인 바람");
			break;
		}
	}
	CloseHandle(hFile);
}

void CForm2::Save_Item(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwByte = 0;
	DWORD dwKeyCount = 0;
	DWORD dwStateCount = 0;
	for (int i = 0; i < ITEMTYPE::END; i++)
	{
		for (auto& pUnit : Projectile->Get_ItemList(i))
		{
			dwKeyCount = (pUnit->wstrKey.length() + 1) * sizeof(wchar_t);
			dwStateCount = (pUnit->wstrState.length() + 1) * sizeof(wchar_t);
			WriteFile(hFile, &dwKeyCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, &dwStateCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, pUnit->wstrKey.c_str(), dwKeyCount, &dwByte, nullptr);
			WriteFile(hFile, pUnit->wstrState.c_str(), dwStateCount, &dwByte, nullptr);
			WriteFile(hFile, &pUnit->D3VecPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pUnit->type, sizeof(UNITTYPE::TYPE), &dwByte, nullptr);
		}

	}
	CloseHandle(hFile);
}

void CForm2::Load_Item(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	Projectile->Release_Projectile();
	DWORD dwByte = 0;
	DWORD dwKeyCount = 0;
	DWORD dwStateCount = 0;
	TCHAR* szKeyBuf = nullptr;
	TCHAR* szStateBuf = nullptr;
	while (true)
	{
		ITEMINFO* pItem = new ITEMINFO;
		ReadFile(hFile, &dwKeyCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		ReadFile(hFile, &dwStateCount, sizeof(DWORD), &dwByte, nullptr);
		szKeyBuf = new TCHAR[dwKeyCount];
		szStateBuf = new TCHAR[dwStateCount];
		ReadFile(hFile, szKeyBuf, dwKeyCount, &dwByte, nullptr);
		ReadFile(hFile, szStateBuf, dwStateCount, &dwByte, nullptr);
		pItem->wstrKey = szKeyBuf;
		pItem->wstrState = szStateBuf;
		Safe_Delete_Array(szKeyBuf);
		Safe_Delete_Array(szStateBuf);
		ReadFile(hFile, &pItem->D3VecPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pItem->type, sizeof(UNITTYPE::TYPE), &dwByte, nullptr);

		switch (pItem->type)
		{
		case ITEMTYPE::BUTCHERKNIFE:
			Projectile->Insert_Item(pItem, pItem->type);
			break;
		case ITEMTYPE::SMOKE:
			Projectile->Insert_Item(pItem, pItem->type);
			break;
		case ITEMTYPE::EXPLOSIVEVIAL:
			Projectile->Insert_Item(pItem, pItem->type);
			break;
		default:
			ERR_MSG(L"아이템 불러오기 문제");
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
	D3DXVECTOR3 LastPoint = pView->m_MouseDownPos;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		flatPoint = { flatPoint.x,LastPoint.y, 0.f };
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::LAND).back()->End.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::LAND).back()->End.y;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			flatPoint = { flatPoint.x,LastPoint.y, 0.f };
	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		flatPoint.x = RECTS->Get_Line(TERRAINTYPE::LAND).back()->Start.x;
		flatPoint.y = RECTS->Get_Line(TERRAINTYPE::LAND).back()->Start.y;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			LastPoint = { LastPoint.x,flatPoint.y, 0.f };
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		LastPoint.x = RECTS->Get_Line(TERRAINTYPE::PASSABLE).back()->End.x;
		LastPoint.y = RECTS->Get_Line(TERRAINTYPE::PASSABLE).back()->End.y;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			flatPoint = { flatPoint.x,LastPoint.y, 0.f };
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		flatPoint.x = RECTS->Get_Line(TERRAINTYPE::PASSABLE).back()->Start.x;
		flatPoint.y = RECTS->Get_Line(TERRAINTYPE::PASSABLE).back()->Start.y;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			LastPoint = { LastPoint.x,flatPoint.y, 0.f };
	}


	RECTS->Insert_Line(LastPoint, flatPoint, TERRAINTYPE::LAND);
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
	Save_Terrain(L"../Data/Stage5/Terrain/Terrain.dat");
	Save_Unit(L"../Data/Stage5/Unit/Unit.dat");
	Save_Item(L"../Data/Stage5/Projectile/Projectile.dat");
	MessageBox(L"저장 완료", L"저장 시스템", MB_OK);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedLoad()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Load_Terrain(L"../Data/Stage5/Terrain/Terrain.dat");
	Load_Unit(L"../Data/Stage5/Unit/Unit.dat");
	Load_Item(L"../Data/Stage5/Projectile/Projectile.dat");
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


void CForm2::OnBnClickedGangster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO6))
	{
		m_iToolState = TOOL_Gangster;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedStageChange()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO7))
	{
		m_iToolState = TOOL_StageChange;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeleteStageChange()
{
	if (RECTS->Get_StageChange().empty())
		return;
	Safe_Delete(RECTS->Get_StageChange().back());
	RECTS->Get_StageChange().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedButcher()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO8))
	{
		m_iToolState = TOOL_Knife;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedSmoke()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO9))
	{
		m_iToolState = TOOL_Smoke;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedExplosive()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO10))
	{
		m_iToolState = TOOL_Explosive;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeleteGang()
{
	if (UNITS->Get_UnitVec(UNITTYPE::GANGSTER).empty())
		return;
	Safe_Delete(UNITS->Get_UnitVec(UNITTYPE::GANGSTER).back());
	UNITS->Get_UnitVec(UNITTYPE::GANGSTER).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedDeleteKnife()
{
	if (Projectile->Get_ItemList(ITEMTYPE::BUTCHERKNIFE).empty())
		return;
	Safe_Delete(Projectile->Get_ItemList(ITEMTYPE::BUTCHERKNIFE).back());
	Projectile->Get_ItemList(ITEMTYPE::BUTCHERKNIFE).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedDeleteSmoke()
{
	if (Projectile->Get_ItemList(ITEMTYPE::SMOKE).empty())
		return;
	Safe_Delete(Projectile->Get_ItemList(ITEMTYPE::SMOKE).back());
	Projectile->Get_ItemList(ITEMTYPE::SMOKE).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedDeleteExplosive()
{
	if (Projectile->Get_ItemList(ITEMTYPE::EXPLOSIVEVIAL).empty())
		return;
	Safe_Delete(Projectile->Get_ItemList(ITEMTYPE::EXPLOSIVEVIAL).back());
	Projectile->Get_ItemList(ITEMTYPE::EXPLOSIVEVIAL).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedPasable()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO1))
	{
		m_iToolState = TOOL_Passable;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeletePassable()
{
	if (RECTS->Get_Passable().empty())
		return;
	Safe_Delete(RECTS->Get_Passable().back());
	RECTS->Get_Passable().pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}


void CForm2::OnBnClickedBoss()
{
	UpdateData(true);

	if (IsDlgButtonChecked(IDC_RADIO11))
	{
		m_iToolState = TOOL_Boss;
	}
	else
	{
		m_iToolState = Tool_Idle;
	}

	UpdateData(false);
}


void CForm2::OnBnClickedDeleteBoss()
{
	if (UNITS->Get_UnitVec(UNITTYPE::BOSS).empty())
		return;
	Safe_Delete(UNITS->Get_UnitVec(UNITTYPE::BOSS).back());
	UNITS->Get_UnitVec(UNITTYPE::BOSS).pop_back();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMapToolView* pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->RedrawWindow();
}
