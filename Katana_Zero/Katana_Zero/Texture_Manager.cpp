#include "stdafx.h"
#include "Texture_Manager.h"
#include "Multi_Texture.h"

IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{

	Release_Texture_Manager();
}
HRESULT CTexture_Manager::Init_Texture_Manager()
{
	//맵
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Map/stage%d.bmp", L"Map", L"Stage", 6)))
		return E_FAIL;

	//플레이어

	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_attack/%d.png", L"Player", L"Attack", 7)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_crouch/%d.png", L"Player", L"Crouch",1)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_doorbreak_full/%d.png", L"Player", L"DoorBreakFull", 10)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_fall/%d.png", L"Player", L"Fall", 4)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_hurtfly_begin/%d.png", L"Player", L"Hurtfly_begin", 2)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_hurtfly_loop/%d.png", L"Player", L"Hurtfly_loop", 4)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_hurtground/%d.png", L"Player", L"Hurtground", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_hurtrecover/%d.png", L"Player", L"Hurtrecover", 9)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_idle/%d.png", L"Player", L"Idle", 11)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_idle_to_run/%d.png", L"Player", L"Idle_to_run", 4)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_idle_to_walk/%d.png", L"Player", L"Idle_to_walk", 4)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_jump/%d.png", L"Player", L"Jump", 4)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_player_dance/%d.png", L"Player", L"Dance", 12)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_player_dooropen_gentle/%d.png", L"Player", L"DoorOpen", 9)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_player_flip/%d.png", L"Player", L"Flip", 11)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_postcrouch/%d.png", L"Player", L"Postcrouch", 3)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_precrouch/%d.png", L"Player", L"Precrouch", 3)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_roll/%d.png", L"Player", L"Roll", 7)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_run/%d.png", L"Player", L"Run", 10)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_run_to_idle/%d.png", L"Player", L"Run_to_idle", 5)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_walk/%d.png", L"Player", L"Walk", 10)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Player/spr_wallslide/%d.png", L"Player", L"Wallslide", 1)))
		return E_FAIL;

	//갱스터
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_arm/%d.png", L"Gangster", L"arm", 2)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsteraim/%d.png", L"Gangster", L"Aim", 7)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterenterstair/%d.png", L"Gangster", L"Enterstair", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterfall/%d.png", L"Gangster", L"Fall", 12)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangstergun/%d.png", L"Gangster", L"Gangstergun", 2)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterhurtfly/%d.png", L"Gangster", L"Hurtfly", 2)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterhurtground/%d.png", L"Gangster", L"Hurtground", 14)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsteridle/%d.png", L"Gangster", L"Idle", 8)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterleavestair/%d.png", L"Gangster", L"Leavestair", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterrun/%d.png", L"Gangster", L"Run", 10)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterturn/%d.png", L"Gangster", L"Turn", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterwalk/%d.png", L"Gangster", L"Walk", 8)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_gangsterwhip/%d.png", L"Gangster", L"Whip", 6)))
		return E_FAIL;

	//이펙트
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Effect/Hitimpact/%d.png", L"Effect", L"Hitimpact", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Effect/Attack/%d.png", L"Effect", L"Attack", 6)))
		return E_FAIL;
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Enemy/Gangster/spr_fire_1/%d.png", L"Effect", L"Gangsterfire", 6)))
		return E_FAIL;

	//투사체
	if (FAILED(Texture_Maneger->Insert_Texture_Manager(L"../Resource/Texture/Projectile/Bullet/%d.png", L"Projectile", L"Bullet", 1)))
		return E_FAIL;

	return S_OK;
}
//L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png",L"Player", L"Attack", 6
//		L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png",L"Player", L"Dash", 11
//// L"../Texture/Cube.png", L"Cube"
HRESULT CTexture_Manager::Insert_Texture_Manager(const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwCount)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey); //해당 이름과 같은 오브젝트가 있는지 체크
	CTexture* pTexture = nullptr;
	if (m_mapTexture.end() == iter_find) //해당 오브젝트와 같은 이름의 키가 없다면
	{
		pTexture = new CMulti_Texture;
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))//해당 오브젝트의 상태 키와 이미지 개수를 넘겨줘서 생성
			goto ERR;

		m_mapTexture.emplace(wstrObjectKey, pTexture); //오브젝트 키와 만들어진 멀티 텍스쳐를 맵에 추가
	}
	else {
		//m_mapTexture[wstrObjectKey] - 여기까지가 CMulti_Texture 
		//해당 오브젝트가 있다면
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, dwCount))) //해당 오브젝트 키에 상태 키와 이미지 추가
			goto ERR;
	}

	return S_OK;
ERR:
	ERR_MSG(L"Texture Insert Failed");
	return E_FAIL;
}

const TEXINFO * CTexture_Manager::Get_TexInfo_Manager(const wstring & wstrOjbectKey, const wstring & wstrStateKey, const DWORD & dwIndex)
{
	auto& iter_find = m_mapTexture.find(wstrOjbectKey);// 해당 이름과 같은 오브젝트  키 있는 지 체크
	if (m_mapTexture.end() == iter_find) //없다면
		return nullptr;
	return iter_find->second->Get_TexInfo(wstrStateKey, dwIndex); //있으면 해당 오브젝트의 텍스쳐에서 특정 상태의 특정 인덱스 이미지 정보 가져오기.
	//return m_mapTexture[wstrOjbectKey]->Get_TexInfo(wstrStateKey, dwIndex); 
}

const size_t CTexture_Manager::Get_TexInfo_Frame(const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey);// 해당 이름과 같은 오브젝트  키 있는 지 체크
	if (m_mapTexture.end() == iter_find) //없다면
		return 0;
	return dynamic_cast<CMulti_Texture*>(iter_find->second)->Get_TexInfo_Frame(wstrStateKey);
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}
