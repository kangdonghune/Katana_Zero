#include "stdafx.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "TimeManager.h"

CGameObject::CGameObject()
	:m_pUnitInfo(nullptr)
	, m_tFrame({})
	, m_fSpeed(0)
	, m_State(PLAYERSTATE::IDLE)
	, m_wstrOldState(L"")
	, m_wstrCurState(L"")
	, m_fRatio(0.f)
	, m_iRotateY(1)
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::Update_HitBox()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"유닛 정보가 없습니다.");
		return;
	}
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1; 
	float fCenterY = pTexInfo->tImageInfo.Height >> 1; 	
	//ratio 기본 배율은 유닛의 경우 2배
	m_tHitBox.left = m_pUnitInfo->D3VecPos.x - fCenterX*m_fRatio;
	m_tHitBox.top = m_pUnitInfo->D3VecPos.y - fCenterY*m_fRatio;
	m_tHitBox.right = m_pUnitInfo->D3VecPos.x + fCenterX*m_fRatio;
	m_tHitBox.bottom = m_pUnitInfo->D3VecPos.y + fCenterY*m_fRatio;

}

void CGameObject::FrameMove(float fSpeed)
{
	Update_Frame(); //이미지 개수 갱신.
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * TimeManager->Get_DeltaTime() * fSpeed; //frame start end는 각 클래스에서 정해준다. fspeed가 1.f일 대 1초동안 애니메이션을 전부 그려준다. 
	if (m_tFrame.fFrameEnd <= m_tFrame.fFrameStart)
		m_tFrame.fFrameStart = 0.f;
}

void CGameObject::Render_HitBox()
{
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(1.f);
	D3DXVECTOR2	vLine[5]{ { (float)m_tHitBox.left, (float)m_tHitBox.top} ,{ (float)m_tHitBox.right, (float)m_tHitBox.top } ,{ (float)m_tHitBox.right, (float)m_tHitBox.bottom } ,{ (float)m_tHitBox.left, (float)m_tHitBox.bottom } ,{ (float)m_tHitBox.left, (float)m_tHitBox.top }};
	Device->m_pLine->Draw(vLine, 5, D3DCOLOR_ARGB(255, 200, 200, 0));
	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGameObject::Update_Frame()
{
	//상태가 바뀔 때만 프레임 갱신.
	m_wstrCurState = m_pUnitInfo->wstrState;
	if (m_wstrOldState != m_wstrCurState)
	{
		m_tFrame.fFrameStart = 0.f;
		m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
		m_wstrOldState = m_wstrCurState;
	}

}
