#include "stdafx.h"
#include "Player.h"
#include "Texture_Manager.h"
#include "GameObjectManager.h"
#include "FrameManager.h"
#include "TimeManager.h"
#include "PlayerAttack.h"

CPlayer::CPlayer()
	:m_fDefaultSpeed(15.0f) // 프레임 속도
	, m_fJumpDistance(0.f) // 최대 점프높이
	, m_iActing(NOACTING) // 동작 여부
	, m_fJumpAngle(0.f) //점프,하강 속도
	, m_fOldJumpAngle(0.f)//이전 점프 높이
	, m_fOldJumpAngleX(0.f)//이전 점프 높이
	, m_fOldJumpAngleY(0.f)//이전 점프 높이
	, m_fFallAngle(0.f)
	, m_szPivot(L"")
{
}


CPlayer::~CPlayer()
{
	Release_GameObject();
}

CGameObject * CPlayer::Create(UNITINFO* pInfo)
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

	if (m_pUnitInfo->iCollide & C_LAND) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_fJumpAngle = 0.f;
		m_fFallAngle = 0.f;
	}


	if ((GetAsyncKeyState(VK_LBUTTON) & 0x0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		m_fJumpAngle - 0.f;
		return;
	}

	if (m_pUnitInfo->iCollide & C_WALL && !(m_pUnitInfo->iCollide & C_LAND))
	{
		m_State = PLAYERSTATE::FALL;
		if(m_pUnitInfo->iCollide & C_WALLL && GetAsyncKeyState('A') & 0x8000)
			m_State = PLAYERSTATE::WALLSLIDE;
		if (m_pUnitInfo->iCollide & C_WALLR && GetAsyncKeyState('D') & 0x8000)
			m_State = PLAYERSTATE::WALLSLIDE;

		return;
	}

	
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (GetAsyncKeyState('A') & 0x8001)
	{
		m_iUnitDir = -1;
		m_State = PLAYERSTATE::IDLE_TO_RUN;
		return;
	}
	if (GetAsyncKeyState('D') & 0x8001)
	{
		m_iUnitDir = 1;
		m_State = PLAYERSTATE::IDLE_TO_RUN;
		return;
	}

	if (GetAsyncKeyState('S') & 0x8001)
	{
		m_State = PLAYERSTATE::PRECROUCH;
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

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}


	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}


	if (GetAsyncKeyState('S') & 0X8001)
	{
		m_State = PLAYERSTATE::CROUCH;
		return;
	}


	m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;
	if (Check_FrameEnd()) // 프레임 끝났는 지 체크
	{
		m_State = PLAYERSTATE::RUN;
	}
}

void CPlayer::Run()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 5.f;

	m_vecPivot.x += m_fUnitSpeed * m_iUnitDir;

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		return;
	}
	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}

	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}

	if (GetAsyncKeyState('S') & 0X8001)
	{
		m_State = PLAYERSTATE::CROUCH;
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

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}


	if (GetAsyncKeyState('W') & 0X8000)
	{
		m_State = PLAYERSTATE::JUMP;
		return;
	}
	if (GetAsyncKeyState('S') & 0X8001)
	{
		m_State = PLAYERSTATE::CROUCH;
		return;
	}


	if (Check_FrameEnd()) // 프레임 끝났는 지 체크
	{
		m_State = PLAYERSTATE::IDLE;
	}
}

void CPlayer::Walk()
{
}

void CPlayer::Attack()
{
	m_fSpeed = 25.f;
	POINT tMousePos{};
	
	if(m_fRotateAngle == 0.f)
	{
		m_fAttackLimit = 0.f;
		GetCursorPos(&tMousePos);
		ScreenToClient(g_hWND, &tMousePos);
		m_vecMousePos = { float(tMousePos.x), float(tMousePos.y), 0 };
		D3DXVECTOR3 TexturePos = { m_vecPivot.x, m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
		m_vecDir = m_vecMousePos - TexturePos;
		D3DXVec3Normalize(&m_vecDir, &m_vecDir); //단위벡터로
		D3DXVECTOR3 vLook{ 1.f,0,0 };
		m_iUnitDir = 1;
		m_fRotateAngle = D3DXToDegree(acosf(D3DXVec3Dot(&m_vecDir, &vLook)));
		if (TexturePos.y <= tMousePos.y)
			m_fRotateAngle *= -1.f;
		m_fTargetAngle = m_fRotateAngle;
		m_fRotateAngle = 360 - m_fRotateAngle;
		if (m_vecMousePos.x <= m_vecPivot.x)
		{
			m_iUnitDir = -1;
			m_fRotateAngle = 180 + m_fRotateAngle;
		}
		
	}
	
	if (m_fJumpDistance > sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance)//점프 속도가 점점 줄다가 최대 높이에 도달하는 순간 떨어지도록.
	{
		m_fOldJumpAngle = sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
		m_fOldJumpAngleX = cosf(D3DXToRadian(m_fTargetAngle)) *sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
		m_fOldJumpAngleY = sinf(D3DXToRadian(m_fTargetAngle)) * sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
		m_fJumpAngle += 1.5f;
		m_vecPivot.x -= m_fOldJumpAngleX;
		m_vecPivot.x += cosf(D3DXToRadian(m_fTargetAngle)) *sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
		m_vecPivot.y += m_fOldJumpAngleY;
		m_vecPivot.y -= sinf(D3DXToRadian(m_fTargetAngle)) * sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
	
	}

	//m_vecPivot.x += m_iUnitDir * cosf(m_fRotateAngle) * m_fUnitSpeed;
	//m_vecPivot.y += m_iUnitDir * sinf(m_fRotateAngle) * m_fUnitSpeed;

	if (Check_FrameEnd())
	{
		m_State = PLAYERSTATE::IDLE;
		m_fRotateAngle = 0;
	}
}

void CPlayer::Precrouch()
{
	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;

		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		m_fJumpAngle = 0.f;
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

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;

		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
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

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;

		return;
	}

	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		return;
	}
	if (GetAsyncKeyState('W') & 0X8000 )
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


	//낙하 속도가 최종적으로는 fFallSpeed가 되도록 점진적으로 증가

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;

		return;
	}

	float fFallSpeed = sinf(3*m_fFallAngle / g_RADIAN) *7.f; //60프레임 기준 1초가 지났을 때 fFallSpeed는 7이 된다.
	if(3*m_fFallAngle != 90.f)
		m_fFallAngle += 1.5f;

	m_vecPivot.y += fFallSpeed;


	if (m_pUnitInfo->iCollide & C_WALL && !(m_pUnitInfo->iCollide & C_LAND))
	{
		if (m_pUnitInfo->iCollide & C_WALLL && GetAsyncKeyState('A') & 0X8000)
		{
			m_State = PLAYERSTATE::WALLSLIDE;
			return;
		}
		if (m_pUnitInfo->iCollide & C_WALLR && GetAsyncKeyState('D') & 0X8000)
		{
			m_State = PLAYERSTATE::WALLSLIDE;
			return;
		}

	}

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

	if (GetAsyncKeyState('S') & 0X8000)
	{
		m_vecPivot.y += 3*m_fUnitSpeed;
	}

	if (m_pUnitInfo->iCollide & C_LAND)
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
	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;

		m_fJumpAngle = 0.f;
		m_fFallAngle = 0.f;
		return;
	}
	if (m_pUnitInfo->iCollide & C_WALL && !(m_pUnitInfo->iCollide & C_LAND))
	{
		if (m_pUnitInfo->iCollide & C_WALLL && GetAsyncKeyState('A') & 0X8000)
		{
			m_State = PLAYERSTATE::WALLSLIDE;
			return;
		}
		if (m_pUnitInfo->iCollide & C_WALLR && GetAsyncKeyState('D') & 0X8000)
		{
			m_State = PLAYERSTATE::WALLSLIDE;
			return;
		}

	}

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

	if (m_fJumpDistance <= sinf(3*m_fJumpAngle / g_RADIAN) * m_fJumpDistance)//점프 속도가 점점 줄다가 최대 높이에 도달하는 순간 떨어지도록.
	{

		m_State = PLAYERSTATE::FALL;
		m_fJumpAngle = 0.f;
		return;
	}
}

void CPlayer::Dance()
{
}

void CPlayer::Flip()
{
	m_fSpeed = 40.f;
	m_fUnitSpeed = 10.f;
	
	if (m_pUnitInfo->iCollide & C_WALL && m_iOldCollide & C_NONE)
	{
		m_State = PLAYERSTATE::WALLSLIDE;
		m_fFallAngle = 0.f;
		m_iOldCollide = m_pUnitInfo->iCollide;
		return;
	}

	if (m_pUnitInfo->iCollide & C_WALLL)
		m_iUnitDir = 1;
	if (m_pUnitInfo->iCollide & C_WALLR)
		m_iUnitDir = -1;


	m_vecPivot.x += m_iUnitDir*m_fUnitSpeed;
	m_vecPivot.y -= m_fUnitSpeed;

	m_iOldCollide = m_pUnitInfo->iCollide;

	if (Check_FrameEnd())
	{
		m_State = PLAYERSTATE::FALL;
		m_fFallAngle = 0.f;
		return;
	}


}

void CPlayer::Roll()
{
	m_fSpeed = 20.f;
	m_fUnitSpeed = 7.f;

	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		return;
	}
	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
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
//	bool b_isInPut = false; //키가 눌렸는지
//
//	if (m_pUnitInfo->iCollide == C_NONE && m_iActing == NOACTING) //충돌하고 있는 상태도 아니고 동작하고 있는 상태도 아니라면 낙하하라.
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
//	if ((GetAsyncKeyState('W') & 0X8000) && m_pUnitInfo->iCollide == C_LAND && m_State != PLAYERSTATE::JUMP) //점프는 한 번이라도 눌리면 계속 상태를 유지한다.
//	{
//		m_State = PLAYERSTATE::JUMP;
//		b_isInPut = true;
//	}
//
//
//	if (!b_isInPut && m_iActing == NOACTING && m_pUnitInfo->iCollide == C_LAND) //키 입력이 없고 딱히 하고 있는 동작이 없다면.
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
		Update_Frame();// 프레임 갱신하고
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
		m_pUnitInfo->wstrState = L"Attack";
		Update_Frame();
		Attack();
		if(GameObjectManager->Get_GameObjectlist(GAMEOBJECT::PLAYERATTACK).empty())
			GameObjectManager->Insert_GameObjectManager(CPlayerAttack::Create(this), GAMEOBJECT::PLAYERATTACK);
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
		m_pUnitInfo->wstrState = L"Flip";
		Update_Frame();
		Flip();
		break;
	case PLAYERSTATE::ROLL:
		m_pUnitInfo->wstrState = L"Roll";
		Update_Frame();
		Roll();
		break;
	case PLAYERSTATE::WALLSLIDE:
		m_pUnitInfo->wstrState = L"Wallslide";
		Update_Frame();
		Wallslide();
		break;
	case PLAYERSTATE::END:
		break;
	default:
		break;
	}
}

void CPlayer::Render_Pivot()
{
	swprintf_s(m_szPivot, L"Pivot x: %f y: %f", m_vecPivot.x, m_vecPivot.y);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 170.f, 0.f);

	Device->Get_Sprite()->SetTransform(&matTrans);
	Device->Get_Font()->DrawTextW(Device->Get_Sprite(), m_szPivot, lstrlen(m_szPivot), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Render_MousePos()
{
	swprintf_s(m_szMousePos, L"Mouse x: %f y: %f", m_vecMousePos.x, m_vecMousePos.y);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 190.f, 0.f);

	Device->Get_Sprite()->SetTransform(&matTrans);
	Device->Get_Font()->DrawTextW(Device->Get_Sprite(), m_szMousePos, lstrlen(m_szMousePos), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}





HRESULT CPlayer::Ready_GameObject()
{
	m_wstrOldState = m_pUnitInfo->wstrState;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fJumpDistance = 100.f;
	m_fSpeed = m_fDefaultSpeed;
	m_fRatio = 1.6f;
	m_iUnitDir = 1;
	m_vecPivot = { m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y + m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	m_fDefaultUnitSpeed = 5.f;
	m_fUnitSpeed = m_fDefaultUnitSpeed;
	return S_OK;
}

void CPlayer::Update_GameObject()
{		
	//Update_KeyInput(); //키 입력 체크
	m_fAttackLimit += TimeManager->Get_DeltaTime();
	Update_UnitState(); // 키 입력에 따른 상태 변화 // 이 단계에서 프레임 끝까지 봤냐 체크해야 하는데
 	Update_Frame(); // 만약 상태가 이전 상태와 다른 상태로 변했다면 프레임 갱신
	FrameMove(m_fSpeed); //현재 프레임 증가 또는 초기화
	Update_HitBox(); // 현재 유닛 기준으로 충돌 범위 업데이트
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

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld; 

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_vecPivot.x, m_vecPivot.y- m_fRatio*fCenterY, 0.f);
	matWorld = matScale * matRolateZ *matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (GetAsyncKeyState(VK_CONTROL) & 0X8000)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 255));
	}
	FrameManager->Render_Frame_Manager_FrameNum((size_t)m_tFrame.fFrameStart);
	FrameManager->Render_Frame_Manager_FrameName(m_pUnitInfo->wstrState);
	Render_HitBox();
	Render_Pivot();
	Render_MousePos();
	Render_ObbLine();
}

void CPlayer::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}

void CPlayer::Wallslide()
{
	m_fSpeed = 20.f;

	if (m_fJumpDistance > sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance && m_fJumpAngle != 0.f)//점프 속도가 점점 줄다가 최대 높이에 도달하는 순간 떨어지도록.
	{
		m_fOldJumpAngle = sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
		m_fJumpAngle += 1.5f;
		m_vecPivot.y += m_fOldJumpAngle;
		m_vecPivot.y -= sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance;
	}
	

	if (m_fJumpDistance <= sinf(3 * m_fJumpAngle / g_RADIAN) * m_fJumpDistance || m_fJumpAngle == 0.f)//점프 속도가 점점 줄다가 최대 높이에 도달하는 순간 떨어지도록.
	{
		float fFallSpeed = sinf(3 * m_fFallAngle / g_RADIAN) *7.f; //60프레임 기준 1초가 지났을 때 fFallSpeed는 7이 된다.
		if (3 * m_fFallAngle != 90.f)
			m_fFallAngle += 1.5f;

		m_vecPivot.y += fFallSpeed;
	}


	if ((GetAsyncKeyState(VK_LBUTTON) & 0X0001) && m_fAttackCool <= m_fAttackLimit)
	{
		m_State = PLAYERSTATE::ATTACK;
		m_fJumpAngle = 0.f;
		m_fFallAngle += 0.f;
		return;
	}

	if ((GetAsyncKeyState('W') & 0x0001))
	{
		m_State = PLAYERSTATE::FLIP;
		m_fJumpAngle = 0.f;
		m_fFallAngle += 0.f;
		return;
	}



	if (m_pUnitInfo->iCollide & C_NONE) //충돌 상태도 아니고, 동작 여부도 아니라면.
	{
		m_State = PLAYERSTATE::FALL;
		m_fJumpAngle += 0.f;
		m_fFallAngle += 0.f;
		return;
	}
	if (m_pUnitInfo->iCollide & C_LAND) 
	{
		m_State = PLAYERSTATE::IDLE;
		m_fJumpAngle = 0.f;
		m_fFallAngle += 0.f;
		return;
	}
}

