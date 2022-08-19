#include "stdafx.h"
#include "..\Public\UI_Loading_Twinkle.h"
#include "GameInstance.h"

CUI_Loading_Twinkle::CUI_Loading_Twinkle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI_Loading_Twinkle::CUI_Loading_Twinkle(const CUI_Loading_Twinkle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Loading_Twinkle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Twinkle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(2.f, XMConvertToRadians(0.f)))))
	{
		MSGBOX("Failed to CUI_Loading_Twinkle NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CUI_Loading_Twinkle SetUp_Components");
		return E_FAIL;
	}

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	_float	SizeX = 64.f;
	_float	SizeY = 64.f;
	_float	SettingX = m_fX - (g_iWinCX * 0.5f);
	_float	SettingY = -m_fY + (g_iWinCY * 0.5f);
	_float  SetX = 800.f;
	_float  SetY = 0.f;

	m_pTransform->Scaled(XMVectorSet(SizeX, SizeY, 1.f, 0.0f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(SettingX + SetX, SettingY + SetY, 0.f, 1.f));

	/*if (pArg != nullptr)
	{
		_float4*	vpInitial_Pos = static_cast<_float4*>(pArg);
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((vpInitial_Pos)));
	}*/

	return S_OK;
}

_int CUI_Loading_Twinkle::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	
	m_fFrame += 16.f * TimeDelta * 2.f;

	if (m_fFrame >= 16.f)
		m_fFrame = 0.f;

	return _int();
}

_int CUI_Loading_Twinkle::Late_Tick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("CUI_Money::LateTick : NotFound");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Loading_Twinkle::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
	{
		MSGBOX("Failed to Background SetUp_OnShader");
		return E_FAIL;
	}

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUI_Loading_Twinkle::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Twinkle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("Failed to Background Prototype_Component_Texture_UI_Loading_Twinkle");
		return E_FAIL;
	}

	return S_OK;
}

CUI_Loading_Twinkle * CUI_Loading_Twinkle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Loading_Twinkle*	pInstance = new CUI_Loading_Twinkle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Loading_Twinkle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Loading_Twinkle::Clone(void * pArg)
{
	CUI_Loading_Twinkle*	pInstance = new CUI_Loading_Twinkle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Loading_Twinkle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Loading_Twinkle::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
