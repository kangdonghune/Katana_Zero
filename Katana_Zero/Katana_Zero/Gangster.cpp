#include "stdafx.h"
#include "Gangster.h"
#include "GameObject.h";
#include "Enemy.h"
#include "GameObjectManager.h"
#include "Texture_Manager.h"
#include "TimeManager.h"
#include "Bullet.h"


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

void CGangster::Update_TargetRotate()
{ 
	D3DXVECTOR3 TargetPos = { m_pTarget->Get_Pivot().x,m_pTarget->Get_Pivot().y - m_pTarget->Get_Ratio()*(Texture_Maneger->Get_TexInfo_Manager(m_pTarget->Get_UnitInfo()->wstrKey,m_pTarget->Get_UnitInfo()->wstrState,0)->tImageInfo.Height>>1) ,0 };
	D3DXVECTOR3 MyPos = { m_pUnitInfo->D3VecPos.x,  m_pUnitInfo->D3VecPos.y - m_fRatio*(Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState,0)->tImageInfo.Height >> 1),0 };
	D3DXVECTOR3 vDir = TargetPos - MyPos;
	D3DXVECTOR3 vLook{ 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	m_iUnitDir = 1;
	if (TargetPos.x <= MyPos.x)//0~90 -1 angle 270 360 1 90~180 1 180~270 -1
	{
		m_iUnitDir = -1;
		vLook.x *= -1;
	}
	float fCos = D3DXVec3Dot(&vDir, &vLook);
	m_fRotateAngle = acosf(fCos);

	
	if (0 <= (TargetPos.x - MyPos.x)* (MyPos.y - TargetPos.y))
		m_fRotateAngle *= -1;

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
	m_fTargetAngle = 0.f;
	return S_OK;
}

void CGangster::Update_GameObject()
{
	m_fAttackLimit += TimeManager->Get_DeltaTime();
	Update_Frame(); // ���� ���°� ���� ���¿� �ٸ� ���·� ���ߴٸ� ������ ����
	FrameMove(m_fSpeed); //���� ������ ���� �Ǵ� �ʱ�ȭ
	Update_TargetRotate();
	Update_HitBox(); // ���� ���� �������� �浹 ���� ������Ʈ
	if (m_fAttackLimit >= m_fAttackCool)
	{
		m_fAttackLimit = 0.f;
		Find_TargetAngle();
		GameObjectManager->Insert_GameObjectManager(CBullet::Create(this), GAMEOBJECT::BULLET);
	}
}

void CGangster::LateUpdate_GameObject()
{
}

void CGangster::Render_GameObject()
{
	if (nullptr == m_pUnitInfo)
	{
		ERR_MSG(L"������ ������ �����ϴ�.");
		return;
	}

	D3DXMATRIX matScale, matRolateZ, matTrans, matWorld;

	const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(m_pUnitInfo->wstrKey, m_pUnitInfo->wstrState, (size_t)m_tFrame.fFrameStart);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Gangster���� �ؽ��� ã�� ����");
		return;
	}

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, m_iUnitDir * m_fRatio, m_fRatio, 0.f);
	//D3DXMatrixRotationZ(&matRolateZ, m_fRotateAngle);
	D3DXMatrixTranslation(&matTrans, m_vecPivot.x, m_vecPivot.y - m_fRatio*fCenterY, 0.f);
	matWorld = matScale  *matTrans;
	//matWorld = matScale * matRolateZ *matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (GetAsyncKeyState(VK_CONTROL) & 0X8000)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 255));
	}
	Render_HitBox();
}

void CGangster::Release_GameObject()
{
	Safe_Delete(m_pUnitInfo);
}