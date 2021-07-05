#include "stdafx.h"
#include "Player.h"
#include "Texture_Manager.h"
#include "FrameManager.h"

CPlayer::CPlayer()
	:m_fDefaultSpeed(15.0f) // ������ �ӵ�
	,m_fJumpDistance(0.f) // �ִ� ��������
	,m_iActing(NOACTING) // ���� ����
	,m_fJumpAngle(0.f) //����,�ϰ� �ӵ�
	,m_fOldJumpAngle(0.f)//���� ���� ����
	,m_fFallAngle(0.f) 
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

void CPlayer::Idle()
{
	m_fSpeed = m_fDefaultSpeed;
	m_fUnitSpeed = m_fDefaultUnitSpeed;
	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}
	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::IDLE_TO_RUN;
		return;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::IDLE_TO_RUN;
		return;
	}

	if (GetAsyncKeyState('S') & 0X8001)
	{
		m_State = PLAYERSTATE::PRECROUCH;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}
}

void CPlayer::Idle_to_walk()
{
}

void CPlayer::Idle_to_run()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 2.5f;

	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}


	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;
	if (Check_FrameEnd()) // ������ ������ �� üũ
	{
		m_State = PLAYERSTATE::RUN;
	}
}

void CPlayer::Run()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 3.f;

	m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;

	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}
	
	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::RUN;
		return;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::RUN;
		return;
	}


	m_State = PLAYERSTATE::RUN_TO_IDLE;
	
}

void CPlayer::Run_to_idle()
{
	m_fSpeed = 30.f;
	m_fUnitSpeed = 1.0f;
	m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;

	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (Check_FrameEnd()) // ������ ������ �� üũ
	{
		m_State = PLAYERSTATE::IDLE;
	}
}

void CPlayer::Walk()
{
}

void CPlayer::Attack()
{
}

void CPlayer::Precrouch()
{
	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}
	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}
	if (Check_FrameEnd())
		m_State = PLAYERSTATE::CROUCH;
}

void CPlayer::Crouch()
{

	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}

	if (GetAsyncKeyState('S') & 0X8001)
	{
		m_State = PLAYERSTATE::CROUCH;
		return;
	}


	m_State = PLAYERSTATE::POSTCROUCH;
}

void CPlayer::Postcrouch()
{

	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}
	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::ROLL;
		return;
	}

	if (Check_FrameEnd())
		m_State = PLAYERSTATE::IDLE;
}

void CPlayer::Dooropen()
{
}

void CPlayer::Doorbreakfull()
{
}

void CPlayer::Fall()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 3.f;


	//���� �ӵ��� ���������δ� fFallSpeed�� �ǵ��� ���������� ����

	float fFallSpeed = sinf(3*m_fFallAngle / g_RADIAN) *7.f; //60������ ���� 1�ʰ� ������ �� fFallSpeed�� 7�� �ȴ�.
	if(3*m_fFallAngle != 90.f)
		m_fFallAngle += 1.5f;

	m_vecPivot.y += fFallSpeed;
	if (GetAsyncKeyState('A') & 0X8000)
	{
		m_iUnitDir = -1;
		m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;
	}

	if (GetAsyncKeyState('D') & 0X8000)
	{
		m_iUnitDir = 1;
		m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;
	}
	if (m_pUnitInfo->iCollide == C_LAND)
	{
		m_State = PLAYERSTATE::IDLE;
		m_fFallAngle = 0.f;
	}
		
}

void CPlayer::Hurtfly_begin()
{
}

void CPlayer::Hurtfly_loob()
{
}

void CPlayer::Hurtground()
{
}

void CPlayer::Hurtrecover()
{
}

void CPlayer::Jump()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 3.f;

	if (GetAsyncKeyState('A') & 0X8001)
	{
		m_iUnitDir = -1;
		m_vecPivot.x += m_fUnitSpeed*m_iUnitDir;
	}
	if (GetAsyncKeyState('D') & 0X8001)
	{
		m_iUnitDir = 1;
		m_vecPivot.x += m_fUnitSpeed*m_iUnitDir;
	}
	
	m_fOldJumpAngle = sinf(3*m_fJumpAngle/g_RADIAN) * m_fJumpDistance;
	m_fJumpAngle += 1.5f;
	m_vecPivot.y += m_fOldJumpAngle;
	m_vecPivot.y -= sinf(3*m_fJumpAngle / g_RADIAN) * m_fJumpDistance;

	if (m_fJumpDistance <= sinf(3*m_fJumpAngle / g_RADIAN) * m_fJumpDistance)//���� �ӵ��� ���� �ٴٰ� �ִ� ���̿� �����ϴ� ���� ����������.
	{
		m_fJumpAngle = 0.f;
		m_State = PLAYERSTATE::FALL;
		return;
	}
}

void CPlayer::Dance()
{
}

void CPlayer::Flip()
{
}

void CPlayer::Roll()
{
	m_fSpeed = 30.f;
	m_fUnitSpeed = 5.f;
	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹 ���µ� �ƴϰ�, ���� ���ε� �ƴ϶��.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}
	if(!Check_FrameEnd())
	{
		m_vecPivot.x += m_fUnitSpeed*m_iUnitDir;
	}
	if (Check_FrameEnd())
		m_State = PLAYERSTATE::IDLE;
}

//void CPlayer::Update_KeyInput()
//{
//	bool b_isInPut = false; //Ű�� ���ȴ���
//
//	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //�浹�ϰ� �ִ� ���µ� �ƴϰ� �����ϰ� �ִ� ���µ� �ƴ϶�� �����϶�.
//	{
//		m_State = PLAYERSTATE::FALL;
//		b_isInPut = true;
//	}
//
//	if (GetAsyncKeyState('A') & 0X8000)
//	{
//		if (m_State == PLAYERSTATE::IDLE)
//		{
//			m_State = PLAYERSTATE::IDLE_TO_RUN;
//		}
//
//		m_iUnitDir = -1;
//		b_isInPut = true;
//	}
//
//	if (GetAsyncKeyState('S') & 0X8000 && m_iActing == NOACTING && m_pUnitInfo->iCollide == C_LAND)
//	{
//		if (m_State != PLAYERSTATE::PRECROUCH && m_State != PLAYERSTATE::CROUCH)
//			m_State = PLAYERSTATE::PRECROUCH;
//		b_isInPut = true;
//	}
//
//	if (GetAsyncKeyState('D') & 0X8000)
//	{
//		if (m_State == PLAYERSTATE::IDLE)
//		{
//			m_State = PLAYERSTATE::IDLE_TO_RUN;
//		}
//		m_iUnitDir = 1;
//		b_isInPut = true;
//	}
//	if ((GetAsyncKeyState('W') & 0X8000) && m_pUnitInfo->iCollide == C_LAND && m_State != PLAYERSTATE::JUMP) //������ �� ���̶� ������ ��� ���¸� �����Ѵ�.
//	{
//		m_State = PLAYERSTATE::JUMP;
//		b_isInPut = true;
//	}
//
//
//	if (!b_isInPut && m_iActing == NOACTING && m_pUnitInfo->iCollide == C_LAND) //Ű �Է��� ���� ���� �ϰ� �ִ� ������ ���ٸ�.
//	{
//		if (m_State == PLAYERSTATE::CROUCH || m_State == PLAYERSTATE::POSTCROUCH)
//			m_State = PLAYERSTATE::POSTCROUCH;
//		else
//			m_State = PLAYERSTATE::IDLE;
//	}
//
//}

void CPlayer::Update_UnitState()
{

	switch (m_State)
	{
	case PLAYERSTATE::IDLE:
		m_pUnitInfo->wstrState = L"Idle";
		Update_Frame();// ������ �����ϰ�
		Idle();
		break;
	case PLAYERSTATE::IDLE_TO_WALK:
		break;
	case PLAYERSTATE::IDLE_TO_RUN:
		m_pUnitInfo->wstrState = L"Idle_to_run";
		Update_Frame();
		Idle_to_run();
		break;
	case PLAYERSTATE::RUN:
		m_pUnitInfo->wstrState = L"Run";
		Update_Frame();
		Run();
		break;
	case PLAYERSTATE::RUN_TO_IDLE:
		m_pUnitInfo->wstrState = L"Run_to_idle";
		Update_Frame();
		Run_to_idle();
		break;
	case PLAYERSTATE::WALK:
		break;
	case PLAYERSTATE::ATTACK:
		break;
	case PLAYERSTATE::PRECROUCH:
		m_pUnitInfo->wstrState = L"Precrouch";
		Update_Frame();
		Precrouch();
		break;
	case PLAYERSTATE::CROUCH:
		m_pUnitInfo->wstrState = L"Crouch";
		Update_Frame();
		Crouch();
		break;
	case PLAYERSTATE::POSTCROUCH:
		m_pUnitInfo->wstrState = L"Postcrouch";
		Update_Frame();
		Postcrouch();
		break;
	case PLAYERSTATE::DOOROPEN:
		break;
	case PLAYERSTATE::DOORBREAKFULL:
		break;
	case PLAYERSTATE::FALL:
		m_pUnitInfo->wstrState = L"Fall";
		Update_Frame();
		Fall();
		break;
	case PLAYERSTATE::HURTFLY_BEGIN:
		break;
	case PLAYERSTATE::HURTFLY_LOOB:
		break;
	case PLAYERSTATE::HURTGROUND:
		break;
	case PLAYERSTATE::HURTRECOVER:
		break;
	case PLAYERSTATE::JUMP:
		m_pUnitInfo->wstrState = L"Jump";
		Update_Frame();
		Jump();
		break;
	case PLAYERSTATE::DANCE:
		break;
	case PLAYERSTATE::FLIP:
		break;
	case PLAYERSTATE::ROLL:
		m_pUnitInfo->wstrState = L"Roll";
		Update_Frame();
		Roll();
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
	m_fJumpDistance = 100.f;
	m_fSpeed = m_fDefaultSpeed;
	m_fRatio = 1.8f;
	m_iUnitDir = 1;
	m_vecPivot = { m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y + m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	m_fDefaultUnitSpeed = 5.f;
	m_fUnitSpeed = m_fDefaultUnitSpeed;
	return S_OK;
}

void CPlayer::Update_GameObject()
{

	//Update_KeyInput(); //Ű �Է� üũ
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
	D3DXMatrixTranslation(&matTrans, m_vecPivot.x, m_vecPivot.y- m_fRatio*fCenterY, 0.f);
	matWorld = matScale *matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	FrameManager->Render_Frame_Manager_FrameNum((size_t)m_tFrame.fFrameStart);
	FrameManager->Render_Frame_Manager_FrameName(m_pUnitInfo->wstrState);
	Render_HitBox();
}

void CPlayer::Release_GameObject()
{
}

void CPlayer::Wallslide()
{
}
