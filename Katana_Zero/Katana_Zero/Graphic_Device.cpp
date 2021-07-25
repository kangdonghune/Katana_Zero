#include "stdafx.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}

HRESULT CGraphic_Device::Ready_Graphic_Device()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DCAPS9 d3dCaps{};

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	/*
	IDirect3D9 인터페이스로의 포인터를 얻는 것으로
	Direct3DCreate9 단일 인자에는 항상 D3D_SDK_VERSION을 전달해야 한다.

	D3D_SDK_VERSION는 어플리케이션이 올바른 해더 파일로 만들어 졌음을 확인하는 것으로 함수가 실패하면 null포인터를 리턴한다.

	m_pSDK 객체는 장치 검증(기능, 디스플레이 모드, 포멘 등과 같이 시스템의 그래픽 장치가 제공하는 특성에 대한 정보를 얻는 과정)과
	물리적 장치를 나타내는 IDirect3DDevice9 객체 생성의 두가지 용도로 이용된다.
	*/



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;
	//p89.하드웨어 버텍스 프로세싱 확인하기
	// * 버텍스 프로세싱이란? 버텍스는 3D 기하 물체를 구성하는 기본단위로 소프트웨어나 하드웨어의 두 가지 방법으로 처리할 수 있다. 소프트웨어 버텍스 프로세싱은
	//언제나 지원되며 항상 이용 가능하지만 하드웨어 버텍스 프로세싱은 그래픽 카드가 버텍스 프로세싱을 지원하는 경우에만 이를 이용할 수 있다.
	// GetDeviceCaps 는 카드에서 버텍스 프로세싱 모드를 지원하는지의 여부를 확인하는 작업이다.
	// D3DADAPTER_DEFAULT : 특성을 얻고자하는 물리 디스플레이 어뎁터
	// D3DDEVTYPE_HAL : 이용할 장치의 탑입을 지정한 것(소프트웨어 장치라는 의미)
	// Hal 은 하드웨어 추상 층으로 불리는 중간 단계로 장치로 하여금 특별한 작업을 수행하도록 하는 장치 고유의 코드이다.
	// &d3dCaps : 초기화된 특성 구조체를 리턴한다.

	DWORD vp = 0;
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	// 정점 변환과 조명연산을 하드웨어에서 처리 하겠다. - 버텍스 프로세싱을 하드웨어에서 하겠다.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// 정점 변환과 조명연산을 소프트웨어에서 처리 하겠다.  - 버텍스 프로세싱을 소프트웨어에서 하겠다.

	//여기서 저장한 vp는 IDirect3DDevice 객체를 만들 때 버텍스 프로세싱의 타입을 지정하기 위하여 사용하였다.


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//p91~92 D3DPRESENT_PARAMETERS 구조체
	D3DPRESENT_PARAMETERS d3dpp; //컴퓨터 정보를 담기 위한 객체.
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINCX; // 픽셀 단위 후면 버퍼 너비
	d3dpp.BackBufferHeight = WINCY;// 픽셀 단위 후면 버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 후면 버퍼의 픽셀 포맷
	d3dpp.BackBufferCount = 1; // 이용할 후면 버퍼의 수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 후면 버퍼에 이용할 멀티 샘플링의 타입.
	d3dpp.MultiSampleQuality = 0; // 멀티 샘플링의 레벨

								  //D3DSWAPEFFECT_DISCARD - 스왑체인 방식을 사용하겠다 라는 뜻. - 학원 필기
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 플리핑 체인의 버퍼가 교환되는 방법을 지정하는 열거형 맴버
	d3dpp.hDeviceWindow = g_hWND; // 서비스와 연결된 윈도우 핸들, 드로잉의 대상이 될 애플리케이션 윈도우를 지정
	d3dpp.Windowed = TRUE;// TRUE 일 경우 창모드, FALSE일 경우 전체화면 모드 
	d3dpp.EnableAutoDepthStencil = TRUE; // Direct3D가 자동으로 깊이/스텐실 버퍼를 만들고 관리하길 바란다.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //깊이/스텐실 버퍼의 포맷

												 /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // 재생율을 지정한다.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // 화면전환을 즉시 해준다.

	//p93.~p94 인터페이스 만들기
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
		goto ERR;
	//CretateDevice
	//D3DADAPTER_DEFAULT : 만들어질 객체와 대응될 물리 어뎁터를 지정한다. 
	//D3DDEVTYPE_HAL : 이용할 장치 타입을 지정한다. - 하드웨어 장치라는 뜻
	//g_hWND 장치와 연결된 윈도으 핸들 
	//vp : 버텍스 프로세싱 어디서 할건지 위에 있음
	//&d3dpp : 장치 특성의 일부를 정의하는 초기화된 D3DPRESENT_PARAMETERS인스턴스를 지정한다.
	//&m_pDevice 생성된 장치를 리턴한다.

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite))) //스프라이트 만들어 줘라
		goto ERR;

	//p220. 글꼴 ID3DXFont
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 20; // 높이
	tFontInfo.Width = 10; // 너비
	tFontInfo.Weight = FW_HEAVY;//두께 
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"돋움체");
	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &tFontInfo, &m_pFont)))
		goto ERR;


	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		goto ERR;

	return S_OK;
ERR:
	ERR_MSG(L"Graphic_Device Ready Failed");

	return E_FAIL;
}

void CGraphic_Device::Release_Graphic_Device()
{
	//(레퍼런스 카운트 라고 부모로 참조될 때마다 레퍼런틋 카운트가 증가하는데 레퍼런스 카운트가 0이 아니면 딜리트가 안되게 해둠
	//이 경우 sdk 산하에 디바이스 > 스프라이트 & 폰트가 만들어 진 것이기에 만든 순서의 반대로 릴리즈 해줘야 딜리트가 된다.
	if (m_pFont)
		m_pFont->Release();

	if (m_pSprite)
		m_pSprite->Release();

	if (m_pDevice)
		m_pDevice->Release();

	if (m_pSDK)
		m_pSDK->Release();
}

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 0.f, 1);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
