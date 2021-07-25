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
	IDirect3D9 �������̽����� �����͸� ��� ������
	Direct3DCreate9 ���� ���ڿ��� �׻� D3D_SDK_VERSION�� �����ؾ� �Ѵ�.

	D3D_SDK_VERSION�� ���ø����̼��� �ùٸ� �ش� ���Ϸ� ����� ������ Ȯ���ϴ� ������ �Լ��� �����ϸ� null�����͸� �����Ѵ�.

	m_pSDK ��ü�� ��ġ ����(���, ���÷��� ���, ���� ��� ���� �ý����� �׷��� ��ġ�� �����ϴ� Ư���� ���� ������ ��� ����)��
	������ ��ġ�� ��Ÿ���� IDirect3DDevice9 ��ü ������ �ΰ��� �뵵�� �̿�ȴ�.
	*/



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;
	//p89.�ϵ���� ���ؽ� ���μ��� Ȯ���ϱ�
	// * ���ؽ� ���μ����̶�? ���ؽ��� 3D ���� ��ü�� �����ϴ� �⺻������ ����Ʈ��� �ϵ������ �� ���� ������� ó���� �� �ִ�. ����Ʈ���� ���ؽ� ���μ�����
	//������ �����Ǹ� �׻� �̿� ���������� �ϵ���� ���ؽ� ���μ����� �׷��� ī�尡 ���ؽ� ���μ����� �����ϴ� ��쿡�� �̸� �̿��� �� �ִ�.
	// GetDeviceCaps �� ī�忡�� ���ؽ� ���μ��� ��带 �����ϴ����� ���θ� Ȯ���ϴ� �۾��̴�.
	// D3DADAPTER_DEFAULT : Ư���� ������ϴ� ���� ���÷��� ���
	// D3DDEVTYPE_HAL : �̿��� ��ġ�� ž���� ������ ��(����Ʈ���� ��ġ��� �ǹ�)
	// Hal �� �ϵ���� �߻� ������ �Ҹ��� �߰� �ܰ�� ��ġ�� �Ͽ��� Ư���� �۾��� �����ϵ��� �ϴ� ��ġ ������ �ڵ��̴�.
	// &d3dCaps : �ʱ�ȭ�� Ư�� ����ü�� �����Ѵ�.

	DWORD vp = 0;
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	// ���� ��ȯ�� �������� �ϵ����� ó�� �ϰڴ�. - ���ؽ� ���μ����� �ϵ����� �ϰڴ�.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// ���� ��ȯ�� �������� ����Ʈ����� ó�� �ϰڴ�.  - ���ؽ� ���μ����� ����Ʈ����� �ϰڴ�.

	//���⼭ ������ vp�� IDirect3DDevice ��ü�� ���� �� ���ؽ� ���μ����� Ÿ���� �����ϱ� ���Ͽ� ����Ͽ���.


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//p91~92 D3DPRESENT_PARAMETERS ����ü
	D3DPRESENT_PARAMETERS d3dpp; //��ǻ�� ������ ��� ���� ��ü.
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINCX; // �ȼ� ���� �ĸ� ���� �ʺ�
	d3dpp.BackBufferHeight = WINCY;// �ȼ� ���� �ĸ� ���� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ� ������ �ȼ� ����
	d3dpp.BackBufferCount = 1; // �̿��� �ĸ� ������ ��

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // �ĸ� ���ۿ� �̿��� ��Ƽ ���ø��� Ÿ��.
	d3dpp.MultiSampleQuality = 0; // ��Ƽ ���ø��� ����

								  //D3DSWAPEFFECT_DISCARD - ����ü�� ����� ����ϰڴ� ��� ��. - �п� �ʱ�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // �ø��� ü���� ���۰� ��ȯ�Ǵ� ����� �����ϴ� ������ �ɹ�
	d3dpp.hDeviceWindow = g_hWND; // ���񽺿� ����� ������ �ڵ�, ������� ����� �� ���ø����̼� �����츦 ����
	d3dpp.Windowed = TRUE;// TRUE �� ��� â���, FALSE�� ��� ��üȭ�� ��� 
	d3dpp.EnableAutoDepthStencil = TRUE; // Direct3D�� �ڵ����� ����/���ٽ� ���۸� ����� �����ϱ� �ٶ���.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //����/���ٽ� ������ ����

												 /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ������� �����Ѵ�.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // ȭ����ȯ�� ��� ���ش�.

	//p93.~p94 �������̽� �����
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
		goto ERR;
	//CretateDevice
	//D3DADAPTER_DEFAULT : ������� ��ü�� ������ ���� ��͸� �����Ѵ�. 
	//D3DDEVTYPE_HAL : �̿��� ��ġ Ÿ���� �����Ѵ�. - �ϵ���� ��ġ��� ��
	//g_hWND ��ġ�� ����� ������ �ڵ� 
	//vp : ���ؽ� ���μ��� ��� �Ұ��� ���� ����
	//&d3dpp : ��ġ Ư���� �Ϻθ� �����ϴ� �ʱ�ȭ�� D3DPRESENT_PARAMETERS�ν��Ͻ��� �����Ѵ�.
	//&m_pDevice ������ ��ġ�� �����Ѵ�.

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite))) //��������Ʈ ����� ���
		goto ERR;

	//p220. �۲� ID3DXFont
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 20; // ����
	tFontInfo.Width = 10; // �ʺ�
	tFontInfo.Weight = FW_HEAVY;//�β� 
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"����ü");
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
	//(���۷��� ī��Ʈ ��� �θ�� ������ ������ ���۷�ƶ ī��Ʈ�� �����ϴµ� ���۷��� ī��Ʈ�� 0�� �ƴϸ� ����Ʈ�� �ȵǰ� �ص�
	//�� ��� sdk ���Ͽ� ����̽� > ��������Ʈ & ��Ʈ�� ����� �� ���̱⿡ ���� ������ �ݴ�� ������ ����� ����Ʈ�� �ȴ�.
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
