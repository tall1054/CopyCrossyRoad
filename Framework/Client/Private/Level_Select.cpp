#include "stdafx.h"
#include "..\Public\Level_Select.h"
#include <random>
#include "Level_Loading.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Grass.h"
#include "Road.h"
#include "Train.h"
#include "Water.h"
//=======object=====//
#include "Tree.h"
#include "TreeA.h"
#include "TreeB.h"
#include "TreeC.h"
#include "FineTreeA.h"
#include "FineTreeB.h"
#include "FineTreeC.h"

CLevel_Select::CLevel_Select(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Select::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
	{
		MSGBOX("Failed to Ready_Lights");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	{
		MSGBOX("Failed to Game Play Layer_Player");
		return E_FAIL;
	}


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		MSGBOX("Failed to Game Play Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	{
		MSGBOX("Failed to Game Play Layer_Monster");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		MSGBOX("Failed to Game Play Layer_BackGround");
		return E_FAIL;
	}

	//if (FAILED(Ready_Layer_Objects(TEXT("Layer_Objects"))))
	//{
	//	MSGBOX("Failed to GamePlay Layer_Objects");
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	{
		MSGBOX("Failed to Game Play Layer_Effect");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Failed to Game Play Layer_UI");
		return E_FAIL;
	}

	return S_OK;
}

_int CLevel_Select::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CLevel_Select::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;


	return 0;
}

HRESULT CLevel_Select::Render()
{
	if (FAILED(__super::Render()))
	{
		MSGBOX("Failed to Game Play Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("게임플레이"));

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_iWalkCount<>, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Lights()
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
		MSGBOX("Failed to Game Play Add_Light");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	//카메라의 위치
	CameraDesc.vEye = _float3(2.f, 10.f, -7.f);
	//카메라가 바라보는 방향
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	//카메라의 축
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	//카메라의 시야 범위(각도)
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	//시야 왜곡 방지
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 100.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_Money"))))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_UI_Money");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_Select * CLevel_Select::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Select*	pInstance = new CLevel_Select(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Select");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Select::Free()
{
	__super::Free();
}
