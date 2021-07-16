#include "stdafx.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "FrameManager.h"
#include "Graphic_Device.h"
#include "ScrollManager.h"

CGameObject::CGameObject()
	: m_pUnitInfo(nullptr)
	, m_tFrame({})
	, m_fSpeed(0)
	, m_State(PLAYERSTATE::IDLE)
	, m_wstrOldState(L"")
	, m_wstrCurState(L"")
	, m_fRatio(0.f)
	, m_iUnitDir(1)
	, m_vecPivot(0, 0, 0)
	, m_fUnitSpeed(0.f)
	, m_fDefaultUnitSpeed(0.f)
	, m_fRotateAngle(0.f)
	, m_fAttackLimit(0.7f)
	, m_fAttackCool(0.5f)
	, m_fTargetAngle(0.f)
	,m_iObjState(NONE)
	, m_GangState(GANGSTERSTATE::IDLE)
	,r(0)
	,g(0)
	,b(0)
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
	m_tHitBox.left = m_vecPivot.x - fCenterX*m_fRatio;
	m_tHitBox.top =    m_vecPivot.y - 2*fCenterY*m_fRatio;
	m_tHitBox.right =  m_vecPivot.x + fCenterX*m_fRatio;
	m_tHitBox.bottom = m_vecPivot.y;

}

void CGameObject::Update_HitBoxOBB()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"유닛 정보가 없습니다.");
		return;
	}
	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	D3DXVECTOR3				m_tHitBoxObbTemp[4];
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	//ratio 기본 배율은 유닛의 경우 2배
	m_tHitBoxObbTemp[0] = {-m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//왼쪽 위
	m_tHitBoxObbTemp[1] = { m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//오른쪽 위
	m_tHitBoxObbTemp[2] = { m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//오른쪽 아래
	m_tHitBoxObbTemp[3] = {-m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//왼쪽 아래
	D3DXMatrixScaling(&matScale, m_iUnitDir, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pUnitInfo->D3VecPos.x, m_pUnitInfo->D3VecPos.y, 0.f);
	matWorld = matScale * matRolateZ * matTrans;
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&m_tHitBoxObb[i], &m_tHitBoxObbTemp[i], &matWorld);
		//D3DXVec3TransformNormal(&m_tHitBoxObb[i], &m_tHitBoxObbTemp[i], &matWorld);
		m_tHitBoxObb[i];
	}

}

void CGameObject::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += FrameManager->Get_SPF() *(FrameManager->Get_FPS()/60)* fSpeed; //frame start end는 각 클래스에서 정해준다. 
	if ((size_t)m_tFrame.fFrameEnd <= (size_t)m_tFrame.fFrameStart)
		m_tFrame.fFrameStart = 0.f;
}

void CGameObject::Render_HitBox()
{
	int r = 255;
	int g = 255;
	int b = 255;
	switch (m_pUnitInfo->iCollide)
	{
	case C_NONE:
		r = 255;
		g = 255;
		b = 255;
		break;
	case C_LAND:
		r = 0;
		g = 255;
		b = 0;
		break;
	case C_WALL|C_WALLL:
		r = 255;
		g = 0;
		b = 0;
		break;
	case C_WALL | C_WALLR:
		r = 255;
		g = 0;
		b = 0;
		break;
	case C_CELLING:
		r = 0;
		g = 0;
		b = 255;
		break;
	case C_LAND| C_WALL | C_WALLL:
		r = 255;
		g = 255;
		b = 0;
		break;
	case C_LAND | C_WALL | C_WALLR:
		r = 255;
		g = 255;
		b = 0;
		break;
	case C_WALL | C_WALLL | C_CELLING:
		r = 255;
		g = 0;
		b = 255;
		break;
	case C_WALL | C_WALLR| C_CELLING:
		r = 255;
		g = 0;
		b = 255;
		break;
	default:
		m_pUnitInfo->iCollide = C_NONE;
		break;
	}
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(1.f);
	D3DXVECTOR2	vLine[5]{ { (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.right - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.right - CScrollManager::Get_ScroolX(), (float)m_tHitBox.bottom - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.bottom - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine, 5, D3DCOLOR_ARGB(255, r, g ,b));
	D3DXVECTOR2	vLine2[2]{ {(float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)m_vecPivot.y - CScrollManager::Get_ScroolY() },{ (float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)m_vecPivot.y - 80.f - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGameObject::Render_HitBoxObb()
{
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(1.f);
	D3DXVECTOR2	vLine[5]{ {m_tHitBoxObb[0].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[0].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[1].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[1].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[2].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[2].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[3].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[3].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[0].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[0].y - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine, 5, D3DCOLOR_ARGB(255, 175, 255, 175));
	
	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGameObject::Render_ObbLine()
{
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(1.f);
	D3DXVECTOR2	vLine2[2]{ { (float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)(m_vecPivot.y -m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Height>>1)) - CScrollManager::Get_ScroolY() }
	,{ (float)(m_vecPivot.x - CScrollManager::Get_ScroolX() + m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Width >> 1)) , (float)(m_vecPivot.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1)) - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 255));;
	Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGameObject::Render_ObbLineD3D()
{
	Device->m_pSprite->End();
	Device->m_pLine->SetWidth(1.f);
	D3DXVECTOR2	vLine[2]{ {m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle ))*50 - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle ))*50 - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 0, 255));
	D3DXVECTOR2	vLine2[2]{ { m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle + 90)) * 50 - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle + 90)) * 50 - CScrollManager::Get_ScroolY() } };
	Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 255));
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

bool CGameObject::Check_FrameEnd() //다음 상태로 변경하기 전 준비 동작을 마쳤는지 체크
{
	static bool FrameCheck = false;
	
	if (FrameCheck) // 준비 동작이 끝났으면
	{
		FrameCheck = false; // 준비동작 끝났음을 공지했으면 다시 false 상태로.
		return true;
	}
	if ((size_t)m_tFrame.fFrameEnd-1 == (size_t)m_tFrame.fFrameStart) // 현재 프레임과 끝 프레임이 같다면 준비 동작이 끝난 것
	{
		FrameCheck = true; 
		return false; // 현재 프레임까지 그리고 나서 바꿔야 하기에 false를 반환
	}
	return false;
}




