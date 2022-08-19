#include "stdafx.h"
#include "..\Public\UI_Money.h"
#include "GameInstance.h"

CUI_Money::CUI_Money(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI_Money::CUI_Money(const CUI_Money & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Money::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Money::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//UI는 직교 투영을 해두어야하기에
	//표현하고자하는 UI의 크기
	//실제 UI의 사이즈

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	//64.f
	m_fX = g_iWinCX * 0.5f;
	//1024.f
	m_fY = g_iWinCY * 0.5f;
	//72.f
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.0f, 1.f)));

	//_float m_SizeX = 1280 m_fx = 540
	//_float m_SizeY = 720 m_fy = 360
	////UI의 뷰포트상의 위치
	//
	//_float SetX = 1024.f;
	//_float SetY = 72.f;
	//직교투영1.가로길이, 2.세로길이

	_float	SizeX = 64.f;
	_float	SizeY = 64.f;
	_float	SettingX = m_fX - (g_iWinCX * 0.5f);		//540-540
	_float	SettingY = -m_fY + (g_iWinCY * 0.5f);		//-360+360
	_float  SetX = 480.f;
	_float  SetY = 340.f;

	m_pTransform->Scaled(XMVectorSet(SizeX, SizeY, 1.f, 0.0f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(SettingX + SetX, SettingY + SetY, 0.f, 1.f));

	return S_OK;
}

_int CUI_Money::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CUI_Money::LateTick(_double TimeDelta)
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

HRESULT CUI_Money::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	{
		MSGBOX("Failed to CUI_Money::Render SetUp_OnShader");
		return E_FAIL;
	}
	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
	{
		MSGBOX("Failed to CUI_Money::Buffer->Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Money::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CUI_Money::SetUp_Component Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CUI_Money::SetUp_Component Shader");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("Failed to CUI_Money::SetUp_Component VIBufferRect");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Money"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("Failed to CUI_Money::SetUp_Component Texture");
		return E_FAIL;
	}

	return S_OK;
}

CUI_Money * CUI_Money::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Money*	pInstance = new CUI_Money(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CUI_Money");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Money::Clone(void * pArg)
{
	CUI_Money*	pInstance = new CUI_Money(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CUI_Money");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Money::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
