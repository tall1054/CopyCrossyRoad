#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Level_Korea.h"
#include "Level_Test.h"
#include "Level_Korea.h"
#include "Level_Select.h"
#include "Camera_Dynamic.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	/* 현재 레벨이 생성될 때, 레벨에서 사용하고자하는 사본객체를 생성한다. */
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		MSGBOX("Failed to Layer_Camera");
		return E_FAIL;
	}
	
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		MSGBOX("Failed to Layer_BackGround");
		return E_FAIL;
	}

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	
	pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);
	pGameInstance->PlayBGM(L"Lobby_Normal.wav");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return 0;
}

_int CLevel_Logo::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	//if (pInstance->Get_DIKeyState(DIK_T) & 0x80)
	//{
	//	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	//	Safe_AddRef(pGameInstance);

	//	if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_TEST))))
	//		return E_FAIL;

	//	Safe_Release(pGameInstance);
	//}

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
	{
		MSGBOX("Failed to Logo Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("로고레벨"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원본객체 생성 */

	/* For.Prototype_GameObject_BackGround */
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
	return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
	{
		MSGBOX("Failed to Logo Add_GameObject");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

}
