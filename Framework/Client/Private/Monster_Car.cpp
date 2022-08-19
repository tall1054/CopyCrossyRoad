#include "stdafx.h"
#include "..\Public\Monster_Car.h"
#include "GameInstance.h"

CMonster_Car::CMonster_Car(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLandObject(pDevice, pDeviceContext)
{
}

CMonster_Car::CMonster_Car(const CMonster_Car & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Car::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster_Car NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Car::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed tor CMonster_Car NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Car SetUP_Components");
		return E_FAIL;
	}

	//===========자동차의 크기와 그 특정 위치를 설정해준다.================//
	//_float3		fCarScale = _float3(0.15f, 0.1f, 0.15f);
	//m_pTransform->Scaled(XMLoadFloat3(&fCarScale));

	if (nullptr != pArg)
		memcpy(&m_tCarPos, pArg, sizeof(CarPosition));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tCarPos.vPosition.x, m_tCarPos.vPosition.y, m_tCarPos.vPosition.z, m_tCarPos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tCarPos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tCarPos.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tCarPos.fSpeed));


	return S_OK;
}

_int CMonster_Car::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += (_float)TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Right(TimeDelta);
	//while문으로 무한 반복처리해두며 반복조건으로 일정시간에 도달하면
	while (m_fTimeFrame > 15.f)
	{
		//탈출처리를 해두게 두며
		if (m_bExitCheck = true)
		{
			//탈추위치는 원상복구가 된 위치로 바꾸어 둔다.
			//그리고 그에 대한 시간을 0으로 초기화해둔다.
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tCarPos.vPosition.x, m_tCarPos.vPosition.y, m_tCarPos.vPosition.z, m_tCarPos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}
	return _int();
}

_int CMonster_Car::LateTick(_double TimedDelta)
{
	if (0 > __super::LateTick(TimedDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	//시간이 지나면 물체가 사라지고 다시 원위치로 만든다.
	return _int();
}

HRESULT CMonster_Car::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to Cmonster_Car Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to Cmonster_Car SetUp_constantTable");
		return E_FAIL;
	}

	_uint	iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

	//디버그일때 충돌체 만들어둔다.
#ifdef _DEBUG
	//m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster_Car::Clear_Monster_Car()
{
	
}

HRESULT CMonster_Car::SetUp_Components()
{
	//렌더러 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Renderer");
		return E_FAIL;
	}

	//쉐이더 컴포넌트?? 렌더러는 스테틱영역이고 쉐이더는 레벨플레이이다. 그 차이 알아보자
	if(FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//모델 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Car"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Model_Monster_Car");
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	//AABB충돌 컴포넌트
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f ,1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);


	//다른 애드컴포넌트와 다르게 콜라이더는 인자: 콜라이더 디스크립션 더 추가해서 애드 컴포넌트를 완성시킨다 이유는??
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Car::SetUp_ConstantTable()
{
	//이 함수의 기능을 알아봐야 한다.
	//빌드하는 도중에 인스턴스가 먹히지 않은 경우가 발생했다. 그럼 어찌할 것인가?
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CPlayer Render_Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CPlayer  Render_Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CPlayer Render_Set_RawValue(Proj)");
		return E_FAIL;
	}

	//빛 구조체의 내용이다. 빛의 구조체의 0번째의 열거체를 얻어내기 직사빛이으로 추정한다. 자세한건 모르니 물어보자
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	//쉐이더에서 빛의 위치를 얻어낸다.
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMonster_Car * CMonster_Car::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Car*	pInstance = new CMonster_Car(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMonster_Car");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Car::Clone(void * pArg)
{
	CMonster_Car*	pInstance = new CMonster_Car(*this);

	if(FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMonster_Car");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Car::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
