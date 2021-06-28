// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MiniView.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CScrollView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}


BEGIN_MESSAGE_MAP(CMiniView, CScrollView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
