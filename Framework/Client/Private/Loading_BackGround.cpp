#include "stdafx.h"
#include "..\Public\Loading_BackGround.h"
#include  "GameInstance.h"

CLoading_BackGround::CLoading_BackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CLoading_BackGround::CLoading_BackGround(const CLoading_BackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoading_BackGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CLoading_BackGround NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoading_BackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
	{
		MSGBOX("Failed to CLoading_BackGround NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CLoading_BackGround SetUp_Components");
		return E_FAIL;
	}

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	return S_OK;
}

_int CLoading_BackGround::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CLoading_BackGround::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;
	//ÁÖÀÇ transformComÀÌ ¾Æ´Ô
	m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CLoading_BackGround::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	{
		MSGBOX("Failed to Background SetUp_OnShader");
		return E_FAIL;
	}

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CLoading_BackGround::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to Background Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to Background Prototype_Component_Shader_VTXTEX");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("Failed to Background Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BackGround"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("Failed to Background Prototype_Component_Texture_Loading_BackGround");
		return E_FAIL;
	}

	return S_OK;
}

CLoading_BackGround * CLoading_BackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLoading_BackGround*	pInstance = new CLoading_BackGround(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CLoading_BackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLoading_BackGround::Clone(void * pArg)
{
	CLoading_BackGround*	pInstance = new CLoading_BackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CLoading_BackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading_BackGround::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
