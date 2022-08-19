#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Loading_BackGround.h"
#include "UI_Loading_Twinkle.h"
#include "UI_Loading.h"
#include "Level_Gameplay.h"
#include "Level_Test.h"
#include "Level_Korea.h"
#include "Level_Japan.h"


CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
	{
		MSGBOX("Failed to Create NativeConstruct");
		return E_FAIL;
	}

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eNextLevel);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	pInstance->StopSound(CSoundMgr::CHANNELID::BGM);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_Loading::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return 0;
}

_int CLevel_Loading::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (true == m_pLoader->isFinished())
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLevel*		pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_GAMEPLAY:
			pLevel = CLevel_GamePlay::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_KOREA:
			pLevel = CLevel_Korea::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_JAPAN:
			pLevel = CLevel_Japan::Create(m_pDevice, m_pDeviceContext);
			break;

		}

		if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, pLevel)))
		{
			MSGBOX("Failed to Loading OpenLevel");
			return E_FAIL;
		}

		Safe_Release(pGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
	{
		MSGBOX("Failed to Loading Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("로딩레벨"));

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Loading_BackGround"))))
	{
		MSGBOX("Failed to Loading Prototype_GameObject_Loading_BackGround");
		return E_FAIL;
	}
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Texture_UI_Loading_Twinkle"))))
	{
		MSGBOX("Failed to Loading Prototype_GameObject_Texture_UI_Loading_Twinkle");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
