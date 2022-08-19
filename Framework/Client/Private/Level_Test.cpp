#include "stdafx.h"
#include "..\Public\Level_Test.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Dynamic.h"
#include "Camera_Player.h"

CLevel_Test::CLevel_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Test::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_NextLevel = eNextLevel;

	if(FAILED(Ready_Light()))
	{
		MSGBOX("Failed to Ready_Lights");
		S_OK;
	}

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), m_NextLevel)))
	{
		MSGBOX("Failed to GameTest Layer_Player");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"), m_NextLevel)))
	{
		MSGBOX("Failed to GameTest Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"), m_NextLevel)))
	{
		MSGBOX("Failed to GameTest Layer_Monster");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"), m_NextLevel)))
	{
		MSGBOX("Failed to GameTest Layer_BackGround");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"), m_NextLevel)))
	{
		MSGBOX("Failed to GameTest Layer_UI");
		return E_FAIL;
	}



	return S_OK;
}

_int CLevel_Test::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return 0;
}

_int CLevel_Test::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_Test::Render()
{
	if (FAILED(__super::Render()))
	{
		MSGBOX("Failed to GameTest Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("게임테스트"));

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Camera(const _tchar * pLayerTag, LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));
	CameraDesc.vEye = _float3(2.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 250.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_BackGround(const _tchar * pLayerTag, LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Terrain");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Tree"))))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Player(const _tchar * pLayerTag, LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Psy"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Psy");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Monster(const _tchar * pLayerTag, LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Monster_Car"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Car");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_KoreaCar001"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar001");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_KoreaCar002"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar002");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_KoreaCar003"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar003");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_UI(const _tchar * pLayerTag, LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Light()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
	{
		MSGBOX("Failed to GameTest Add_Light");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Test * CLevel_Test::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, LEVEL eNextLevel)
{
	CLevel_Test*	pInstance = new CLevel_Test(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Test::Free()
{
	__super::Free();
}
