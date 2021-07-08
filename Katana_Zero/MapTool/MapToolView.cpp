
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "MainFrm.h"
#include "C_Map.h"
#include "Form2.h"
#include "RECT.h"
#include "Texture_Manager.h"
#include "Unit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView
HWND g_hWND;
IMPLEMENT_DYNCREATE(CMapToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMapToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
	:m_MousePos(0.f, 0.f, 0.f)
	,m_MouseDownPos(0.f, 0.f, 0.f)
	,m_MouseUpPos(0.f, 0.f, 0.f)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{

	Safe_Delete(m_pMap);
	RECTS->Destroy_Instance();
	UNITS->Destroy_Instance();
	Texture_Maneger->Destroy_Instance();
	Device->Destroy_Instance();

}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Device->Render_Begin();
	m_pMap->Render_Map();
	RenderRects();
	UNITS->Render_Unit();
	Device->Render_End();


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(5000, 5000));
	g_hWND = m_hWnd;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	RECT rcMain{};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView);
	int iGapX = rcMain.right - rcView.right; //1440 - 1420 = 20
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX + 1, WINCY + iGapY + 1, SWP_NOMOVE);

	if (FAILED(Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return;
	}

	m_pMap = new C_Map();
	if (FAILED(m_pMap->Ready_Map()))
		return;
	m_pMap->SetView(this);
	Texture_Maneger->Init_Texture_Manager();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMapToolView::Select_ToolFunction(int ToolState)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm2* pForm = dynamic_cast<CForm2*>(pMain->m_MainSplitter.GetPane(0, 0));
	switch (ToolState)
	{
	case Tool_Idle:
		return;
	case TOOL_Wall:
		pForm->CreateWall();
		return;
	case TOOL_Land:
		pForm->CreateLine();
		return;
	case TOOL_Celling:
		pForm->CreateCelling();
		return;
	case TOOL_Player:
		pForm->CreatePlayer();
	case TOOL_Gangster:
		pForm->CreateGangster();
	default:
		break;
	}
}

D3DXVECTOR3 CMapToolView::GetMousePos()
{
	m_MousePos = {};
	CPoint rc = {};
	GetCursorPos(&rc);
	ScreenToClient(&rc); // 화면에서 해당 클래스
	m_MousePos.x = rc.x + GetScrollPos(SB_HORZ);
	m_MousePos.y = rc.y + GetScrollPos(SB_VERT);
	return m_MousePos;
}

void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate(FALSE);
	m_MouseDownPos = GetMousePos();
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_MouseUpPos = GetMousePos();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm2* pForm = dynamic_cast<CForm2*>(pMain->m_MainSplitter.GetPane(0, 0));
	Select_ToolFunction(pForm->m_iToolState);
	Invalidate(false);
	CScrollView::OnLButtonUp(nFlags, point);
}

void CMapToolView::RenderRects()
{

	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(5.f);
	for (auto& pLine : RECTS->Get_Wall())
	{
		D3DXVECTOR2	vLine[2]{ { float(pLine->Start.x - GetScrollPos(SB_HORZ)),float(pLine->Start.y - GetScrollPos(SB_VERT)) },{ float(pLine->End.x - GetScrollPos(SB_HORZ)),float(pLine->End.y - GetScrollPos(SB_VERT)) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

	}

	for (auto& pLine : RECTS->Get_Land())
	{
		D3DXVECTOR2	vLine[2]{ { float(pLine->Start.x - GetScrollPos(SB_HORZ)),float(pLine->Start.y - GetScrollPos(SB_VERT)) },{ float(pLine->End.x - GetScrollPos(SB_HORZ)),float(pLine->End.y - GetScrollPos(SB_VERT)) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	for (auto& pLine : RECTS->Get_Celling())
	{
		D3DXVECTOR2	vLine[2]{ { float(pLine->Start.x - GetScrollPos(SB_HORZ)),float(pLine->Start.y - GetScrollPos(SB_VERT)) },{ float(pLine->End.x - GetScrollPos(SB_HORZ)),float(pLine->End.y - GetScrollPos(SB_VERT)) } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 0, 0, 255));
	}

	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
