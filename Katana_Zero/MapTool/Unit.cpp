#include "stdafx.h"
#include "Unit.h"
#include "Texture_Manager.h"
#include "MainFrm.h"
#include "MapToolView.h"

IMPLEMENT_SINGLETON(CUnit)
CUnit::CUnit()
	:m_pView(nullptr)
{
}


CUnit::~CUnit()
{
	Release_Unit();
}

void CUnit::Render_Unit()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pView = dynamic_cast<CMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	for (auto& pUnitvec : m_vecUnit)
	{
		for (auto& pUnit : pUnitvec)
		{
			const TEXINFO* pTexInfo = Texture_Maneger->Get_TexInfo_Manager(pUnit->wstrKey, pUnit->wstrState, 0);
			if (nullptr == pTexInfo)
			{
				ERR_MSG(L"Unit::Render_Unit에서 텍스쳐 찾기 실패");
				return;
			}
			
			float fCenterX = pTexInfo->tImageInfo.Width >> 1;
			float fCenterY = pTexInfo->tImageInfo.Height >> 1;
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, pUnit->D3VecPos.x - m_pView->GetScrollPos(SB_HORZ), pUnit->D3VecPos.y - m_pView->GetScrollPos(SB_VERT), 0.f);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	
	}

	
}

void CUnit::Release_Unit()
{
	for (auto& pUnitvec : m_vecUnit)
	{
		for (auto& pUnit : pUnitvec)
		{
			Safe_Delete(pUnit);
		}
		pUnitvec.clear();
		pUnitvec.shrink_to_fit();
	}
	m_vecUnit->clear();
	m_vecUnit->shrink_to_fit();
}
