#include "stdafx.h"
#include "..\Public\Monster_Taxi.h"
#include "GameInstance.h"

CMonster_Taxi::CMonster_Taxi(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)	//용도가 무엇인가??
{
}

CMonster_Taxi::CMonster_Taxi(const CMonster_Taxi & rhs)
	: CLandObject(rhs)// 마찬가지로 용도가 무엇인가.
{
}

HRESULT CMonster_Taxi::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster_Taxi NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Taxi::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed tor CMonster_Taxi NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Taxi SetUP_Components");
		return E_FAIL;
	}

	//===========자동차의 크기와 그 특정 위치를 설정해준다.================//
	_float3		fTaxiScale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fTaxiScale));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(15.f, 0.f, 10.f, 1.f));

	return S_OK;
}

_int CMonster_Taxi::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Left(TimeDelta);

	return _int();
}

_int CMonster_Taxi::LateTick(_double TimedDelta)
{
	if (0 > __super::LateTick(TimedDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	//시간이 지나면 물체가 사라지고 다시 원위치로 만든다.
	return _int();
}

HRESULT CMonster_Taxi::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to Cmonster_Taxi Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to Cmonster_Taxi SetUp_constantTable");
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
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster_Taxi::Clear_Monster_Car()
{
}

HRESULT CMonster_Taxi::SetUp_Components()
{
	//renderer component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Taxi Prototype_Component_Renderer");
		return E_FAIL;
	}
	//shader component
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Taxi Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}
	//model component
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Taxi"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Taxi Prototype_Component_Model_Monster_Taxi");
		return E_FAIL;
	}

	//collider component
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Taxi::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CMonster_Taxi Bind_OnShader(World)");
		return E_FAIL;
	}
	//Get_TransformFloat4x4_TP의 역할과 사용법을 알아내고 어떻게 사용해야하는지 물어보자
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Taxi Set_RawValue(View)");
		return E_FAIL;
	}
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Taxi Set_RawValue(Proj)");
		return E_FAIL;
	}
	
	const LIGHTDESC*	pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

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

CMonster_Taxi * CMonster_Taxi::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Taxi*	pInstance = new CMonster_Taxi(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMonster_Taxi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Taxi::Clone(void * pArg)
{
	CMonster_Taxi*	pInstance = new CMonster_Taxi(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMonster_Taxi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Taxi::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
