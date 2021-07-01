#include "stdafx.h"
#include "GameObject.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "FrameManager.h"
#include "Graphic_Device.h"

CGameObject::CGameObject()
	:m_pUnitInfo(nullptr)
	, m_tFrame({})
	, m_fSpeed(0)
	, m_State(PLAYERSTATE::IDLE)
	, m_wstrOldState(L"")
	, m_wstrCurState(L"")
	, m_fRatio(0.f)
	, m_iUnitDir(1)
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
	m_tHitBox.left = m_pUnitInfo->D3VecPos.x - fCenterX*m_fRatio;
	m_tHitBox.top = m_pUnitInfo->D3VecPos.y - fCenterY*m_fRatio;
	m_tHitBox.right = m_pUnitInfo->D3VecPos.x + fCenterX*m_fRatio;
	m_tHitBox.bottom = m_pUnitInfo->D3VecPos.y + fCenterY*m_fRatio;

}

void CGameObject::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += FrameManager->Get_SPF() * fSpeed; //frame start end�� �� Ŭ�������� �����ش�. 
	if ((size_t)m_tFrame.fFrameEnd <= (size_t)m_tFrame.fFrameStart)
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
