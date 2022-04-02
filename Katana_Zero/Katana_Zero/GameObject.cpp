#include "stdafx.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "FrameManager.h"
#include "Graphic_Device.h"
#include "ScrollManager.h"
#include "MapObjectManager.h"

CGameObject::CGameObject()
	: m_pUnitInfo(nullptr)
	, m_pItemInfo(nullptr)
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
	, m_iObjState(NONE)
	, m_GangState(GANGSTERSTATE::IDLE)
	, r(0)
	, g(0)
	, b(0)
	, m_fTargetDist(10000)
	, m_fHitSpeed(0.f)
	, m_fHitAngle(0.f)
	, m_iHitDir(1)
	, m_tCurLand({})
	,m_tOldLand({})
	, m_NextLine({})
	, m_NowLine({})
{
	m_tCurLand.type = TERRAINTYPE::END;
	m_NextLine.type = TERRAINTYPE::END;
	m_NowLine.type = TERRAINTYPE::END;
}


CGameObject::~CGameObject()
{

}

void CGameObject::Update_HitBox()
{
	if (nullptr == m_pUnitInfo)  
	{
		ERR_MSG(L"오브젝트 정보가 없습니다.");
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

void CGameObject::Update_HitBoxLaser()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"유닛 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans,matParent, matWorld;

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
	m_tHitBoxObbTemp[0] = { 0.f, -m_fRatio*fCenterY, 0.f };//왼쪽 위
	m_tHitBoxObbTemp[1] = { m_fRatio * 2 * fCenterX, -m_fRatio*fCenterY, 0.f };//오른쪽 위
	m_tHitBoxObbTemp[2] = { m_fRatio * 2 * fCenterX,  m_fRatio*fCenterY, 0.f };//오른쪽 아래
	m_tHitBoxObbTemp[3] = { 0.f,  m_fRatio*fCenterY, 0.f };//왼쪽 아래
	D3DXMatrixScaling(&matScale, m_iUnitDir, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_iUnitDir * 30, 0, 0.f);
	D3DXMatrixTranslation(&matParent, m_pShooter->Get_UnitInfo()->D3VecPos.x, m_pShooter->Get_UnitInfo()->D3VecPos.y, 0.f);
	matWorld = matScale*matTrans*matRolateZ*matParent;
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&m_tHitBoxObb[i], &m_tHitBoxObbTemp[i], &matWorld);
		//D3DXVec3TransformNormal(&m_tHitBoxObb[i], &m_tHitBoxObbTemp[i], &matWorld);
		m_tHitBoxObb[i];
	}
}

void CGameObject::Update_ProjectileHitBoxOBB()
{
	if (nullptr == m_pItemInfo)
	{
		ERR_MSG(L"투사체 정보가 없습니다.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pItemInfo->wstrKey, m_pItemInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	D3DXVECTOR3				m_tHitBoxObbTemp[4];
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player에서 텍스쳐 찾기 실패");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	//ratio 기본 배율은 유닛의 경우 2배
	m_tHitBoxObbTemp[0] = { -m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//왼쪽 위
	m_tHitBoxObbTemp[1] = { m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//오른쪽 위
	m_tHitBoxObbTemp[2] = { m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//오른쪽 아래
	m_tHitBoxObbTemp[3] = { -m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//왼쪽 아래
	D3DXMatrixScaling(&matScale, m_iUnitDir, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRolateZ, D3DXToRadian(m_fRotateAngle));
	D3DXMatrixTranslation(&matTrans, m_pItemInfo->D3VecPos.x, m_pItemInfo->D3VecPos.y, 0.f);
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

void CGameObject::FrameMoveIndependence(float fSpeed)
{
	m_tFrame.fFrameStart += FrameManager->Get_SPF() * fSpeed; //frame start end는 각 클래스에서 정해준다. 
	if ((size_t)m_tFrame.fFrameEnd <= (size_t)m_tFrame.fFrameStart)
		m_tFrame.fFrameStart = 0.f;
}

void CGameObject::Render_HitBox()
{
	if (m_pUnitInfo == nullptr)
		return;

	if (GetAsyncKeyState(VK_SPACE) & 0X8001)
	{
	
		int r = 0;
			int g = 0;
			int b = 0;
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
			case C_WALL | C_CELLING:
				r = 255;
				g = 0;
				b = 100;
				break;
			case C_WALL | C_WALLL:
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

			case C_PASSABLE:
				r = 170;
				g = 50;
				b = 255;
				break;

			case C_LAND | C_WALL | C_WALLL:
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
			case C_WALL | C_WALLR | C_CELLING:
				r = 255;
				g = 0;
				b = 255;
				break;
			default:
				break;
			}
		Device->m_pSprite->End();
		Device->m_pLine->SetWidth(1.f);
		D3DXVECTOR2	vLine[5]{ { (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.right - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.right - CScrollManager::Get_ScroolX(), (float)m_tHitBox.bottom - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.bottom - CScrollManager::Get_ScroolY() } ,{ (float)m_tHitBox.left - CScrollManager::Get_ScroolX(), (float)m_tHitBox.top - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine, 5, D3DCOLOR_ARGB(255, r, g, b));
		D3DXVECTOR2	vLine2[2]{ {(float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)m_vecPivot.y - CScrollManager::Get_ScroolY() },{ (float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)m_vecPivot.y - 80.f - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
		Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	
}

void CGameObject::Render_HitBoxObb()
{
	if (m_pUnitInfo == nullptr)
		return;

	if (GetAsyncKeyState(VK_SPACE) & 0X8001)
	{
		Device->m_pSprite->End();
		Device->m_pLine->SetWidth(1.f);
		D3DXVECTOR2	vLine[5]{ {m_tHitBoxObb[0].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[0].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[1].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[1].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[2].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[2].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[3].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[3].y - CScrollManager::Get_ScroolY() },{ m_tHitBoxObb[0].x - CScrollManager::Get_ScroolX(),m_tHitBoxObb[0].y - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine, 5, D3DCOLOR_ARGB(255, 175, 255, 175));

		Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
}

void CGameObject::Render_ObbLine()
{
	if (GetAsyncKeyState(VK_SPACE) & 8001)
	{
		Device->m_pSprite->End();
		Device->m_pLine->SetWidth(1.f);
		D3DXVECTOR2	vLine2[2]{ { (float)m_vecPivot.x - CScrollManager::Get_ScroolX(), (float)(m_vecPivot.y - m_fRatio * (Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1)) - CScrollManager::Get_ScroolY() }
		,{ (float)(m_vecPivot.x - CScrollManager::Get_ScroolX() + m_fRatio * (Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Width >> 1)) , (float)(m_vecPivot.y - m_fRatio * (Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey,m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1)) - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 255));;
		Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	
}

void CGameObject::Render_ObbLineD3D()
{
	if (m_pUnitInfo == nullptr)
		return;

	if (GetAsyncKeyState(VK_SPACE) & 0X8001)
	{
		Device->m_pSprite->End();
		Device->m_pLine->SetWidth(1.f);
		D3DXVECTOR2	vLine[2]{ {m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle)) * 50 - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle)) * 50 - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine, 2, D3DCOLOR_ARGB(255, 255, 0, 255));
		D3DXVECTOR2	vLine2[2]{ { m_pUnitInfo->D3VecPos.x - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - CScrollManager::Get_ScroolY() },{ m_pUnitInfo->D3VecPos.x + cosf(D3DXToRadian(m_fTargetAngle + 90)) * 50 - CScrollManager::Get_ScroolX(), m_pUnitInfo->D3VecPos.y - sinf(D3DXToRadian(m_fTargetAngle + 90)) * 50 - CScrollManager::Get_ScroolY() } };
		Device->m_pLine->Draw(vLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 255));
		Device->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
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

	if ((size_t)m_tFrame.fFrameEnd <= (size_t)m_tFrame.fFrameStart +1) // 현재 프레임과 끝 프레임이 같다면 준비 동작이 끝난 것
	{
		return true; // 현재 프레임까지 그리고 나서 바꿔야 하기에 false를 반환
	}
	return false;
}

void CGameObject::Update_TargetDist()
{
	float x = m_pUnitInfo->D3VecPos.x - m_pTarget->Get_UnitInfo()->D3VecPos.x;
	float y = m_pUnitInfo->D3VecPos.y - m_pTarget->Get_UnitInfo()->D3VecPos.y;
	m_fTargetDist = sqrtf(x*x + y*y);

}

void CGameObject::Update_CurLand()
{

}

bool CGameObject::Find_Root(const MYLINE StartLine, const MYLINE TargetLine)
{

	m_lstRoot.clear();
	m_vecID.clear();
	if (TargetLine.ID == StartLine.ID)
	{
		m_lstRoot.emplace_front(StartLine);
		return true;
	}
	if (RotateRootL(StartLine, TargetLine))
	{
		m_lstRoot.emplace_front(StartLine);
		return true;
	}
	if (RotateRootR(StartLine, TargetLine))
	{
		m_lstRoot.emplace_front(StartLine);
		return true;
	}
	return false;
}

bool CGameObject::RotateRootL(const MYLINE StartLine, const MYLINE TargetLine)
{
	if(m_vecID.empty())
		m_vecID.push_back(StartLine.ID);
	if(!Check_ID(StartLine.ID))
		m_vecID.push_back(StartLine.ID);
	for (auto& tLine : MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND))
	{
		if (Check_ID(tLine.ID))
			continue;
		if ((tLine.End.x == StartLine.Start.x) && (tLine.End.y == StartLine.Start.y)) // 왼쪽점과 오른쪽 점이 겹치는 두 선
		{
			if (tLine.ID == TargetLine.ID)
			{
				m_lstRoot.emplace_front(tLine);//경로 리스트 앞에 추가
				return true;
			}
				
			else
			{
				if (RotateRootL(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
				if(RotateRootR(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
			}
		}
	}
	for (auto& tLine : MapObjectManager->Get_TerrainVector(TERRAINTYPE::PASSABLE))
	{
		if (Check_ID(tLine.ID))
			continue;
		if ((tLine.End.x == StartLine.Start.x) && (tLine.End.y == StartLine.Start.y)) // 왼쪽점과 오른쪽 점이 겹치는 두 선
		{
			if (tLine.ID == TargetLine.ID)
			{
				m_lstRoot.emplace_front(tLine);
				return true;
			}
			else
			{
				if (RotateRootL(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
				if (RotateRootR(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
			}
		}
	}
	return false;
}

bool CGameObject::RotateRootR(MYLINE StartLine, MYLINE TargetLine)
{
	if (m_vecID.empty())
		m_vecID.push_back(StartLine.ID);
	if (!Check_ID(StartLine.ID))
		m_vecID.push_back(StartLine.ID);
	for (auto& tLine : MapObjectManager->Get_TerrainVector(TERRAINTYPE::LAND))
	{
		if (Check_ID(tLine.ID))
			continue;
		if ((tLine.Start.x == StartLine.End.x) && (tLine.Start.y == StartLine.End.y)) // 왼쪽점과 오른쪽 점이 겹치는 두 선
		{
			if (tLine.ID == TargetLine.ID)
			{
				m_lstRoot.emplace_front(tLine);
				return true;
			}
			else
			{
				if (RotateRootL(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
				if (RotateRootR(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
			}
		}
	}
	for (auto& tLine : MapObjectManager->Get_TerrainVector(TERRAINTYPE::PASSABLE))
	{
		if (Check_ID(tLine.ID))
			continue;
		if ((tLine.Start.x == StartLine.End.x) && (tLine.Start.y == StartLine.End.y)) // 왼쪽점과 오른쪽 점이 겹치는 두 선
		{
			if (tLine.ID == TargetLine.ID)
			{
				m_lstRoot.emplace_front(tLine);
				return true;
			}
			else
			{
				if (RotateRootL(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
				if (RotateRootR(tLine, TargetLine))
				{
					m_lstRoot.emplace_front(tLine);
					return true;
				}
			}
		}
	}
	return false;
}

bool CGameObject::Check_ID(int ID)
{
	for (auto iID : m_vecID)
	{
		if (ID == iID)
			return true;
	}
	return false;
}





