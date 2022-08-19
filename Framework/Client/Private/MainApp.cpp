#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Loading_BackGround.h"
#include "UI_Loading_Twinkle.h"
#include "UI_Loading.h"
#include "UI_Money.h"
#include "UI_Touch.h"
#include "Psy.h"
#include "Player_Chicken.h"
#include "Item.h"

#ifdef USE_IMGUI
#include "ImgUi_Manager.h"
#endif // USE_IMGUI

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
	{
		MSGBOX("Failed to Initialize_Engine");
		return E_FAIL;
	}
	
	if (FAILED(Ready_Prototype_ShowObjects()))
	{
		MSGBOX("Failed to Ready_Prototype_ShowObjects");
		return E_FAIL;
	}


	if (FAILED(Ready_Prototype_Components()))
	{
		MSGBOX("Failed to Ready_Prototype_Components");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObjects()))
	{
		MSGBOX("Failed to Ready_Prototype_GameObjects");
		return E_FAIL;
	}

	if (FAILED(Open_Level(LEVEL_LOGO)))
	{
		MSGBOX("Failed to Open Level Logo");
		return E_FAIL;
	}

#ifdef USE_IMGUI
	CImgUi_Manager::GetInstance()->ImgUi_SetUp(m_pDevice, m_pDeviceContext);
#endif // USE_IMGUI

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;

#ifdef _DEBUG
	m_dTimerAcc += TimeDelta;
#endif // _DEBUG

	if (0 > m_pGameInstance->Tick_Engine(TimeDelta))
		return -1;

#ifdef USE_IMGUI
	CImgUi_Manager::GetInstance()->ImgUi_Tick();
#endif // USE_IMGUI

	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 1.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	if (FAILED(m_pRenderer->Render()))
		return E_FAIL;

#ifdef USE_IMGUI
	CImgUi_Manager::GetInstance()->ImgUi_Render();
#endif // USE_IMGUI

	//개선 해자
	//어느 플레이어든 간에 적용하게 만들어보자
	CPlayer_Chicken*		pPsy = nullptr;
	m_pLayer = m_pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	if (m_pLayer != nullptr)
	{
		CGameObject* cGameObject = m_pLayer->Get_Objects().front();
		if (cGameObject != nullptr) {
			pPsy = (CPlayer_Chicken*)cGameObject;
			wsprintf(m_szCross, TEXT("Cross %d"), pPsy->Get_WalkCount());
			m_pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_szCross, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		}
	}
	//m_pLayer = m_pGameInstance->Get_ObjectLayer(LEVEL_KOREA, TEXT("Layer_Player"));
	//if (m_pLayer != nullptr)
	//{
	//	CGameObject* cGameObject = m_pLayer->Get_Objects().front();
	//	if (cGameObject != nullptr) {
	//		pPsy = (CPsy*)cGameObject;
	//		wsprintf(m_szCross, TEXT("Cross %d"), pPsy->Get_WalkCount());
	//		m_pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_szCross, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	//	}
	//}

	//맵컴테이너와ㄱ,와 고나련돼 ㄴpair문제
	m_pLayer = m_pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	if (m_pLayer != nullptr)
	{
		CGameObject* cGameObject = m_pLayer->Get_Objects().front();
		if (cGameObject != nullptr) {
			pPsy = (CPlayer_Chicken*)cGameObject;
			wsprintf(m_szCoin, TEXT("%d"), pPsy->Get_CoinValue());
			m_pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_szCoin, _float2(1150.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
	}
	//최종 개선
	//m_pGameInstance->Render_Score();

	//RELEASE_INSTANCE(CGameObject);

	m_pGameInstance->Render_Level();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/*HRESULT		hr = 0;*/

	CLevel*		pLevel = nullptr;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
		break;
	case LEVEL_GAMEPLAY:
	case LEVEL_KOREA:
	case LEVEL_JAPAN:
		pLevel = CLevel_Loading::Create(m_pDevice, m_pDeviceContext, eLevelIndex);
		eLevelIndex = LEVEL_LOADING;
		break;
	}

	if (nullptr == pLevel)
		return E_FAIL;

	if (FAILED(m_pGameInstance->OpenLevel(eLevelIndex, pLevel)))
		return E_FAIL;

	return S_OK;

}

HRESULT CMainApp::Ready_Prototype_Components()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Renderer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXTEX */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"),
		VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/CrossRoad/Texture/Title/Front_Image.jpg"), 1))))
		return E_FAIL;

	//=======Loading_Level=============//

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BackGround"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/CrossRoad/Texture/Loading_Page/Black_Background.jpg"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Twinkle"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/CrossRoad/Texture/Loading/Twinkle_%d.png"), 16))))
		return E_FAIL;

	//========UI====================//

	/* For.Prototype_Component_Texture_UI_Money */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Money"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/CrossRoad/Texture/Coin/Coin.png"), 1))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Money */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Money"),
		CUI_Money::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Loading_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_BackGround"),
		CLoading_BackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Texture_UI_Loading_Twinkle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_UI_Loading_Twinkle"),
		CUI_Loading_Twinkle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ShowObjects()
{
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("8BIT_WONDER"), TEXT("../Bin/Resources/CrossRoad/Font/8BIT_WONDER.spritefont"))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
#ifdef USE_IMGUI
	CImgUi_Manager::GetInstance()->DestroyInstance();
#endif // USE_IMGUI

	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}
