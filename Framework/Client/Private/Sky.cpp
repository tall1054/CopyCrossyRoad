#include "stdafx.h"
#include "..\Public\Sky.h"
#include "GameInstance.h"


CSky::CSky(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CSky::CSky(const CSky & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CSky NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSky::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
	{
		MSGBOX("Failed to CSky NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CSky SetUp_Components");
		return E_FAIL;
	}

	return S_OK;
}

_int CSky::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CSky::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransform->Set_State(CTransform::STATE_POSITION, pGameInstance->Get_CamPosition());

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}




HRESULT CSky::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("Failed to CSky Render_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CSky Render_SetUp_ConstantTable");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
	{
		MSGBOX("Failed to CSky Render_VIBuffer_Shader");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSky::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CSky Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Cube"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CSky Prototype_Component_Shader_Cube");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("Failed to CSky Prototype_Component_VIBuffer_Cube");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("Failed to CSky Prototype_Component_Texture_Sky");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSky::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		MSGBOX("Failed to CSky Shader");
		return E_FAIL;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CSky Transform_Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CSky Shader_Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CSky Shader_Set_RawValue(Proj)");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 2)))
	{
		MSGBOX("Failed to CSky Texture_SetUp_OnShader(Diffuse)");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CSky * CSky::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CSky*	pInstance = new CSky(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSky::Clone(void * pArg)
{
	CSky*	pInstance = new CSky(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSky::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
