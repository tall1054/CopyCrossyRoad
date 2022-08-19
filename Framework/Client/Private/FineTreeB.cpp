#include "stdafx.h"
#include "..\Public\FineTreeB.h"
#include "GameInstance.h"

CFineTreeB::CFineTreeB(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CFineTreeB::CFineTreeB(const CFineTreeB & rhs)
	: CLandObject(rhs)
{
}

HRESULT CFineTreeB::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFineTreeB::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tFineTreeBPos, pArg, sizeof(FineTreeBPosition));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tFineTreeBPos.vPosition.x, m_tFineTreeBPos.vPosition.y, m_tFineTreeBPos.vPosition.z, m_tFineTreeBPos.vPosition.w));

	return S_OK;
}

_int CFineTreeB::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CFineTreeB::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	return _int();
}

HRESULT CFineTreeB::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CFineTreeB::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FineTreeB"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(25.f, 0.35f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.2f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFineTreeB::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

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



CFineTreeB * CFineTreeB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CFineTreeB*	pInstance = new CFineTreeB(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CFineTreeB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFineTreeB::Clone(void * pArg)
{
	CFineTreeB*	pInstance = new CFineTreeB(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CFineTreeB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFineTreeB::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
