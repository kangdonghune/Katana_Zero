#include "stdafx.h"
#include "Player.h"
#include "Texture_Manager.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release_GameObject();
}

CPlayer * CPlayer::Create(UNITINFO* pInfo)
{
	CPlayer* pPlayer = new CPlayer();
	pPlayer->Set_Info(pInfo);
	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Delete(pPlayer);
		return pPlayer;
	}
	return pPlayer;
}

void CPlayer::Update_KeyInput()
{
	bool b_isInPut = false;

	if (GetAsyncKeyState('A') & 0X8000)
	{
		m_State = PLAYERSTATE::RUN;
		m_iRotateY = -1;
		b_isInPut = true;
	}

	if (GetAsyncKeyState('D') & 0X8000)
	{
		m_State = PLAYERSTATE::RUN;
		m_iRotateY = 1;
		b_isInPut = true;
	}
	
	if (!b_isInPut)
		m_State = PLAYERSTATE::IDLE;
}



HRESULT CPlayer::Ready_GameObject()
{
	m_wstrOldState = m_pUnitInfo->wstrState;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 1.f;
	m_fRatio = 2.f;
	m_iRotateY = 1;
	return S_OK;
}

void CPlayer::Update_GameObject()
{

	Update_KeyInput();

	switch (m_State)
	{
	case PLAYERSTATE::IDLE:
		m_pUnitInfo->wstrState = L"Idle";
		break;
	case PLAYERSTATE::RUN:
		m_pUnitInfo->wstrState = L"Run";
		break;
	default:
		break;
	}
	FrameMove(m_fSpeed);
	Update_HitBox();
}

void CPlayer::LateUpdate_GameObject()
{
}

void CPlayer::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"플레이어 정보가 없습니다.");
		return;
	}
	D3DXMATRIX matScale, matRolateY, matTrans, matWorld; 

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iRotateY * m_fRatio, m_fRatio, 0.f);
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
	matWorld = matScale *matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	Render_HitBox();
}

void CPlayer::Release_GameObject()
{
}
