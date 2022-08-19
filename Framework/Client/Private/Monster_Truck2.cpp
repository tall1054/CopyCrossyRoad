#include "stdafx.h"
#include "..\Public\Monster_Truck2.h"
#include "GameInstance.h"

CMonster_Truck2::CMonster_Truck2(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{
}

CMonster_Truck2::CMonster_Truck2(const CMonster_Truck2 & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Truck2::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster_Truck2 NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Truck2::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(5.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed to CMonster_Truck2 NativeConstruct");
		return E_FAIL;
	}

	if(FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Truck2 SetUp_Components");
		return E_FAIL;
	}

	/*_float3		fTruckScale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fTruckScale));*/

	if (nullptr != pArg)
		memcpy(&m_tTruck2, pArg, sizeof(MonTruck2));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tTruck2.vPosition.x, m_tTruck2.vPosition.y, m_tTruck2.vPosition.z, m_tTruck2.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tTruck2.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tTruck2.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tTruck2.fSpeed));


	return S_OK;
}

_int CMonster_Truck2::Tick(_double TimeDelta)
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
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tTruck2.vPosition.x, m_tTruck2.vPosition.y, m_tTruck2.vPosition.z, m_tTruck2.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}
	return _int();
}

_int CMonster_Truck2::LateTick(_double TimedDelta)
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

HRESULT CMonster_Truck2::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CMonster_Truck2 Render_Model");
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CMonster_Truck2 SetUp_ConstantTable");
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

void CMonster_Truck2::Clear_Monster_Car()
{
}

HRESULT CMonster_Truck2::SetUp_Components()
{
	//������������Ʈ
	if(FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Truck2 Prototype_Component_Renderer");
		return E_FAIL;
	}

	//���̴�������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Truck2 Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//��������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Truck2"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Truck2 Prototype_Component_Model_Monster_Truck2");
		return E_FAIL;
	}
	
	//aabb�浹������Ʈ
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

HRESULT CMonster_Truck2::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CMonster_Truck2 Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Truck2 Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster_Truck2 Set_RawValue(Proj)");
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

CMonster_Truck2 * CMonster_Truck2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Truck2*		pInstance = new CMonster_Truck2(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMonster_Truck2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Truck2::Clone(void * pArg)
{
	CMonster_Truck2*		pInstance = new CMonster_Truck2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMonster_Truck2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Truck2::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
