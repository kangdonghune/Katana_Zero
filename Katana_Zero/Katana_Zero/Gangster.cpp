#include "stdafx.h"
#include "Gangster.h"
#include "GameObject.h";
#include "Enemy.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "Bullet.h"
#include "ColliderManager.h"
#include "MapObjectManager.h"
#include "ScrollManager.h"
#include "FrameManager.h"

CGangster::CGangster()
{
}


CGangster::~CGangster()
{
	Release_GameObject();
}

CGameObject * CGangster::Create(CGameObject * pTarget, UNITINFO* pInfo)
{
	CGangster* pGang = new CGangster();
	pGang->Set_Target(pTarget);
	pGang->Set_Info(pInfo);
	if (FAILED(pGang->Ready_GameObject()))
	{
		Safe_Delete(pGang);
		return pGang;
	}
	return pGang;
}

void CGangster::Idle()
{
	//주위 일정 거리내 플레이어 접근 시 aim 상태로
	
	
	
	
}

void CGangster::Aim()
{
}

void CGangster::Enterstair()
{
}

void CGangster::Fall()
{
}

void CGangster::Hurtfly()
{
}

void CGangster::Hurtground()
{
}

void CGangster::Leavestair()
{
}

void CGangster::Run()
{
}

void CGangster::Turn()
{
}

void CGangster::Walk()
{
}

void CGangster::Whip()
{
}

void CGangster::Shooting()
{
	if (Ray_Traising())
	{
		m_fAttackLimit += TimeManager->Get_DeltaTime()*(FrameManager->Get_FPS() / 60);
		if (m_fAttackLimit >= m_fAttackCool)
		{
			m_fAttackLimit = 0.f;
			GameObjectManager->Insert_GameObjectManager(CBullet::Create(this), GAMEOBJECT::BULLET);
		}
	}
	else
	{
		m_fAttackLimit = 0.f;
	}
}

void CGangster::Update_TargetRotate()
{
	D3DXVECTOR3 TargetPos = { m_pTarget->Get_Pivot().x,m_pTarget->Get_Pivot().y - m_pTarget->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pTarget->Get_UnitInfo()->wstrKey,m_pTarget->Get_UnitInfo()->wstrState,0)->tImageInfo.Height >> 1) ,0 };
	D3DXVECTOR3 MyPos = { m_vecPivot.x,  m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	D3DXVECTOR3 vDir = TargetPos - MyPos;
	D3DXVECTOR3 vLook{ 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	m_iUnitDir = 1;

	float fCos = D3DXVec3Dot(&vDir, &vLook);
	m_fRotateAngle = D3DXToDegree(acosf(fCos));
	if (MyPos.y <= TargetPos.y)
		m_fRotateAngle *= -1.f;
	m_fTargetAngle = m_fRotateAngle;
	m_fRotateAngle = 360 - m_fRotateAngle;
	if (TargetPos.x <= MyPos.x)//0~90 -1 angle 270 360 1 90~180 1 180~270 -1
	{
		m_iUnitDir = -1;
		m_fRotateAngle = 180 + m_fRotateAngle;
	}
}

void CGangster::Update_D3DPos()
{
	m_pUnitInfo->D3VecPos.x = m_vecPivot.x;
	m_pUnitInfo->D3VecPos.y = m_vecPivot.y - m_fRatio * (Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, 0)->tImageInfo.Height >> 1);
}

void CGangster::Update_UnitState()
{	
	switch (m_GangState)
	{
	case GANGSTERSTATE::IDLE:
		m_pUnitInfo->wstrState = L"Idle";
		Update_Frame();
		break;
	case GANGSTERSTATE::AIM:
		m_pUnitInfo->wstrState = L"Aim";
		Update_Frame();
		break;
	case GANGSTERSTATE::ENTERSTAIR:
		m_pUnitInfo->wstrState = L"Enterstair";
		Update_Frame();
		break;
	case GANGSTERSTATE::FALL:
		m_pUnitInfo->wstrState = L"Fall";
		Update_Frame();
		break;
	case GANGSTERSTATE::HURTFLY:
		m_pUnitInfo->wstrState = L"Hurtfly";
		Update_Frame();
		break;
	case GANGSTERSTATE::HURTGROUND:
		m_pUnitInfo->wstrState = L"HurtGround";
		Update_Frame();
		break;
	case GANGSTERSTATE::LEAVESTAIR:
		m_pUnitInfo->wstrState = L"Leavestair";
		Update_Frame();
		break;
	case GANGSTERSTATE::RUN:
		m_pUnitInfo->wstrState = L"Run";
		Update_Frame();
		break;
	case GANGSTERSTATE::TURN:
		m_pUnitInfo->wstrState = L"Turn";
		Update_Frame();
		break;
	case GANGSTERSTATE::WALK:
		m_pUnitInfo->wstrState = L"Walk";
		Update_Frame();
		break;
	case GANGSTERSTATE::WHIP:
		m_pUnitInfo->wstrState = L"Whip";
		Update_Frame();
		break;
	default:
		break;
	}

}

void CGangster::Render_Raytraise()
{
	D3DXVECTOR2	vLine2[2]{ { m_pTarget->Get_UnitInfo()->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pTarget->Get_UnitInfo()->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, r, g, b));
}

bool CGangster::Ray_Traising()
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

HRESULT CGangster::Ready_GameObject()
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
	m_fAttackLimit = 0.f;
	m_fAttackCool = 1.f;
	m_pUnitInfo->wstrState = L"Aim";
	m_fTargetAngle = 0.f;
	return S_OK;
}

void CGangster::Update_GameObject()
{
	Update_Frame(); // 만약 상태가 이전 상태와 다른 상태로 변했다면 프레임 갱신
	FrameMove(m_fSpeed); //현재 프레임 증가 또는 초기화
	Update_TargetRotate();
	Update_HitBox(); // 현재 유닛 기준으로 충돌 범위 업데이트
	Update_D3DPos();
	Shooting();
	

}

void CGangster::LateUpdate_GameObject()
{
}

void CGangster::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"갱스터 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);

	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Gangster에서 텍스쳐 찾기 실패");
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
	if (m_pUnitInfo->wstrState == L"Aim")
	{
		const TEXINFO* pArmTex = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, L"arm", 0);
		const TEXINFO* pGun = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, L"Gangstergun", 0);
		D3DXMATRIX matGunScale, matGunRolateZ, matGunTrans, matGunWorld;
		float fGunCenterX = pGun->tImageInfo.Width >> 1;
		float fGunCenterY = pGun->tImageInfo.Height >> 1;
		D3DXMatrixScaling(&matGunScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
		D3DXMatrixRotationZ(&matGunRolateZ, D3DXToRadian(m_fRotateAngle));
		D3DXMatrixTranslation(&matGunTrans, m_vecPivot.x - CScrollManager::Get_ScroolX(), m_vecPivot.y - m_fRatio*fCenterY - CScrollManager::Get_ScroolY(), 0.f);
		matGunWorld = matGunScale * matGunRolateZ *matGunTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matGunWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pGun->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		if (GetAsyncKeyState(VK_CONTROL) & 0X8001)
		{
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matGunWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pGun->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 150, 150, 150));
		}


		D3DXMATRIX matArmScale, matArmRolateZ, matArmTrans, matArmWorld;
		float fArmCenterX = pArmTex->tImageInfo.Width >> 1;
		float fArmCenterY = pArmTex->tImageInfo.Height >> 1;
		D3DXMatrixScaling(&matArmScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
		D3DXMatrixRotationZ(&matArmRolateZ, D3DXToRadian(m_fRotateAngle));
		D3DXMatrixTranslation(&matArmTrans, m_vecPivot.x - m_iUnitDir * m_fRatio*fArmCenterX - CScrollManager::Get_ScroolX(), m_vecPivot.y - m_fRatio*fCenterY - CScrollManager::Get_ScroolY(), 0.f);
		matArmWorld = matArmScale * matArmRolateZ *matArmTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matArmWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pArmTex->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		if (GetAsyncKeyState(VK_CONTROL) & 0X8001)
		{
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matArmWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pArmTex->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 150, 150, 150));
		}
	}

	Render_Raytraise();
	Render_HitBox();
	Render_ObbLine();
}

void CGangster::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}
