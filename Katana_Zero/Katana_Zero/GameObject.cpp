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
		ERR_MSG(L"���� ������ �����ϴ�.");
		return;
	}
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player���� �ؽ��� ã�� ����");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1; 
	float fCenterY = pTexInfo->tImageInfo.Height >> 1; 	
	//ratio �⺻ ������ ������ ��� 2��
	m_tHitBox.left = m_vecPivot.x - fCenterX*m_fRatio;
	m_tHitBox.top =    m_vecPivot.y - 2*fCenterY*m_fRatio;
	m_tHitBox.right =  m_vecPivot.x + fCenterX*m_fRatio;
	m_tHitBox.bottom = m_vecPivot.y;

}

void CGameObject::Update_HitBoxOBB()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"���� ������ �����ϴ�.");
		return;
	}
	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	D3DXVECTOR3				m_tHitBoxObbTemp[4];
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Player���� �ؽ��� ã�� ����");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	//ratio �⺻ ������ ������ ��� 2��
	m_tHitBoxObbTemp[0] = {-m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//���� ��
	m_tHitBoxObbTemp[1] = { m_fRatio*fCenterX, -m_fRatio*fCenterY, 0.f };//������ ��
	m_tHitBoxObbTemp[2] = { m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//������ �Ʒ�
	m_tHitBoxObbTemp[3] = {-m_fRatio*fCenterX,  m_fRatio*fCenterY, 0.f };//���� �Ʒ�
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
	m_tFrame.fFrameStart += FrameManager->Get_SPF() *(FrameManager->Get_FPS()/60)* fSpeed; //frame start end�� �� Ŭ�������� �����ش�. 
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
	//���°� �ٲ� ���� ������ ����.
	m_wstrCurState = m_pUnitInfo->wstrState;
	if (m_wstrOldState != m_wstrCurState)
	{
		m_tFrame.fFrameStart = 0.f;
		m_tFrame.fFrameEnd = Texture_Maneger->Get_TexInfo_Frame(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState);
		m_wstrOldState = m_wstrCurState;
	}

}

bool CGameObject::Check_FrameEnd() //���� ���·� �����ϱ� �� �غ� ������ ���ƴ��� üũ
{
	static bool FrameCheck = false;
	
	if (FrameCheck) // �غ� ������ ��������
	{
		FrameCheck = false; // �غ��� �������� ���������� �ٽ� false ���·�.
		return true;
	}
	if ((size_t)m_tFrame.fFrameEnd-1 == (size_t)m_tFrame.fFrameStart) // ���� �����Ӱ� �� �������� ���ٸ� �غ� ������ ���� ��
	{
		FrameCheck = true; 
		return false; // ���� �����ӱ��� �׸��� ���� �ٲ�� �ϱ⿡ false�� ��ȯ
	}
	return false;
}




