#include "stdafx.h"
#include "..\Public\Monster_Truck.h"
#include "GameInstance.h"

CMonster_Truck::CMonster_Truck(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{
}

CMonster_Truck::CMonster_Truck(const CMonster_Truck & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Truck::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster_truck NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Truck::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(5.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed to CMonster_truck NativeConstruct");
		return E_FAIL;
	}

	if(FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Truck SetUp_Components");
		return E_FAIL;
	}

	/*_float3		fTruckScale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fTruckScale));*/

	if (nullptr != pArg)
		memcpy(&m_tTruck, pArg, sizeof(MonTruck));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tTruck.vPosition.x, m_tTruck.vPosition.y, m_tTruck.vPosition.z, m_tTruck.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tTruck.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tTruck.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tTruck.fSpeed));

	return S_OK;
}

_int CMonster_Truck::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Right(TimeDelta);

	while (m_fTimeFrame > 15.f)
	{
		if (m_bExitCheck = true)
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tTruck.vPosition.x, m_tTruck.vPosition.y, m_tTruck.vPosition.z, m_tTruck.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}
	return _int();
}

_int CMonster_Truck::LateTick(_double TimedDelta)
{
	if (0 > __super::LateTick(TimedDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	/*__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));*/

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CMonster_Truck::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CMonster_Truck Render_Model");
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CMonster_Truck SetUp_ConstantTable");
		return E_FAIL;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	//m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster_Truck::Clear_Monster_Car()
{
}

HRESULT CMonster_Truck::SetUp_Components()
{
	//·»´õ·¯ÄÄÆ÷³ÍÆ®
	if(FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Truck Prototype_Component_Renderer");
		return E_FAIL;
	}

	//½¦ÀÌ´õÄÄÆ÷³ÍÆ®
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Truck Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//¸ðµ¨ÄÄÆ÷³ÍÆ®
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Truck"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Truck Prototype_Component_Model_Monster_Truck");
		return E_FAIL;
	}
	
	//aabbÃæµ¹ÄÄÆ÷³ÍÆ®
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(30.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 5.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Truck::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CMonster_Truck Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Truck Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Truck Set_RawValue(Proj)");
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

CMonster_Truck * CMonster_Truck::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Truck*		pInstance = new CMonster_Truck(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMonster_Truck");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Truck::Clone(void * pArg)
{
	CMonster_Truck*		pInstance = new CMonster_Truck(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMonster_Truck");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Truck::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
