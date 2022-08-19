#include "stdafx.h"
#include "..\Public\Monster.h"
#include "GameInstance.h"


CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
	{
		MSGBOX("Failed to CMonster NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster SetUp_Components");
		return E_FAIL;
	}

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 10, 0.f, rand() % 10, 1.f));

	m_pModelCom->SetUp_AnimIndex(rand() % 10);

	return S_OK;
}

_int CMonster::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());
	m_pOBBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CMonster::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta);

	__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));
	m_pOBBCom->Collision_OBB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_OBB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CMonster::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CMonster Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CMonster SetUp_ConstantTable");
		return E_FAIL;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}
#ifdef _DEBUG
	m_pAABBCom->Render();
	m_pOBBCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMonster::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster Prototype_Component_Shader_AnimModel");
		return E_FAIL;
	}

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster Prototype_Component_Model_Fiona");
		return E_FAIL;
	}
	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.6f, 1.4f, 0.6f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.0f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.5f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CMonster Render_Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster Render_Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CMonster Render_Set_RawValue(Proj)");
		return E_FAIL;
	}

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
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



CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster*	pInstance = new CMonster(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg)
{
	CMonster*	pInstance = new CMonster(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
