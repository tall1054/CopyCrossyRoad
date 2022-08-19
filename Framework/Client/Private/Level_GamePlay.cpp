#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
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
//=========item======//
#include "Item.h"
//========monster=======//
#include "Monster_Train.h"
#include "Monster_TrainMiddle.h"
#include "Monster_TrainEnd.h"
#include "Monster_Truck.h"
#include "Monster_Truck2.h"
#include "Monster_OilTruck.h"
#include "Monster_Car.h"
#include "KoreaCar001.h"
#include "KoreaCar002.h"
#include "KoreaCar003.h"
#include "Muccle.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
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

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	{
		MSGBOX("Failed to Game Play Layer_Effect");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
	{
		MSGBOX("Failed to Game Play Layer_Item");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))
	{
		MSGBOX("Failed to GamePlay Layer_Structure");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Failed to GamePlay Layer_UI");
		return E_FAIL;
	}

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	pInstance->StopSound(CSoundMgr::CHANNELID::BGM);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CLevel_GamePlay::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CLevel_GamePlay::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;
	//레벨전환을 이루어내자

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_KOREA))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CLevel_GamePlay::Render()
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

HRESULT CLevel_GamePlay::Ready_Lights()
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

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Terrain");
		return E_FAIL;
	}

	_float	Grass0 = 2.f;
	for (_uint j = 0; j < 4; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, -9.f + Grass0, 1.f);
		Grass0 = Grass0 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float Grass1 = 2.f;
	for (_uint j = 0; j < 4; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, -1.f + Grass1, 1.f);
		Grass1 = Grass1 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float Grass2 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 11.f + Grass2, 1.f);
		Grass2 = Grass2 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Train1 = 2.f;
	for (_uint i = 0; i < 1; ++i)
	{
		CTrain::TrainPosition		TrainPos;
		ZeroMemory(&TrainPos, sizeof(CTrain::TrainPosition));
		TrainPos.vPosition = _float4(25.f, 0.f, 25.f + Train1, 1.f);
		Train1 = Train1 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Train"), &TrainPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Train");
			return E_FAIL;
		}
	}

	_float	Train2 = 2.f;
	for (_uint i = 0; i < 2; ++i)
	{
		CTrain::TrainPosition		TrainPos;
		ZeroMemory(&TrainPos, sizeof(CTrain::TrainPosition));
		TrainPos.vPosition = _float4(25.f, 0.f, 7.f + Train2, 1.f);
		Train2 = Train2 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Train"), &TrainPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Train");
			return E_FAIL;
		}
	}

	_float	Road1 = 2.f;
	for (_uint i = 0; i < 2; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 13.f + Road1, 1.f);
		Road1 = Road1 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}
	_float	Water1 = 2.f;
	for (_uint i = 0; i < 2; ++i)
	{
		CWater::WaterPosition	WaterPos;
		ZeroMemory(&WaterPos, sizeof(CWater::WaterPosition));
		WaterPos.vPosition = _float4(25.f, 0.f, 17.f + Water1, 1.f);
		Water1 = Water1 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Water"), &WaterPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Water");
			return E_FAIL;
		}
	}

	_float Grass3 = 2.f;
	for (_uint j = 0; j < 2; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 21.f + Grass3, 1.f);
		Grass3 = Grass3 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Road2 = 2.f;
	for (_uint i = 0; i < 4; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 27.f + Road2, 1.f);
		Road2 = Road2 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass4 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 35.f + Grass4, 1.f);
		Grass4 = Grass4 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Water2 = 2.f;
	for (_uint i = 0; i < 5; ++i)
	{
		CWater::WaterPosition	WaterPos;
		ZeroMemory(&WaterPos, sizeof(CWater::WaterPosition));
		WaterPos.vPosition = _float4(25.f, 0.f, 37.f + Water2, 1.f);
		Water2 = Water2 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Water"), &WaterPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Water");
			return E_FAIL;
		}
	}

	_float Grass5 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 45.f + Grass4, 1.f);
		Grass4 = Grass4 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Road3 = 2.f;
	for (_uint i = 0; i < 4; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 49.f + Road3, 1.f);
		Road3 = Road3 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass6 = 2.f;
	for (_uint j = 0; j < 3; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 59.f + Grass6, 1.f);
		Grass6 = Grass6 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Water3 = 2.f;
	for (_uint i = 0; i < 1; ++i)
	{
		CWater::WaterPosition	WaterPos;
		ZeroMemory(&WaterPos, sizeof(CWater::WaterPosition));
		WaterPos.vPosition = _float4(25.f, 0.f, 57.f + Water3, 1.f);
		Water3 = Water3 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Water"), &WaterPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Water");
			return E_FAIL;
		}
	}
	_float	Road4 = 2.f;
	for (_uint i = 0; i < 4; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 65.f + Road4, 1.f);
		Road4 = Road4 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass7 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 73.f + Grass7, 1.f);
		Grass7 = Grass7 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Train3 = 2.f;
	for (_uint i = 0; i < 4; ++i)
	{
		CTrain::TrainPosition		TrainPos;
		ZeroMemory(&TrainPos, sizeof(CTrain::TrainPosition));
		TrainPos.vPosition = _float4(25.f, 0.f, 75.f + Train3, 1.f);
		Train3 = Train3 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Train"), &TrainPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Train");
			return E_FAIL;
		}
	}

	_float Grass8 = 2.f;
	for (_uint j = 0; j < 2; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 83.f + Grass8, 1.f);
		Grass8 = Grass8 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Train4 = 2.f;
	for (_uint i = 0; i < 1; ++i)
	{
		CTrain::TrainPosition		TrainPos;
		ZeroMemory(&TrainPos, sizeof(CTrain::TrainPosition));
		TrainPos.vPosition = _float4(25.f, 0.f, 87.f + Train4, 1.f);
		Train4 = Train4 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Train"), &TrainPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Train");
			return E_FAIL;
		}
	}

	_float	Road5 = 2.f;
	for (_uint i = 0; i < 2; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 89.f + Road5, 1.f);
		Road5 = Road5 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass9 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 93.f + Grass9, 1.f);
		Grass9 = Grass9 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Road6 = 2.f;
	for (_uint i = 0; i < 4; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 95.f + Road6, 1.f);
		Road6 = Road6 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass10 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 103.f + Grass10, 1.f);
		Grass10 = Grass10 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Road7 = 2.f;
	for (_uint i = 0; i < 3; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 105.f + Road7, 1.f);
		Road7 = Road7 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float Grass11 = 2.f;
	for (_uint j = 0; j < 1; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 111.f + Grass11, 1.f);
		Grass11 = Grass11 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	_float	Train5 = 2.f;
	for (_uint i = 0; i < 2; ++i)
	{
		CTrain::TrainPosition		TrainPos;
		ZeroMemory(&TrainPos, sizeof(CTrain::TrainPosition));
		TrainPos.vPosition = _float4(25.f, 0.f, 113.f + Train5, 1.f);
		Train5 = Train5 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Train"), &TrainPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Train");
			return E_FAIL;
		}
	}

	_float	Road8 = 2.f;
	for (_uint i = 0; i < 3; ++i)
	{
		CRoad::RoadPosition		RoadPos;
		ZeroMemory(&RoadPos, sizeof(CRoad::RoadPosition));
		RoadPos.vPosition = _float4(25.f, 0.f, 117.f + Road8, 1.f);
		Road8 = Road8 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Road"), &RoadPos)))
		{
			MSGBOX("Failed to Game Pdlay Prototype_GameObject_Road");
			return E_FAIL;
		}
	}

	_float	Water4 = 2.f;
	for (_uint i = 0; i < 1; ++i)
	{
		CWater::WaterPosition	WaterPos;
		ZeroMemory(&WaterPos, sizeof(CWater::WaterPosition));
		WaterPos.vPosition = _float4(25.f, 0.f, 123.f + Water4, 1.f);
		Water4 = Water4 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Water"), &WaterPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Water");
			return E_FAIL;
		}
	}

	_float Grass12 = 2.f;
	for (_uint j = 0; j < 12; ++j)
	{
		CGrass::GrassPosition	GrassPos;
		ZeroMemory(&GrassPos, sizeof(CGrass::GrassPosition));
		GrassPos.vPosition = _float4(25.f, 0.f, 125.f + Grass12, 1.f);
		Grass12 = Grass12 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grass"), &GrassPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Grass");
			return E_FAIL;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Psy"))))
	//{
	//	MSGBOX("Failed to GamePlay Prototype_GameObject_Psy");
	//	return E_FAIL;
	//}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_Chicken"))))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Player_Chicken");
		return E_FAIL;
	}

	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Samurai"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Samurai"))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//=======================Train============================//
	CMonster_Train::TrainFront		FrontPos;
	ZeroMemory(&FrontPos, sizeof(CMonster_Train::TrainFront));
	FrontPos.vPosition = _float4(45.f, 0.7f, 9.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(-7.f, 0.7f, 11.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(180.f);
	FrontPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(-7.f, 0.7f, 27.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(180.f);
	FrontPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(-7.f, 0.7f, 77.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(180.f);
	FrontPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(45.f, 0.7f, 79.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(0.f);
	FrontPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(45.f, 0.7f, 81.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(0.f);
	FrontPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(-7.f, 0.7f, 83.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(180.f);
	FrontPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(45.f, 0.7f, 89.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(0.f);
	FrontPos.fSpeed = _float(20.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(-7.f, 0.7f, 115.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(180.f);
	FrontPos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	FrontPos.vPosition = _float4(45.f, 0.7f, 117.f, 1.f);
	FrontPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	FrontPos.fAngle = _float(0.f);
	FrontPos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Train"), &FrontPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}

	CMonster_TrainMiddle::TrainMiddle	MiddlePos;
	ZeroMemory(&MiddlePos, sizeof(CMonster_TrainMiddle::TrainMiddle));
	MiddlePos.vPosition = _float4(54.f, 0.7f, 9.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(-16.f, 0.7f, 11.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(180.f);
	MiddlePos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(-16.f, 0.7f, 27.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(180.f);
	MiddlePos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(-16.f, 0.7f, 77.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(180.f);
	MiddlePos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(54.f, 0.7f, 79.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(0.f);
	MiddlePos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(54.f, 0.7f, 81.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(0.f);
	MiddlePos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(-16.f, 0.7f, 83.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(180.f);
	MiddlePos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(54.f, 0.7f, 89.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(0.f);
	MiddlePos.fSpeed = _float(20.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(-16.f, 0.7f, 115.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(180.f);
	MiddlePos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	MiddlePos.vPosition = _float4(54.f, 0.7f, 117.f, 1.f);
	MiddlePos.vScale = _float3(0.2f, 0.2f, 0.2f);
	MiddlePos.fAngle = _float(0.f);
	MiddlePos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainMiddle"), &MiddlePos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}

	CMonster_TrainEnd::TrainEnd		EndPos;
	ZeroMemory(&EndPos, sizeof(CMonster_TrainEnd::TrainEnd));
	EndPos.vPosition = _float4(61.f, 0.7f, 9.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(-23.f, 0.7f, 11.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(180.f);
	EndPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(-23.f, 0.7f, 27.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(180.f);
	EndPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(-23.f, 0.7f, 77.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(180.f);
	EndPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(61.f, 0.7f, 79.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(0.f);
	EndPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(61.f, 0.7f, 81.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(0.f);
	EndPos.fSpeed = _float(50.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(-23.f, 0.7f, 83.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(180.f);
	EndPos.fSpeed = _float(40.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(61.f, 0.7f, 89.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(0.f);
	EndPos.fSpeed = _float(20.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(-23.f, 0.7f, 115.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(180.f);
	EndPos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	EndPos.vPosition = _float4(61.f, 0.7f, 117.f, 1.f);
	EndPos.vScale = _float3(0.2f, 0.2f, 0.2f);
	EndPos.fAngle = _float(0.f);
	EndPos.fSpeed = _float(70.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_TrainEnd"), &EndPos)))
	{
		MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	//==========================================================//

	//========================Car==================================//
	_float	Moncar1 = 0.f;
	for (_uint i = 0; i < 10; ++i)
	{
		//GreenCar
		CMonster_Car::CarPosition	CarPos;
		ZeroMemory(&CarPos, sizeof(CMonster_Car::CarPosition));
		CarPos.vPosition = _float4(45.f + Moncar1, 0.3f, 15.f, 1.f);
		CarPos.fAngle = _float(180.f);
		CarPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		CarPos.fSpeed = _float(5.f);
		Moncar1 = Moncar1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Car"), &CarPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Car");
			return E_FAIL;
		}

		CarPos.vPosition = _float4(-90.f + Moncar1, 0.3f, 55.f, 1.f);
		CarPos.fAngle = _float(0.f);
		CarPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		CarPos.fSpeed = _float(5.f);
		Moncar1 = Moncar1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Car"), &CarPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Car");
			return E_FAIL;
		}

		CarPos.vPosition = _float4(-90.f + Moncar1, 0.3f, 103.f, 1.f);
		CarPos.fAngle = _float(0.f);
		CarPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		CarPos.fSpeed = _float(5.f);
		Moncar1 = Moncar1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Car"), &CarPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Car");
			return E_FAIL;
		}
	}
	
	_float	KorCar1 = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//korea box truck
		CKoreaCar001::KorCar1Position	KorCarPos1;
		ZeroMemory(&KorCarPos1, sizeof(CKoreaCar001::KorCar1Position));
		KorCarPos1.vPosition = _float4(-30.f + KorCar1, 0.3f, 17.f, 1.f);
		KorCarPos1.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos1.fSpeed = _float(5.f);
		KorCarPos1.fAngle = _float(0.f);
		KorCar1 = KorCar1 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar001"), &KorCarPos1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar001");
			return E_FAIL;
		}

		KorCarPos1.vPosition = _float4(-120.f + KorCar1, 0.3f, 97.f, 1.f);
		KorCarPos1.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos1.fSpeed = _float(5.f);
		KorCarPos1.fAngle = _float(0.f);
		KorCar1 = KorCar1 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar001"), &KorCarPos1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar001");
			return E_FAIL;
		}
	}
	

	_float	KorCar2 = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//katamari car
		CKoreaCar002::KorCar2Position	KorCarPos2;
		ZeroMemory(&KorCarPos2, sizeof(CKoreaCar002::KorCar2Position));
		KorCarPos2.vPosition = _float4(45.f + KorCar2, 0.3f, 51.f, 1.f);
		KorCarPos2.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos2.fAngle = _float(180.f);
		KorCarPos2.fSpeed = _float(8.f);
		KorCar2 = KorCar2 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar002"), &KorCarPos2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar002");
			return E_FAIL;
		}
		KorCarPos2.vPosition = _float4(-60.f + KorCar2, 0.3f, 93.f, 1.f);
		KorCarPos2.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos2.fAngle = _float(0.f);
		KorCarPos2.fSpeed = _float(15.f);
		KorCar2 = KorCar2 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar002"), &KorCarPos2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar002");
			return E_FAIL;
		}

		KorCarPos2.vPosition = _float4(-60.f + KorCar2, 0.3f, 121.f, 1.f);
		KorCarPos2.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos2.fAngle = _float(0.f);
		KorCarPos2.fSpeed = _float(15.f);
		KorCar2 = KorCar2 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar002"), &KorCarPos2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar002");
			return E_FAIL;
		}
	}

	
	_float	KorCar3 = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//GrayCar
		CKoreaCar003::KorCar3Position	KorCarPos3;
		ZeroMemory(&KorCarPos3, sizeof(CKoreaCar003::KorCar3Position));
		KorCarPos3.vPosition = _float4(45.f + KorCar3, 0.3f, 53.f, 1.f);
		KorCarPos3.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos3.fAngle = _float(0.f);
		KorCarPos3.fSpeed = _float(5.f);
		KorCar3 = KorCar3 + 13.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar003"), &KorCarPos3)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar003");
			return E_FAIL;
		}
		KorCarPos3.vPosition = _float4(-60.f + KorCar3, 0.3f, 91.f, 1.f);
		KorCarPos3.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos3.fAngle = _float(180.f);
		KorCarPos3.fSpeed = _float(10.f);
		KorCar3 = KorCar3 + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar003"), &KorCarPos3)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar003");
			return E_FAIL;
		}

		KorCarPos3.vPosition = _float4(-60.f + KorCar3, 0.3f, 123.f, 1.f);
		KorCarPos3.vScale = _float3(0.15f, 0.15f, 0.15f);
		KorCarPos3.fAngle = _float(180.f);
		KorCarPos3.fSpeed = _float(10.f);
		KorCar3 = KorCar3 + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_KoreaCar003"), &KorCarPos3)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_KoreaCar003");
			return E_FAIL;
		}
	}

	_float	Muccle1 = 0.f;
	for (_uint i = 0; i < 15; ++i)
	{
		CMuccle::MucclePosition		MucclePos1;
		ZeroMemory(&MucclePos1, sizeof(CMuccle::MucclePosition));
		MucclePos1.vPosition = _float4(-90.f + Muccle1, 0.3f, 57.f, 1.f);
		MucclePos1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MucclePos1.fAngle = _float(180.f);
		MucclePos1.fSpeed = _float(10.f);
		Muccle1 = Muccle1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Muccle"), &MucclePos1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Muccle");
			return E_FAIL;
		}

		MucclePos1.vPosition = _float4(-90.f + Muccle1, 0.3f, 73.f, 1.f);
		MucclePos1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MucclePos1.fAngle = _float(180.f);
		MucclePos1.fSpeed = _float(15.f);
		Muccle1 = Muccle1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Muccle"), &MucclePos1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Muccle");
			return E_FAIL;
		}

		MucclePos1.vPosition = _float4(-90.f + Muccle1, 0.3f, 119.f, 1.f);
		MucclePos1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MucclePos1.fAngle = _float(180.f);
		MucclePos1.fSpeed = _float(15.f);
		Muccle1 = Muccle1 + 15.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Muccle"), &MucclePos1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Muccle");
			return E_FAIL;
		}
	}


	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Taxi"))))
	//{
	//	MSGBOX("Failed to Game Play Prototype_GameObject_Monster_Taxi");
	//	return E_FAIL;
	//}

	_float	Truck1 = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//red truck
		CMonster_Truck::MonTruck		MonTruck1;
		ZeroMemory(&MonTruck1, sizeof(CMonster_Truck::MonTruck));
		MonTruck1.vPosition = _float4(-30.f + Truck1, 0.5f, 31.f, 1.f);
		MonTruck1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck1.fAngle = _float(0.f);
		MonTruck1.fSpeed = _float(5.f);
		Truck1 = Truck1 + 20.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck"), &MonTruck1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck");
			return E_FAIL;
		}

		MonTruck1.vPosition = _float4(45.f + Truck1, 0.5f, 35.f, 1.f);
		MonTruck1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck1.fAngle = _float(180.f);
		MonTruck1.fSpeed = _float(8.f);
		Truck1 = Truck1 + 16.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck"), &MonTruck1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck");
			return E_FAIL;
		}

		MonTruck1.vPosition = _float4(45.f + Truck1, 0.5f, 71.f, 1.f);
		MonTruck1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck1.fAngle = _float(180.f);
		MonTruck1.fSpeed = _float(10.f);
		Truck1 = Truck1 + 8.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck"), &MonTruck1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck");
			return E_FAIL;
		}

		MonTruck1.vPosition = _float4(30.f + Truck1, 0.5f, 99.f, 1.f);
		MonTruck1.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck1.fAngle = _float(180.f);
		MonTruck1.fSpeed = _float(10.f);
		Truck1 = Truck1 + 5.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck"), &MonTruck1)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck");
			return E_FAIL;
		}
	}

	_float	Truck2 = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//blue truck
		CMonster_Truck2::MonTruck2		MonTruck2;
		ZeroMemory(&MonTruck2, sizeof(CMonster_Truck::MonTruck));
		MonTruck2.vPosition = _float4(45.f + Truck2, 0.5f, 33.f, 1.f);
		MonTruck2.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck2.fAngle = _float(180.f);
		MonTruck2.fSpeed = _float(5.f);
		Truck2 = Truck2 + 17.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck2"), &MonTruck2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck2");
			return E_FAIL;
		}
		MonTruck2.vPosition = _float4(-30.f + Truck2, 0.5f, 69.f, 1.f);
		MonTruck2.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck2.fAngle = _float(0.f);
		MonTruck2.fSpeed = _float(8.f);
		Truck2 = Truck2 + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck2"), &MonTruck2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck2");
			return E_FAIL;
		}

		MonTruck2.vPosition = _float4(-30.f + Truck2, 0.5f, 101.f, 1.f);
		MonTruck2.vScale = _float3(0.15f, 0.15f, 0.15f);
		MonTruck2.fAngle = _float(0.f);
		MonTruck2.fSpeed = _float(8.f);
		Truck2 = Truck2 + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_Truck2"), &MonTruck2)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_Truck2");
			return E_FAIL;
		}
	}

	_float	OilTruck = 0.f;
	for (_uint i = 0; i < 20; ++i)
	{
		//blue truck
		CMonster_OilTruck::OilTruck		OilTruckPos;
		ZeroMemory(&OilTruckPos, sizeof(CMonster_OilTruck::OilTruck));
		OilTruckPos.vPosition = _float4(-30.f + OilTruck, 0.5f, 29.f, 1.f);
		OilTruckPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		OilTruckPos.fAngle = _float(0.f);
		OilTruckPos.fSpeed = _float(5.f);
		OilTruck = OilTruck + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_OilTruck"), &OilTruckPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_OilTruck");
			return E_FAIL;
		}

		OilTruckPos.vPosition = _float4(30.f + OilTruck, 0.5f, 67.f, 1.f);
		OilTruckPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		OilTruckPos.fAngle = _float(180.f);
		OilTruckPos.fSpeed = _float(7.f);
		OilTruck = OilTruck + 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_OilTruck"), &OilTruckPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_OilTruck");
			return E_FAIL;
		}

		OilTruckPos.vPosition = _float4(30.f + OilTruck, 0.5f, 107.f, 1.f);
		OilTruckPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		OilTruckPos.fAngle = _float(180.f);
		OilTruckPos.fSpeed = _float(10.f);
		OilTruck = OilTruck + 5.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_OilTruck"), &OilTruckPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_OilTruck");
			return E_FAIL;
		}

		OilTruckPos.vPosition = _float4(-30.f + OilTruck, 0.5f, 109.f, 1.f);
		OilTruckPos.vScale = _float3(0.15f, 0.15f, 0.15f);
		OilTruckPos.fAngle = _float(0.f);
		OilTruckPos.fSpeed = _float(10.f);
		OilTruck = OilTruck + 5.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster_OilTruck"), &OilTruckPos)))
		{
			MSGBOX("Failed to GamePlay Prototype_GameObject_Monster_OilTruck");
			return E_FAIL;
		}
	}
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Item(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint Item1 = 0;
	for (_uint i = 0; i < 25; ++i)
	{
		CItem::ITEMDESC		ItemPos;
		ZeroMemory(&ItemPos, sizeof(CItem::ITEMDESC));
		ItemPos.vItemPos = _float4(rand() % 30, 0.7f, 6.f + Item1, 1.f);
		ItemPos.fItemValue = 100.f;
		Item1 = Item1 + rand() % 10;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Item"), &ItemPos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Item");
			return E_FAIL;
		}
	}
	/*if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("8BIT_WONDER"), TEXT("../Bin/Resources/CrossRoad/Font/8BIT_WONDER.spritefont"))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//for (_uint i = 0; i < 30; ++i)
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
	//		return E_FAIL;
	//	MSGBOX("Failed to Game Play Prototype_GameObject_Effect");
	//}	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Structure(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float	Tree1 = 0.f;
	for (_uint i = 0; i < 5; ++i)
	{
		CTree::TreePosition		TreePos;
		ZeroMemory(&TreePos, sizeof(CTree::TreePosition));
		//1pos
		TreePos.vPosition = _float4(1.f + Tree1, 0.3f, 1.f, 1.f);
		Tree1 = Tree1 + 2.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
			return E_FAIL;
		}
		//3pos
		TreePos.vPosition = _float4(Tree1, 0.3f, 3.f, 1.f);
		Tree1 = rand() % 16;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
			return E_FAIL;
		}
		//5pos
		TreePos.vPosition = _float4(Tree1, 0.3f, 5.f, 1.f);
		Tree1 = rand() % 5;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
			return E_FAIL;
		}
		//CTree::TreePosition		TreePos;
		//ZeroMemory(&TreePos, sizeof(CTree::TreePosition));
		//7pos
		TreePos.vPosition = _float4(Tree1 + 36, 0.3f, 7.f, 1.f);
		Tree1 =  rand() % 7;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
			return E_FAIL;
		}
		
	}
	
	CTree::TreePosition		TreePos;
	ZeroMemory(&TreePos, sizeof(CTree::TreePosition));
	//-3pos
	TreePos.vPosition = _float4(1.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(23.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(28.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}

	//7pos
	TreePos.vPosition = _float4(28.f, 0.3f, 7.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(36.f, 0.3f, 7.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(38.f, 0.3f, 7.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//25pos
	TreePos.vPosition = _float4(38.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//37pos
	TreePos.vPosition = _float4(6.f, 0.3f, 37.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}

	TreePos.vPosition = _float4(10.f, 0.3f, 37.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}

	TreePos.vPosition = _float4(25.f, 0.3f, 37.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//49pos
	//61pos
	//63pos
	TreePos.vPosition = _float4(20.f, 0.3f, 63.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(28.f, 0.3f, 63.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//65pos
	//75pos
	//85pos
	//87pos
	//95pos
	TreePos.vPosition = _float4(20.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(28.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(36.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(44.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//105pos
	//113pos
	TreePos.vPosition = _float4(2.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(6.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(8.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(10.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(16.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//127pos
	//129pos
	//131pos
	TreePos.vPosition = _float4(44.f, 0.3f, 131.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(46.f, 0.3f, 131.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//133pos
	//135pos
	TreePos.vPosition = _float4(10.f, 0.3f, 135.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(14.f, 0.3f, 135.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	//137pos
	//139pos
	//141pos
	TreePos.vPosition = _float4(26.f, 0.3f, 141.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}
	TreePos.vPosition = _float4(28.f, 0.3f, 141.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
		return E_FAIL;
	}

	CTreeA::TreeAPosition	TreeAPos;
	ZeroMemory(&TreeAPos, sizeof(CTreeA::TreeAPosition));
	TreeAPos.vPosition = _float4(1.f, 0.3f, 3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	TreeAPos.vPosition = _float4(1.f, 0.3f, 3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	//-5pos
	TreeAPos.vPosition = _float4(10.f, 0.3f, -5.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	TreeAPos.vPosition = _float4(16.f, 0.3f, -5.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	TreeAPos.vPosition = _float4(23.f, 0.3f, -5.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	//13pos
	TreeAPos.vPosition = _float4(6.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	TreeAPos.vPosition = _float4(10.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//25pos
	TreeAPos.vPosition = _float4(36.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(44.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}

	//37pos
	TreeAPos.vPosition = _float4(16.f, 0.3f, 37.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(23.f, 0.3f, 37.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//49pos
	TreeAPos.vPosition = _float4(28.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//61pos
	TreeAPos.vPosition = _float4(28.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//63pos
	//65pos
	//75pos
	TreeAPos.vPosition = _float4(18.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(8.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(48.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//85pos
	TreeAPos.vPosition = _float4(26.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(44.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(10.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//87pos
	//95pos
	TreeAPos.vPosition = _float4(2.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(6.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(8.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(10.f, 0.3f, 95.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//105pos
	TreeAPos.vPosition = _float4(6.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(8.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	TreeAPos.vPosition = _float4(10.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//113pos
	//127pos
	TreeAPos.vPosition = _float4(30.f, 0.3f, 127.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//129pos
	TreeAPos.vPosition = _float4(34.f, 0.3f, 127.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeA"), &TreeAPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	//131pos
	//133pos
	//135pos
	//137pos
	//139pos
	//141pos

	CTreeB::TreeBPosition	TreeBPos;
	ZeroMemory(&TreeBPos, sizeof(CTreeB::TreeBPosition));

	//-3pos
	TreeBPos.vPosition = _float4(8.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}

	TreeBPos.vPosition = _float4(12.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}

	TreeBPos.vPosition = _float4(16.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}

	TreeBPos.vPosition = _float4(33.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}

	//13pos
	TreeBPos.vPosition = _float4(1.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(45.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(41.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(36.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(32.f, 0.3f, 13.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}

	//23pos
	TreeBPos.vPosition = _float4(6.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(16.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(18.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(36.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(40.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(44.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//37pos
	//49pos
	TreeBPos.vPosition = _float4(4.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(6.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//61pos
	TreeBPos.vPosition = _float4(2.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(6.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(8.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//63pos
	//65pos
	TreeBPos.vPosition = _float4(6.f, 0.3f, 65.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(22.f, 0.3f, 65.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(36.f, 0.3f, 65.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//75pos
	//85pos
	TreeBPos.vPosition = _float4(4.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(16.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(38.f, 0.3f, 85.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//87pos
	TreeBPos.vPosition = _float4(4.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(40.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(28.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//95pos
	//105pos
	TreeBPos.vPosition = _float4(26.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(28.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(40.f, 0.3f, 105.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//113pos
	//127pos
	TreeBPos.vPosition = _float4(6.f, 0.3f, 127.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(10.f, 0.3f, 127.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	TreeBPos.vPosition = _float4(30.f, 0.3f, 127.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//129pos
	//131pos
	TreeBPos.vPosition = _float4(10.f, 0.3f, 131.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeB"), &TreeBPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	//133pos
	//135pos
	//137pos
	//139pos
	//141pos

	CTreeC::TreeCPosition	TreeCPos;
	ZeroMemory(&TreeCPos, sizeof(CTreeC::TreeCPosition));
	TreeCPos.vPosition = _float4(1.f, 0.3f, 23.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}

	//-3pos
	TreeCPos.vPosition = _float4(40.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(43.f, 0.3f, -3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}

	//3pos
	TreeCPos.vPosition = _float4(14.f, 0.3f, 3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(26.f, 0.3f, 3.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}

	//25pos
	TreeCPos.vPosition = _float4(4.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(6.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(6.f, 0.3f, 25.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//37pos
	//49pos
	TreeCPos.vPosition = _float4(10.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(30.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(38.f, 0.3f, 49.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//61pos
	TreeCPos.vPosition = _float4(30.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(38.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(44.f, 0.3f, 61.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//63pos
	TreeCPos.vPosition = _float4(10.f, 0.3f, 63.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(16.f, 0.3f, 63.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//65pos
	//75pos
	TreeCPos.vPosition = _float4(26.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(38.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(44.f, 0.3f, 75.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//85pos
	//87pos
	TreeCPos.vPosition = _float4(16.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(10.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(8.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(6.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(32.f, 0.3f, 87.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//95pos
	//105pos
	//113pos
	TreeCPos.vPosition = _float4(22.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(30.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(32.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	TreeCPos.vPosition = _float4(40.f, 0.3f, 113.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//127pos
	//129pos
	TreeCPos.vPosition = _float4(8.f, 0.3f, 129.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//131pos
	TreeCPos.vPosition = _float4(4.f, 0.3f, 131.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_TreeC"), &TreeCPos)))
	{
		MSGBOX("Failed to Game Play Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	//133pos
	//135pos
	//137pos
	//139pos
	//141pos
	/*(12.5f, -5.f, 0.5f, 1.f)*/


	/*_float	Tree2 = 2.f;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &TreePos)))
		{
			MSGBOX("Failed to Game Play Prototype_GameObject_Tree");
			return E_FAIL;
		}
	}*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
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

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
