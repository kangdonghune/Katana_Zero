#include "stdafx.h"
#include "Boss.h"
#include "GameObject.h";
#include "Enemy.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "ColliderManager.h"
#include "MapObjectManager.h"
#include "ScrollManager.h"
#include "FrameManager.h"
#include "Bullet.h"
#include "Laser.h"
#include "Granade.h"
#include "Explosion.h"
#include <random>
CBoss::CBoss()
	:m_pLaser(nullptr)
	, m_pGranade(nullptr)
	,m_fJumpAngle(0.f)
{
}


CBoss::~CBoss()
{
	Release_GameObject();
}

CGameObject * CBoss::Create(CGameObject * pTarget, UNITINFO * pInfo)
{
	CBoss* pBoss = new CBoss();
	pBoss->Set_Target(pTarget);
	pBoss->Set_Info(pInfo);
	if (FAILED(pBoss->Ready_GameObject()))
	{
		Safe_Delete(pBoss);
		return pBoss;
	}
	return pBoss;
}

HRESULT CBoss::Ready_GameObject()
{
	m_wstrOldState = m_pUnitInfo->wstrState;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
	m_fSpeed = 10.f;
	m_fRatio = 1.6f;
	m_iUnitDir = 1;
	m_vecPivot = { m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y + m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	m_fDefaultUnitSpeed = 5.f;
	m_fUnitSpeed = m_fDefaultUnitSpeed;
	m_pUnitInfo->wstrState = L"Idle";
	m_BossState = BOSSSTATE::IDLE;
	m_fTargetAngle = 0.f;
	ShotLine[0] = {};
	ShotLine[1] = {};
	m_fCenter = MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(0).Start.x + (MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(1).Start.x - MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(0).Start.x)/2;
	return S_OK;
}

void CBoss::Update_GameObject()
{
	Update_D3DPos();
	Update_HitBox();
	Update_UnitState();
	FrameMove(m_fSpeed); //현재 프레임 증가 또는 초기화
	Update_TargetRotate();
	Ray_Traising();
}

void CBoss::LateUpdate_GameObject()
{
}

void CBoss::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"보스 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);

	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Boss에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	//D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_vecPivot.x - CScrollManager::Get_ScroolX(), m_vecPivot.y - m_fRatio*fCenterY - CScrollManager::Get_ScroolY(), 0.f);
	matWorld = matScale  *matTrans;
	//matWorld = matScale * matRolateZ *matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (GetAsyncKeyState(VK_CONTROL) & 0X8001)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 150, 150, 150));
	}
	Render_Raytraise();
	Render_HitBox();
	Render_ObbLine();
	Render_shotLine();
}

void CBoss::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}

void CBoss::Update_TargetRotate()
{
	D3DXVECTOR3 TargetPos = { m_pTarget->Get_Pivot().x,m_pTarget->Get_Pivot().y - m_pTarget->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pTarget->Get_UnitInfo()->wstrKey,m_pTarget->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1) ,0 };
	D3DXVECTOR3 MyPos = { m_vecPivot.x,  m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	D3DXVECTOR3 vDir = TargetPos - MyPos;
	D3DXVECTOR3 vLook{ 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	//m_iUnitDir = 1;

	float fCos = D3DXVec3Dot(&vDir, &vLook);
	m_fRotateAngle = D3DXToDegree(acosf(fCos));
	if (MyPos.y <= TargetPos.y)
		m_fRotateAngle *= -1.f;
	m_fTargetAngle = m_fRotateAngle;
	m_fRotateAngle = 360 - m_fRotateAngle;
	if (TargetPos.x <= MyPos.x)//0~90 -1 angle 270 360 1 90~180 1 180~270 -1
	{
		//m_iUnitDir = -1;
		m_fRotateAngle = 180 + m_fRotateAngle;
	}
}

void CBoss::Update_D3DPos()
{
	m_pUnitInfo->D3VecPos.x = m_vecPivot.x;
	m_pUnitInfo->D3VecPos.y = m_vecPivot.y - m_fRatio * (Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Height >> 1);
}

void CBoss::Update_UnitState()
{
	switch (m_BossState)
	{
	case BOSSSTATE::AIM:
		m_pUnitInfo->wstrState = L"Aim";
		Update_Frame();
		Aim();
		break;
	case BOSSSTATE::DASH:
		m_pUnitInfo->wstrState = L"Dash";
		Update_Frame();
		break;
	case BOSSSTATE::DASH_END:
		m_pUnitInfo->wstrState = L"Dashend";
		Update_Frame();
		break;
	case BOSSSTATE::DASH_SLOWMOTION:
		m_pUnitInfo->wstrState = L"Dashslow";
		Update_Frame();
		break;
	case BOSSSTATE::BDEAD:
		m_pUnitInfo->wstrState = L"Dead";
		Update_Frame();
		break;
	case BOSSSTATE::HURT:
		m_pUnitInfo->wstrState = L"Hurt";
		Update_Frame();
		break;
	case BOSSSTATE::IDLE:
		m_pUnitInfo->wstrState = L"Idle";
		Update_Frame();
		Idle();
		break;
	case BOSSSTATE::PREDASH:
		m_pUnitInfo->wstrState = L"Predash";
		Update_Frame();
		break;
	case BOSSSTATE::PREJUMP:
		m_pUnitInfo->wstrState = L"Prejump";
		Update_Frame();
		Prejump();
		break;
	case BOSSSTATE::PUTBACK:
		m_pUnitInfo->wstrState = L"Putback";
		Update_Frame();
		Putback();
		break;
	case BOSSSTATE::RECOVER:
		m_pUnitInfo->wstrState = L"Recover";
		Update_Frame();
		break;
	case BOSSSTATE::ROLL:
		m_pUnitInfo->wstrState = L"Roll";
		Update_Frame();
		break;
	case BOSSSTATE::SWEEP:
		m_pUnitInfo->wstrState = L"Sweep";
		Update_Frame();
		break;
	case BOSSSTATE::TAKEOUT:
		m_pUnitInfo->wstrState = L"Takeout";
		Update_Frame();
		Takeout();
		break;
	case BOSSSTATE::TELEPORT_IN:
		m_pUnitInfo->wstrState = L"Teleportin";
		Update_Frame();
		break;
	case BOSSSTATE::TELEPORT_OUT:
		m_pUnitInfo->wstrState = L"Teleportout";
		Update_Frame();
		break;
	case BOSSSTATE::TELEPORT_IN_SWEEP:
		m_pUnitInfo->wstrState = L"Teleportinsweep";
		Update_Frame();
		break;
	case BOSSSTATE::TELEPORT_OUT_SWEEP:
		m_pUnitInfo->wstrState = L"Teleportoutsweep";
		Update_Frame();
		break;
	case BOSSSTATE::TELEPORT_IN_GROUND:
		m_pUnitInfo->wstrState = L"Teleportinground";
		Update_Frame();
		break;
	case BOSSSTATE::TELEPORT_OUT_GROUND:
		m_pUnitInfo->wstrState = L"Teleportoutground";
		Update_Frame();
		break;
	case BOSSSTATE::Jump:
		m_pUnitInfo->wstrState = L"Jump";
		Update_Frame();
		Jump();
		break;
	case BOSSSTATE::WALLJUMP:
		m_pUnitInfo->wstrState = L"Walljump";
		Update_Frame();
		Walljump();
		break;
	case BOSSSTATE::WALLJUMP_LAND:
		m_pUnitInfo->wstrState = L"Walljumpland";
		Update_Frame();
		break;
	default:
		break;
	}
}

void CBoss::Render_Raytraise()
{
	D3DXVECTOR2	vLine2[2]{ { m_pTarget->Get_UnitInfo()->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pTarget->Get_UnitInfo()->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, r, g, b));
}

bool CBoss::Ray_Traising()
{
	for (int i = 0; i < TERRAINTYPE::END; i++)
	{

		r = 255;
		g = 0;
		b = 0;
		if (ColliderManager->Collide_TerrainAndRay(MapObjectManager->Get_TerrainVector(i), this))
		{
			r = 0;
			g = 255;
			b = 0;
			return false;
		}
	}
	return true;
}

void CBoss::Render_shotLine()
{
	if (m_BossState == BOSSSTATE::TAKEOUT)
	{
		D3DXVECTOR2	vLine[2]{ { (float)ShotLine[0].x,(float)ShotLine[0].y },{ (float)ShotLine[1].x,(float)ShotLine[1].y } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 255, 153));
	}
}

void CBoss::Shoot_Laser()
{

	if (m_iUnitDir == -1)
	{
		ShotLine[0].x = m_tHitBox.left;
		ShotLine[0].y = m_tHitBox.bottom - ((m_tHitBox.bottom - m_tHitBox.top) * 3.5 / 5) - CScrollManager::Get_ScroolY();
		ShotLine[1].x = m_tHitBox.left - 1050;
		ShotLine[1].y = m_tHitBox.bottom - ((m_tHitBox.bottom - m_tHitBox.top) * 3.5 / 5) - CScrollManager::Get_ScroolY();
	}

	if (m_iUnitDir == 1)
	{
		ShotLine[0].x = m_tHitBox.right;
		ShotLine[0].y = m_tHitBox.bottom - ((m_tHitBox.bottom - m_tHitBox.top) *3.5 / 5) - CScrollManager::Get_ScroolY();
		ShotLine[1].x = m_tHitBox.right + 1050;
		ShotLine[1].y = m_tHitBox.bottom - ((m_tHitBox.bottom - m_tHitBox.top) * 3.5 / 5) - CScrollManager::Get_ScroolY();
	}
	if (m_pLaser == nullptr)
	{
		m_pLaser = CLaser::Create(this);
		if (m_iUnitDir == -1)
			m_pLaser->Set_Pos((ShotLine[1].x + (ShotLine[0].x - ShotLine[1].x) / 2), ShotLine[0].y + CScrollManager::Get_ScroolY());
		if (m_iUnitDir == 1)
			m_pLaser->Set_Pos((ShotLine[0].x + (ShotLine[1].x - ShotLine[0].x) / 2), ShotLine[0].y + CScrollManager::Get_ScroolY());
		GameObjectManager->Insert_GameObjectManager(m_pLaser, GAMEOBJECT::LASER);
	}
	static float ShotCool = 0.3f;
	static float Shotlimit = 0.f;
	if (Shotlimit <= ShotCool)
	{
		Shotlimit += TimeManager->Get_DeltaTime() *(FrameManager->Get_FPS() / 60);
	}
	if (Shotlimit > ShotCool)
	{
		ShotLine[0] = {};
		ShotLine[1] = {};
		static float fRatio = 0;
		static bool	 bEnd = false;
		if (fRatio <= 20 && !bEnd)
			fRatio += 2.f;
		if (fRatio > 20)
			bEnd = true;
		if (bEnd)
			fRatio -= 2.f;
		m_pLaser->Set_WidthRatio(31.f);
		m_pLaser->Set_HeightRatio(fRatio);
		if (fRatio <= 0)
		{
			bEnd = false;
			Shotlimit = 0.f;
			m_pLaser->Set_ObjState(DEAD);
			m_pLaser = nullptr;
			m_BossState = BOSSSTATE::PUTBACK;
			ShotLine[0] = {};
			ShotLine[1] = {};
			return;
		}
	}
	
}

void CBoss::Shoot_Granade()
{
	if (m_pGranade == nullptr)
	{
		m_pGranade = CGranade::Create(this);
		GameObjectManager->Insert_GameObjectManager(m_pGranade, GAMEOBJECT::GRANADE);
	}
	static float BoobCool = 1.5f;
	static float BoobCount = 0.f;
	static float BoobMoveCount = 0.f;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dir(0.4f, 0.8f);
	static float BoobMoveLimit = dir(gen);
	
	if (BoobMoveCount <= BoobMoveLimit)
	{
		BoobMoveCount += TimeManager->Get_DeltaTime() *(FrameManager->Get_FPS() / 60);
	}
	if (BoobMoveCount > BoobMoveLimit)
	{
		m_pGranade->Set_UnitSpeed(0.f);
		BoobCount += TimeManager->Get_DeltaTime() *(FrameManager->Get_FPS() / 60);
		if (BoobCount >= BoobCool)
		{
			GameObjectManager->Insert_GameObjectManager(CExplosion::Create(m_pGranade), GAMEOBJECT::EFFECT);
			BoobMoveCount = 0.f;
			BoobMoveLimit = dir(gen);
			BoobCount = 0.f;
			m_pGranade->Set_ObjState(DEAD);
			m_pGranade = nullptr;
			m_BossState = BOSSSTATE::PUTBACK;
			return;
		}
	}


}

void CBoss::AngleToLWall()
{
	D3DXVECTOR3 TargetPos = { (float)MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(1).Start.x,(float)MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(1).Start.y + 60.f, 0.f };
	D3DXVECTOR3 MyPos = { m_vecPivot.x,  m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	D3DXVECTOR3 vDir = TargetPos - MyPos;
	D3DXVECTOR3 vLook{ 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	//m_iUnitDir = 1;

	float fCos = D3DXVec3Dot(&vDir, &vLook);
	m_fJumpAngle = D3DXToDegree(acosf(fCos));
	if (MyPos.y <= TargetPos.y)
		m_fJumpAngle *= -1.f;
}

void CBoss::AngleToRWall()
{
	D3DXVECTOR3 TargetPos = { (float)MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(0).Start.x, (float)MapObjectManager->Get_TerrainVector(TERRAINTYPE::WALL).at(0).Start.y + 60.f, 0.f };
	D3DXVECTOR3 MyPos = { m_vecPivot.x,  m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	D3DXVECTOR3 vDir = TargetPos - MyPos;
	D3DXVECTOR3 vLook{ 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	//m_iUnitDir = 1;

	float fCos = D3DXVec3Dot(&vDir, &vLook);
	m_fJumpAngle = D3DXToDegree(acosf(fCos));
	if (MyPos.y <= TargetPos.y)
		m_fJumpAngle *= -1.f;
}

void CBoss::Aim()
{

}

void CBoss::Dash()
{
}

void CBoss::Dash_end()
{
}

void CBoss::Dash_slowmotion()
{
}

void CBoss::Dead()
{
}

void CBoss::Hurt()
{
}

void CBoss::Idle()
{
	m_fSpeed = 10.f;
	m_iUnitDir = 1;
	if(m_vecPivot.x >= m_pTarget->Get_UnitInfo()->D3VecPos.x)
		m_iUnitDir = -1;

	m_BossState = BOSSSTATE::PREJUMP;

}

void CBoss::Predash()
{
}

void CBoss::Prejump()
{
	m_fSpeed = 5.f;
	if (Check_FrameEnd())
		m_BossState = BOSSSTATE::Jump;
}

void CBoss::Putback()
{
	m_fSpeed = 10.f;
	if (Check_FrameEnd())
		m_BossState = BOSSSTATE::IDLE;
}

void CBoss::Recover()
{
}

void CBoss::Roll()
{
}

void CBoss::Sweep()
{
}

void CBoss::Takeout()
{
	if (Check_FrameEnd())
	{
		m_fSpeed = 0.f;
		//Shoot_Laser();
		Shoot_Granade();
	}
	


}


void CBoss::Teleport_in()
{
}

void CBoss::Teleport_out()
{
}

void CBoss::Teleport_in_sweep()
{
}

void CBoss::Teleport_out_sweep()
{
}

void CBoss::Teleport_in_ground()
{
}

void CBoss::Teleport_out_ground()
{
}

void CBoss::Jump()
{
	if(m_vecPivot.x >= m_fCenter)
		AngleToRWall();
	else
		AngleToLWall();
	m_fUnitSpeed = 13.f;
	
	m_vecPivot.x += cosf(D3DXToRadian(m_fJumpAngle))*m_fUnitSpeed;
	m_vecPivot.y -= sinf(D3DXToRadian(m_fJumpAngle))*m_fUnitSpeed;
}

void CBoss::Walljump()
{
}

void CBoss::Walljump_land()
{
}
