
// MapToolView.cpp : CMapToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMapToolView ����/�Ҹ�

CMapToolView::CMapToolView()
	:m_MousePos(0.f, 0.f, 0.f)
	,m_MouseDownPos(0.f, 0.f, 0.f)
	,m_MouseUpPos(0.f, 0.f, 0.f)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapToolView �׸���

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


	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMapToolView �μ�

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMapToolView ����

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView �޽��� ó����


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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	ScreenToClient(&rc); // ȭ�鿡�� �ش� Ŭ����
	m_MousePos.x = rc.x + GetScrollPos(SB_HORZ);
	m_MousePos.y = rc.y + GetScrollPos(SB_VERT);
	return m_MousePos;
}

void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	Invalidate(FALSE);
	m_MouseDownPos = GetMousePos();
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
