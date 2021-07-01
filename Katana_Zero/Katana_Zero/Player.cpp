#include "stdafx.h"
#include "Player.h"
#include "Texture_Manager.h"
#include "FrameManager.h"

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

	if (true)
	{
		m_State = PLAYERSTATE::Fall;
		b_isInPut = true;
	}

	if (GetAsyncKeyState('A') & 0X8000)
	{
		if (m_State == PLAYERSTATE::IDLE)
		{
			m_State = PLAYERSTATE::IDLE_TO_RUN;
		}

		m_iUnitDir = -1;
		b_isInPut = true;
	}

	if (GetAsyncKeyState('D') & 0X8000)
	{
		if (m_State == PLAYERSTATE::IDLE)
		{
			m_State = PLAYERSTATE::IDLE_TO_RUN;
		}
		m_iUnitDir = 1;
		b_isInPut = true;
	}
	
	if (!b_isInPut)
		m_State = PLAYERSTATE::IDLE;
}

void CPlayer::Update_UnitState()
{

	switch (m_State)
	{
	case PLAYERSTATE::IDLE:
		m_pUnitInfo->wstrState = L"Idle";
		break;
	case PLAYERSTATE::IDLE_TO_WALK:
		break;
	case PLAYERSTATE::IDLE_TO_RUN:
		m_pUnitInfo->wstrState = L"Idle_to_run";
		m_pUnitInfo->D3VecPos.x += 2.5f * m_iUnitDir;
		Update_Frame();// ������ �����ϰ�
		if (Check_FrameEnd()) // ������ ������ �� üũ
		{
			m_State = PLAYERSTATE::RUN;
		}
		break;
	case PLAYERSTATE::RUN:
		m_pUnitInfo->wstrState = L"Run";
		m_pUnitInfo->D3VecPos.x += 2.5f * m_iUnitDir;
		break;
	case PLAYERSTATE::RUN_TO_IDLE:
		m_pUnitInfo->wstrState = L"Run_to_idle";
		break;
	case PLAYERSTATE::WALK:
		break;
	case PLAYERSTATE::ATTACK:
		break;
	case PLAYERSTATE::PRECROUCH:
		break;
	case PLAYERSTATE::CROUCH:
		break;
	case PLAYERSTATE::POSTCROUCH:
		break;
	case PLAYERSTATE::DoorOpen:
		break;
	case PLAYERSTATE::DoorBreakFull:
		break;
	case PLAYERSTATE::Fall:
		m_pUnitInfo->wstrState = L"Fall";
		m_pUnitInfo->D3VecPos.y += 3.f;
		break;
	case PLAYERSTATE::Hurtfly_begin:
		break;
	case PLAYERSTATE::Hurtfly_loob:
		break;
	case PLAYERSTATE::Hurtground:
		break;
	case PLAYERSTATE::Hurtrecover:
		break;
	case PLAYERSTATE::Jump:
		break;
	case PLAYERSTATE::Dance:
		break;
	case PLAYERSTATE::Flip:
		break;
	case PLAYERSTATE::ROLL:
		break;
	case PLAYERSTATE::WALLSLIDE:
		break;
	case PLAYERSTATE::END:
		break;
	default:
		break;
	}
}



HRESULT CPlayer::Ready_GameObject()
{
	m_wstrOldState = m_pUnitInfo->wstrState;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 12.0f;
	m_fRatio = 1.2f;
	m_iUnitDir = 1;
	return S_OK;
}

void CPlayer::Update_GameObject()
{

	Update_KeyInput(); //Ű �Է� üũ
	Update_UnitState(); // Ű �Է¿� ���� ���� ��ȭ // �� �ܰ迡�� ������ ������ �ó� üũ�ؾ� �ϴµ�

 	Update_Frame(); // ���� ���°� ���� ���¿� �ٸ� ���·� ���ߴٸ� ������ ����
	FrameMove(m_fSpeed); //���� ������ ���� �Ǵ� �ʱ�ȭ
	Update_HitBox(); // ���� ���� �������� �浹 ���� ������Ʈ
}

void CPlayer::LateUpdate_GameObject()
{
}

void CPlayer::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"�÷��̾� ������ �����ϴ�.");
		return;
	}
	D3DXMATRIX matScale, matRolateY, matTrans, matWorld; 

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player���� �ؽ��� ã�� ����");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
	matWorld = matScale *matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	FrameManager->Render_Frame_Manager_FrameNum((size_t)m_tFrame.fFrameStart);
	Render_HitBox();
}

void CPlayer::Release_GameObject()
{
}
