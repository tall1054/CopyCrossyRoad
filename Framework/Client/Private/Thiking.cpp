//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

////카메라사본객체
//CCamera::CAMERADESC		CameraDesc;
//ZeroMemory(&CameraDesc, sizeof(CameraDesc));

//CTransform*	pPlayerSight_TransformCom = static_cast<CTransform*>(pGameInstance->Get_Component(Level_Korea, TEXT("Layer_Player"), TEXT("Com_Transform")));

//_float3	vAt = XMStoreFloat3(pPlayerSight_TransformCom->Get_State(CTransform::STATE_POSITION), );

//CameraDesc.vEye = _float3(15.f, 6.f, -7.f);
//CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
//CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

//CameraDesc.fFovy = XMConvertToRadians(30.0f);
//CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
//CameraDesc.fNear = 0.2f;
//CameraDesc.fFar = 300.f;

//CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
//CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

//if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
//{
//	MSGBOX("Failed to Game Play Add_GameObject");
//	return E_FAIL;
//}

//RELEASE_INSTANCE(CGameInstance);

//return S_OK;

/*_float	Time = 0.f;
_vector vPosition = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_POSITION);
_vector	vRight = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_RIGHT);
_vector vLook = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_LOOK);
_vector vUp = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_UP);*/
//vRight = XMVectorGetX(vRight);
/*CTransform* pPlayerCamera = static_cast<CTransform*>(pGameInstance->Get_Component(Level_Korea, TEXT("Layer_Player"), TEXT("Com_Transform")));*/

/* 사본객체를 생성ㅎ나다. */

//xmfloat3 = xmfloat3(lookAt으로 해두면 좋지 않을까??)
//CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
//CameraDesc.vAt = XMStoreFloat3(&(_float3*)CTransform::Go_Straight(TimeDelta)/*float3의 목적지*/, CTransform::LookAt(position,time)/*목적지는어디인가??*/);

//CTransform*	pPlayerSight_TransformCom = static_cast<CTransform*>(pGameInstance->Get_Component(Level_Korea, TEXT("Layer_Player"), TEXT("Com_Transform")));


//if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
//	return E_FAIL;


//if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
//{
//	MSGBOX("Failed to Game Play Prototype_GameObject_Sky");
//	return E_FAIL;
//}

/*if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Player"))))
{
MSGBOX("Failed to Game Play Prototype_GameObject_Player");
return E_FAIL;
}

CModel*		pModel = (CModel*)pGameInstance->Get_Component(Level_Korea, pLayerTag, TEXT("Com_Model"), 0);
CTransform*	pTransform = (CTransform*)pGameInstance->Get_Component(Level_Korea, pLayerTag, CGameObject::m_pTransformTag, 0);

CSword::SWORDDESC			SwordDesc;
SwordDesc.pModel = pModel;
SwordDesc.pSocketName = "SWORD";
SwordDesc.pTransform = pTransform;

if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Sword"), &SwordDesc)))
{
MSGBOX("Failed to Game Play Prototype_GameObject_Sword");
return E_FAIL;
}*/


/*for (_uint i = 0; i < 10; ++i)
{
if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Monster"))))
return E_FAIL;
}*/



//==================Junk========================//


/*if (FAILED(pGameInstance->Add_GameObject(Level_Korea, pLayerTag, TEXT("Prototype_GameObject_Fork"))))
{
MSGBOX("Failed to Game Play Prototype_GameObject_Fork");
return E_FAIL;
}*/

//_float	GrassPos = 1.f;
//for (_uint i = 1; i < 4; ++i)
//{
//	CGrass::GrassPosition	GrassPos1;
//	ZeroMemory(&GrassPos1, sizeof(CGrass::GrassPosition));
//	GrassPos1.vAxis = _float3(1.f, 0.f, 0.f);
//	GrassPos1.fAngle = 0.f;
//	GrassPos1.vPosition = _float3(12.5f, 0.5f, 0.5f + GrassPos);
//	GrassPos = GrassPos + 1.f;
//	GrassPos1.vScale = _float3(0.f, 0.f, 0.f);
//	
//}


//#ifdef _DEBUG
//	++m_dwNumRender;
//
//	if (m_dTimerAcc >= 1.f)
//	{
//		wsprintf(m_szFPS, TEXT("FPS : %d"), m_dwNumRender);
//		SetWindowText(g_hWnd, m_szFPS);
//
//		m_dwNumRender = 0;
//		m_dTimerAcc = 0.f;
//	}
//	m_pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
//
//#endif // _DEBUG
//if ()
//{
//	//플레이어 점수를 올리는 방법 1.

//CGameObject*	pInstance = GET_INSTANCE(CGameObject);

/*m_pGameInstance->Get_Component(Level_Korea, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Psy"), 1);*/